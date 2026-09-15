#ifndef LED_H
#define LED_H
#include "esp_err.h"
/*Generic Constants*/
#define IO_ON 1
#define IO_OFF 0

void led_on(int GPIO_PIN_NUMBER);
void led_off(int GPIO_PIN_NUMBER);
esp_err_t led_init(int GPIO_PIN_NUMBER);

#endif 