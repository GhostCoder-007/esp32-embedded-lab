#include "esp_err.h"
#ifndef LED_H
#define LED_H

/*Generic Constants*/
#define IO_ON 1
#define IO_OFF 0

void led_on(void);
void led_off(void);
esp_err_t led_init(void);

#endif 