add_library(
    pico_rockmachine
    src/pico/rock_machine.hpp
    src/pico/rock_machine.cpp
    src/pico/rock_machine_state.cpp
    src/pico/rock_machine_idle_wait_state.cpp
    src/pico/rock_machine_rts_state.cpp
    src/pico/rock_machine_rts_wait_state.cpp
    src/pico/rock_machine_flowctrl_state.cpp
    src/pico/rock_machine_flowctrl_wait_state.cpp
    src/pico/rock_machine_mobuffer_state.cpp
    src/pico/rock_machine_mobuffer_wait_state.cpp
    src/pico/rock_machine_sendbad_wait_state.cpp
    src/pico/rock_machine_sendgood_wait_state.cpp
    src/pico/rock_machine_send_state.cpp
    src/pico/rock_machine_send_wait_state.cpp
)

target_include_directories(pico_rockmachine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(pico_rockmachine pico_stdlib pico_serial)