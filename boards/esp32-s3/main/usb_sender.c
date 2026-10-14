#include "usb_sender.h"
#include "rev7_map.h"
#include "ini_parser.h"
#include "led.h"
#include "esp_log.h"
#include "usb/usb_host.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG="rev7_usb";

// ---- host lifecycle ----
static void host_lib_task(void *arg){
    while(1){
        uint32_t ev; usb_host_lib_handle_events(portMAX_DELAY,&ev);
    }
}

typedef struct { usb_host_client_handle_t client; uint8_t dev_addr; bool present; } ctx_t;
static ctx_t s_ctx;

static void client_cb(const usb_host_client_event_msg_t *msg,void *arg){
    ctx_t *c=arg;
    if(msg->event==USB_HOST_CLIENT_EVENT_NEW_DEV){ c->dev_addr=msg->new_dev.address; c->present=true; }
    else if(msg->event==USB_HOST_CLIENT_EVENT_DEV_GONE){ c->present=false; }
}

// envia 1 control transfer e espera terminar
typedef struct { SemaphoreHandle_t done; int status; int xfered; } ctrl_wait_t;
static void ctrl_cb(usb_transfer_t *t){
    ctrl_wait_t *w=t->context;
    w->status=t->status; w->xfered=t->actual_num_bytes;
    xSemaphoreGive(w->done);
}

// Faz um control transfer (setup em transfer->data_buffer[0..7], dados a partir de [8]).
static esp_err_t do_ctrl(usb_host_client_handle_t cli, usb_device_handle_t dev,
                         uint8_t bm,uint8_t breq,uint16_t wval,uint16_t widx,
                         uint8_t *data,uint16_t wlen,uint8_t *out,int *out_len){
    usb_transfer_t *t;
    ESP_ERROR_CHECK(usb_host_transfer_alloc(8+ (wlen?wlen:64),0,&t));
    usb_setup_packet_t *s=(usb_setup_packet_t*)t->data_buffer;
    s->bmRequestType=bm; s->bRequest=breq; s->wValue=wval; s->wIndex=widx; s->wLength=wlen;
    if(wlen && !(bm&0x80) && data) memcpy(t->data_buffer+8,data,wlen);
    t->num_bytes=8+wlen;
    t->bEndpointAddress=0x00;
    t->device_handle=dev;
    ctrl_wait_t w={ .done=xSemaphoreCreateBinary() };
    t->callback=ctrl_cb; t->context=&w;
    esp_err_t r=usb_host_transfer_submit_control(cli,t);
    if(r==ESP_OK){
        xSemaphoreTake(w.done,pdMS_TO_TICKS(1000));
        r = (w.status==USB_TRANSFER_STATUS_COMPLETED)?ESP_OK:ESP_FAIL;
        if(out && out_len && (bm&0x80)){
            int n=w.xfered-8; if(n<0)n=0;
            memcpy(out,t->data_buffer+8,n); *out_len=n;
        }
    }
    vSemaphoreDelete(w.done);
    usb_host_transfer_free(t);
    return r;
}

