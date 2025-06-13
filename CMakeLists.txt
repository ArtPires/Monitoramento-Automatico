cmake_minimum_required(VERSION 3.10)
project(MonitoramentoAutomatico)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-g)

# Detectar arquitetura com base no nome do compilador
get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)

if(COMPILER_NAME MATCHES "aarch64.*")
    message(STATUS "Compilando para arquitetura AARCH64 (detectado via compilador: ${COMPILER_NAME})")
    set(USE_WIRINGPI TRUE)
elseif(COMPILER_NAME MATCHES "x84_64*")
    message(STATUS "Compilando para arquitetura x86_64 ou outra (detectado via compilador: ${COMPILER_NAME})")
    set(USE_WIRINGPI FALSE)
else()
    message(STATUS "Arquitetura nao suportada: ${COMPILER_NAME})")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    #${CMAKE_SOURCE_DIR}/test/include
)

if(USE_WIRINGPI)
    message(STATUS "Include WiringPi")
    include_directories(${CMAKE_SOURCE_DIR}/3rdparty/includes/wiringPi)
endif()

add_subdirectory(3rdparty/backward-cpp)
include_directories(3rdparty/backward-cpp)

add_subdirectory(3rdparty/json)
include_directories(3rdparty/json/include)

file(GLOB_RECURSE SRC_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    #${CMAKE_SOURCE_DIR}/test/src/*.cpp
)

add_executable(AutoMonitor ${SRC_FILES})

target_link_libraries(AutoMonitor
    backward            # backward-cpp
    dl                  # para stack trace
    nlohmann_json::nlohmann_json
    #unwind              # para stack trace
)

# WiringPi (somente para builds ARM)
if(USE_WIRINGPI)
    message(STATUS "Linkando WiringPi")
    target_link_libraries(AutoMonitor
        ${CMAKE_SOURCE_DIR}/3rdparty/pre-compiled-libs/libwiringpi.a
    )
endif()