# SRC_DIRS - All .c files in this directory get compiled automatically
# REQUIRES - pulls the unity framework and links to the project_abc component that you want to test.
idf_component_register(SRCS "test_led.c" "mocks/mock_gpio.c"
                       INCLUDE_DIRS "." "mocks"
                       REQUIRES unity led)

idf_component_set_property(test WHOLE_ARCHIVE TRUE)

target_link_options(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=gpio_set_level")