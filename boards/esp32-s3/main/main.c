// DDJ-REV7 USB auto-config  (ESP32-S3)
// Modo: ao ligar (LED azul) ha ~3 s observando o BOOT.
//   toque BOOT -> EDICAO (pendrive)   | sem toque -> HOST (aplica o .ini)
//   HOST sem controladora em ~10 s -> cai para EDICAO.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led.h"
#include "msc_disk.h"
#include "usb_sender.h"

#define MOUNT "/data"
#define INI   MOUNT "/config.ini"
#define BOOT_BTN GPIO_NUM_0
#define EDIT_WINDOW_MS 3000
static const char *TAG="main";

void app_main(void){
    led_init(); led_set(LED_IDLE);
    if(!storage_init()){ led_set(LED_ERROR); return; }
    // monta local p/ garantir o config.ini padrao
    if(!storage_fs_mount_local(MOUNT)){ led_set(LED_ERROR); return; }

    gpio_set_direction(BOOT_BTN,GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOOT_BTN,GPIO_PULLUP_ONLY);
    bool edit=false;
    ESP_LOGI(TAG,"toque BOOT em %d ms para modo EDICAO...",EDIT_WINDOW_MS);
    for(int t=0;t<EDIT_WINDOW_MS;t+=50){
        if(gpio_get_level(BOOT_BTN)==0){ edit=true; break; }
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    if(edit){
        ESP_LOGI(TAG,"BOOT tocado -> modo EDICAO (pendrive)");
        msc_usb_start();                 // solta a FAT local e expoe por USB
        while(1) vTaskDelay(pdMS_TO_TICKS(1000));
    }

    ESP_LOGI(TAG,"modo HOST: procurando DDJ-REV7...");
    int n = rev7_apply_from_ini(INI);    // le /data/config.ini (FAT ainda montada local)
    if(n<=0){
        // Nao da para cair para o pendrive aqui: o USB ja esta em modo host nesta
        // porta unica e o ESP nao troca host->device sem reiniciar. Entao so avisa.
        ESP_LOGW(TAG,"nenhuma controladora encontrada.");
        ESP_LOGW(TAG,"para EDITAR o config.ini: aperte RESET e toque BOOT nos primeiros 3s.");
        led_set(LED_IDLE);   // azul
    }
    while(1) vTaskDelay(pdMS_TO_TICKS(1000));
}
