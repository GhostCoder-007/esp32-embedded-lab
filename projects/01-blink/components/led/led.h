#ifndef LED_H
#define LED_H
#include "esp_err.h"
#include "esp_log.h"
#include <stdint.h>
#include "driver/gpio.h"
/*Generic Constants*/
#define IO_ON 1
#define IO_OFF 0

typedef struct
{
    gpio_num_t  pin;
} led_config_t;

esp_err_t led_init(led_config_t *led_config);
void led_on(led_config_t *led_config);
void led_off(led_config_t *led_config);


#endif 