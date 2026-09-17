#include "unity.h"
#include "led.h"


led_config_t led = {
        .pin = GPIO_NUM_2
    };
const char* TAG = "TEST";

void setUp(void) {
    // set stuff up here
    ESP_LOGW(TAG, "-----> Running setUP()");
}

void tearDown(void) {
    // clean stuff up here
    ESP_LOGW(TAG, "-----> Running tearDown()");
}

TEST_CASE("Initialize", "[init]")
{
    TEST_ASSERT_EQUAL(ESP_OK, led_init(&led));       // Should return ESP_OK if already initialized.
}

// TEST_CASE("LED OFF", "[LED-OFF]")
// {
//     TEST_ASSERT_EQUAL(ESP_OK, led_init(&led));       // Should return ESP_OK if GPIO commands were correct
// }