int rev7_apply_from_ini(const char *ini_path){
    led_set(LED_IDLE);
    const usb_host_config_t hcfg={ .intr_flags=ESP_INTR_FLAG_LEVEL1 };
    if(usb_host_install(&hcfg)!=ESP_OK){ ESP_LOGE(TAG,"host install"); return -1; }
    TaskHandle_t lib; xTaskCreate(host_lib_task,"usb_lib",4096,NULL,5,&lib);

    memset(&s_ctx,0,sizeof s_ctx);
    const usb_host_client_config_t ccfg={ .max_num_event_msg=5,
        .async={ .client_event_callback=client_cb, .callback_arg=&s_ctx } };
    if(usb_host_client_register(&ccfg,&s_ctx.client)!=ESP_OK){ return -2; }

    // Escaneia por ~15 s e LISTA no log TODO dispositivo USB que aparecer.
    // Se achar a DDJ-REV7, aplica os ajustes; os demais sao so listados.
    ESP_LOGI(TAG,"=== escaneando dispositivos USB por ~15 s (pisca ambar) ===");
    usb_device_handle_t dev=NULL;
    bool is_rev7=false;
    int last_handled=-1;
    for(int i=0;i<300 && !is_rev7;i++){            // 300 x 50ms = 15s
        usb_host_client_handle_events(s_ctx.client,pdMS_TO_TICKS(50));
        led_set((i/4)%2 ? LED_BUSY : LED_OFF);     // pisca ambar = procurando
        if(s_ctx.present && s_ctx.dev_addr!=last_handled){
            last_handled=s_ctx.dev_addr;
            usb_device_handle_t d;
            if(usb_host_device_open(s_ctx.client,s_ctx.dev_addr,&d)==ESP_OK){
                const usb_device_desc_t *dd; usb_host_get_device_descriptor(d,&dd);
                ESP_LOGI(TAG,"DISPOSITIVO ENCONTRADO: VID=%04x PID=%04x classe=0x%02x subclasse=0x%02x bcdUSB=%04x",
                         dd->idVendor,dd->idProduct,dd->bDeviceClass,dd->bDeviceSubClass,dd->bcdUSB);
                if(dd->idVendor==REV7_VID && dd->idProduct==REV7_PID){
                    ESP_LOGI(TAG,"  -> e a DDJ-REV7! aplicando config.ini...");
                    dev=d; is_rev7=true; break;
                } else {
                    ESP_LOGW(TAG,"  -> outro dispositivo (nao e a DDJ-REV7), apenas listado");
                    usb_host_device_close(s_ctx.client,d);
                }
            } else {
                ESP_LOGE(TAG,"falha ao abrir dispositivo addr=%d",s_ctx.dev_addr);
            }
        }
    }
    if(!is_rev7){ ESP_LOGW(TAG,"scan terminou: nenhuma DDJ-REV7 encontrada"); return 0; }

    led_set(LED_BUSY);
    int ok=0, total=0;
    for(int i=0;i<REV7_N_SETTINGS;i++){
        const rev7_setting_t *S=&REV7_SETTINGS[i];
        bool en; int opt;
        if(!ini_get_setting(ini_path,S->section,&en,&opt)) continue;
        if(!en) continue;
        if(opt<1||opt>S->n_options){ ESP_LOGE(TAG,"%s: option %d invalida",S->section,opt); continue; }
        total++;
        uint16_t wval=S->options[opt-1].wValue;
        const char *lab=S->options[opt-1].label;
        // SET
        esp_err_t r=do_ctrl(s_ctx.client,dev,REV7_BM_SET,REV7_BREQUEST_SET,wval,S->wIndex,NULL,0,NULL,NULL);
        if(r!=ESP_OK){ ESP_LOGE(TAG,"%s = %s  FALHA no envio",S->section,lab); led_set(LED_ERROR); continue; }
        // CONFIRMA via GET (quando aplicável: roteamento/nível devolvem o byte atual)
        bool verified=true;
        uint8_t rb[8]; int rlen=0;
        uint16_t getval = (S->wIndex==0xC002)? (wval & 0xFF00) : 0x0000; // seleciona porta no roteamento
        if(do_ctrl(s_ctx.client,dev,REV7_BM_GET,REV7_BREQUEST_GET,getval,S->wIndex,NULL,2,rb,&rlen)==ESP_OK && rlen>=1){
            ESP_LOGI(TAG,"%s = %s  OK (readback=%02x %02x)",S->section,lab,rb[0],rlen>1?rb[1]:0);
        } else {
            ESP_LOGW(TAG,"%s = %s  enviado (sem readback)",S->section,lab);
            verified=false;
        }
        (void)verified;
        ok++;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    ESP_LOGI(TAG,"aplicados %d/%d ajustes",ok,total);
    led_set(ok>0 && ok==total ? LED_OK : (ok>0?LED_OK:LED_ERROR));

    usb_host_device_close(s_ctx.client,dev);
    return ok;
}
