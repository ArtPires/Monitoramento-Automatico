cmake_minimum_required(VERSION 3.10)

project(MonitoramentoAutomatico)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/pre-compiled-libs/include)

add_executable(AutoMonitor main.cpp)

target_link_libraries(AutoMonitor
    ${CMAKE_SOURCE_DIR}/3rdparty/pre-compiled-libs/libwiringpi.a
)
