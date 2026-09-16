#include "unity.h"
#include "led.h"

void test_led_init(void)
{
    led_config_t led = {
        .pin = GPIO_NUM_2
    };

    
    TEST_ASSERT_EQUAL(ESP_OK, led_init(&led));
}

TEST_CASE("[1] Initialize", "[init]")
{

}

void app_main(void)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_led_init);

    UNITY_END();
}