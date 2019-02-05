cmake_minimum_required(VERSION 3.1)

project(rpi-uav)

set(FLAGS "-g -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -Wall -Wpedantic -Wextra")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")

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
    src/control.cpp
    src/config.cpp
    src/hardware.cpp
    src/logging.cpp
    src/transmission.cpp
    src/transmission_impl.cpp
    src/comms.cpp
    test/Assert.cpp
    test/Test_all.cpp
    test/Test_FloatToBinary.cpp
    test/Test_Transmission.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main controller)
add_executable(unit_test test/Unit-Tests-PC.cpp)
target_link_libraries(unit_test controller)
add_executable(bin2txt src/bin2txt.cpp)
target_link_libraries(bin2txt controller)
add_executable(pack src/pack.cpp)
target_link_libraries(pack controller)
