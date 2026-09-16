idf_component_register(
    SRCS "test_led.c"
    INCLUDE_DIRS "."
    REQUIRES unity led
)

idf_component_get_property(original_component_dir  driver COMPONENT_OVERRIDEN_DIR)
idf_component_mock(INCLUDE_DIRS "/opt/esp/idf/components/driver/gpio/include/"
                   MOCK_HEADER_FILES "/opt/esp/idf/components/driver/gpio/include/driver/gpio.h")