cmake_minimum_required(VERSION 3.10)

project(MonitoramentoAutomatico)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
    ${CMAKE_SOURCE_DIR}/3rdparty/includes/wiringpi
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(AutoMonitor ${SRC_FILES})

target_link_libraries(AutoMonitor
    ${CMAKE_SOURCE_DIR}/3rdparty/pre-compiled-libs/libwiringpi.a
)
