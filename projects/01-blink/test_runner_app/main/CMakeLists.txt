# This file registers the "main" component for the test_runner_app "project".
# In ESP-IDF the build system is entirely component-based -> Every project must have at least one component.
idf_component_register(SRCS "test_runner_app.c"
                       INCLUDE_DIRS "."
                       REQUIRES unity test)