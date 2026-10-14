#pragma once
typedef enum { LED_OFF, LED_IDLE, LED_BUSY, LED_OK, LED_ERROR } led_state_t;
void led_init(void);
void led_set(led_state_t st);
