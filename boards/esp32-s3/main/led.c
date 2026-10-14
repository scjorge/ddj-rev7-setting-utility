#include "led.h"
#include "sdkconfig.h"
#include "led_strip.h"
#include "driver/gpio.h"

// ESP32-S3 DevKit: LED RGB WS2812 em GPIO48 (ajuste em menuconfig se necessário).
#ifndef CONFIG_REV7_LED_GPIO
#define CONFIG_REV7_LED_GPIO 48
#endif

static led_strip_handle_t strip;

void led_init(void){
    led_strip_config_t sc = { .strip_gpio_num = CONFIG_REV7_LED_GPIO, .max_leds = 1 };
    led_strip_rmt_config_t rc = { .resolution_hz = 10*1000*1000 };
    led_strip_new_rmt_device(&sc,&rc,&strip);
    led_strip_clear(strip);
}

void led_set(led_state_t st){
    uint8_t r=0,g=0,b=0;
    switch(st){
        case LED_OFF:   break;
        case LED_IDLE:  b=20; break;            // azul fraco = pronto/aguardando
        case LED_BUSY:  r=40; g=25; break;      // âmbar = enviando
        case LED_OK:    g=50; break;            // verde = concluído com sucesso
        case LED_ERROR: r=60; break;            // vermelho = erro
    }
    led_strip_set_pixel(strip,0,r,g,b);
    led_strip_refresh(strip);
}
