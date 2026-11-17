#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "esp_err.h"
#include "driver/gpio.h"
#include <time.h>

#define IO_ON 1
#define IO_OFF 0

typedef struct {
    gpio_num_t trig_pin; 
    gpio_num_t echo_pin;
    time_t starttime;
    time_t endtime;
} ultrasonic_t;

esp_err_t ultrasonic_init(ultrasonic_t *sensor);

#define ECHO_PIN GPIO_NUM_23
#define TRIG_PIN GPIO_NUM_22

#endif