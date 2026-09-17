# SRC_DIRS - All .c files in this directory get compiled automatically
# REQUIRES - pulls the unity framework and links to the project_abc component that you want to test.
idf_component_register(SRCS "test_led.c"
                       INCLUDE_DIRS "."
                       REQUIRES unity led)

