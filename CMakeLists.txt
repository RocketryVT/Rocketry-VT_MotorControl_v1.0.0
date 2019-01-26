cmake_minimum_required(VERSION 3.1)

project(rpi-uav)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

option(DEBUG "Toggle debugging output" ON)

if(DEBUG)
    add_definitions(-DDEBUG)
endif(DEBUG)

include_directories(include)
include_directories(test)
 
add_library(controller
    src/Adafruit_MAX31855.cpp
    src/control.cpp
    src/config.cpp
    src/Hardware.cpp
    src/HX711.cpp
    src/motor.cpp
    src/Transmission.cpp
    src/Transmission_Impl.cpp
    src/XBee_IO.cpp
    test/Assert.cpp
    test/Test_all.cpp
    test/Test_FloatToBinary.cpp
    test/Test_Transmission.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main controller)
add_executable(unit_test test/Unit-Tests-PC.cpp)
target_link_libraries(unit_test controller)
