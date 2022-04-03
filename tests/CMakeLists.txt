add_executable(get_response
        get_response.cpp
        )

# pull in common dependencies
target_link_libraries(get_response pico_stdlib pico_rockmachine)

# enable usb output, disable uart output
pico_enable_stdio_usb(get_response 1)
pico_enable_stdio_uart(get_response 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(get_response)

# add url via pico_set_program_url
#example_auto_set_url(get_response)