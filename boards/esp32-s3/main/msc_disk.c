#include "msc_disk.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "wear_levelling.h"
#include "tinyusb.h"
#include "tusb_msc_storage.h"
#include "ff.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

static const char *TAG="storage";
static wl_handle_t s_wl = WL_INVALID_HANDLE;
static bool s_local_mounted = false;

extern const char ini_default_start[] asm("_binary_config_ini_start");
extern const char ini_default_end[]   asm("_binary_config_ini_end");

bool storage_init(void){
    const esp_partition_t *p = esp_partition_find_first(
        ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, "storage");
    if(!p){ ESP_LOGE(TAG,"particao 'storage' (fat) nao encontrada"); return false; }
    if(wl_mount(p,&s_wl)!=ESP_OK){ ESP_LOGE(TAG,"wl_mount falhou"); return false; }
    const tinyusb_msc_spiflash_config_t cfg = { .wl_handle = s_wl };
    if(tinyusb_msc_storage_init_spiflash(&cfg)!=ESP_OK){ ESP_LOGE(TAG,"msc_storage_init falhou"); return false; }
    return true;
}

bool storage_fs_mount_local(const char *mp){
    if(tinyusb_msc_storage_mount(mp)!=ESP_OK){ ESP_LOGE(TAG,"mount local falhou"); return false; }
    s_local_mounted = true;
    char path[64]; snprintf(path,sizeof path,"%s/config.ini",mp);
    struct stat st;
    if(stat(path,&st)!=0){
        ESP_LOGW(TAG,"config.ini ausente, criando padrao");
        FILE *f=fopen(path,"w");
        if(f){ fwrite(ini_default_start,1,ini_default_end-ini_default_start,f); fclose(f); }
    }
    // rotulo do volume que aparece no PC (FAT: <= 11 chars). Grava so se diferente.
    char cur[16]={0};
    if(f_getlabel("",cur,NULL)!=FR_OK || strncmp(cur,"DDJ REV 7",11)!=0){
        f_setlabel("DDJ REV 7");
    }
    return true;
}

void storage_fs_unmount_local(void){
    if(s_local_mounted){ tinyusb_msc_storage_unmount(); s_local_mounted=false; }
}

void msc_usb_start(void){
    storage_fs_unmount_local();            // o PC precisa de acesso exclusivo
    const tinyusb_config_t tcfg = {
        .device_descriptor=NULL, .string_descriptor=NULL,
        .external_phy=false, .configuration_descriptor=NULL };
    ESP_ERROR_CHECK(tinyusb_driver_install(&tcfg));
    ESP_LOGI(TAG,"MSC ativo: edite config.ini pelo PC");
}
