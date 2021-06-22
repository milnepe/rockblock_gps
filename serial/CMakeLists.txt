add_library(
    pico_serial
    src/pico/serial.hpp
    src/pico/serial.cpp
)

target_include_directories(pico_serial PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(pico_serial PRIVATE pico_stdlib)