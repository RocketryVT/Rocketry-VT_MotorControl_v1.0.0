cmake_minimum_required(VERSION 3.1)

project(rpi-uav)

set(FLAGS "-g -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -Wall -Wpedantic -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(include)
include_directories(include/drivers)
include_directories(test)
 
add_library(controller
    src/control.cpp
    src/config.cpp
    src/hardware.cpp
    src/logging.cpp
    src/transmission.cpp
    src/behavior.cpp
    src/comms.cpp
    src/predicates.cpp
    src/drivers/led.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main controller)
add_executable(bin2txt src/bin2txt.cpp)
target_link_libraries(bin2txt controller)
add_executable(pack src/pack.cpp)
target_link_libraries(pack controller)
add_executable(rand src/rand.cpp)
target_link_libraries(rand controller)
add_executable(drivertest src/drivers/test.cpp)
target_link_libraries(drivertest controller)
add_executable(readin src/readin.cpp)
target_link_libraries(readin controller)
