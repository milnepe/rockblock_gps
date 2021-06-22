add_executable(rockblock_gps rockblock_gps.cpp 
    rock_machine.cpp
    rock_machine_state.cpp
    rock_machine_idle_wait_state.cpp
    rock_machine_rts_state.cpp
    rock_machine_rts_wait_state.cpp
    rock_machine_flowctrl_state.cpp
    rock_machine_flowctrl_wait_state.cpp
    rock_machine_mobuffer_state.cpp
    rock_machine_mobuffer_wait_state.cpp
    rock_machine_sendbad_wait_state.cpp
    rock_machine_sendgood_wait_state.cpp
    rock_machine_send_state.cpp
    rock_machine_send_wait_state.cpp
)

pico_set_program_name(rockblock_gps "rockblock_gps")
pico_set_program_version(rockblock_gps "0.1")

pico_enable_stdio_uart(rockblock_gps 1)
pico_enable_stdio_usb(rockblock_gps 1)

# Add the standard library to the build
target_link_libraries(rockblock_gps pico_stdlib pico_serial)

pico_add_extra_outputs(rockblock_gps)

