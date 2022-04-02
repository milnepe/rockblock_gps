
add_executable(rockblock_gps rockblock_gps.cpp)

pico_set_program_name(rockblock_gps "rockblock_gps")
pico_set_program_version(rockblock_gps "0.1")

pico_enable_stdio_uart(rockblock_gps 0)
pico_enable_stdio_usb(rockblock_gps 1)


# Add the standard library to the build
target_link_libraries(rockblock_gps pico_stdlib pico_serial pico_rockmachine)

pico_add_extra_outputs(rockblock_gps)

