#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "esp_err.h"
#include "driver/gpio.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define IO_ON 1
#define IO_OFF 0

typedef enum
{
    ULTRASONIC_IDLE,
    ULTRASONIC_WAITING_FOR_RISING,
    ULTRASONIC_WAITING_FOR_FALLING,
    ULTRASONIC_COMPLETE
} ultrasonic_state_t;

typedef struct {
    gpio_num_t trig_pin; 
    gpio_num_t echo_pin;
    int64_t  rising_timestamp;
    int64_t  falling_timestamp;
    volatile ultrasonic_state_t state;
    TaskHandle_t measurement_task;
} ultrasonic_t;

esp_err_t ultrasonic_init(ultrasonic_t *sensor);
esp_err_t ultrasonic_measure(ultrasonic_t *sensor, float *distance_cm);

#endif