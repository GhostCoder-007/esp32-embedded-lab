
#ifndef MOCK_GPIO_H
#define MOCK_GPIO_H

#include "soc/gpio_num.h"
#include <stdint.h>

typedef struct {
    gpio_num_t gpio_num;
    uint32_t level;
} gpio_dev_t;

void gpio_mock_reset(void);
gpio_dev_t gpio_mock_get_last_call(void);

#endif