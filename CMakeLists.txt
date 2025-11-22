cmake_minimum_required(VERSION 3.10)
project(Gateway)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/Src/*.cpp
    ${CMAKE_SOURCE_DIR}/Src/Hw/*.cpp
)

# EXTERNAL DEPENDENCIES ################################
add_library(json_external INTERFACE)
target_include_directories(json_external INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/External/JsonCpp>
)

list(APPEND CMAKE_PREFIX_PATH
    ${CMAKE_SOURCE_DIR}/External/paho.mqtt.c/install
    ${CMAKE_SOURCE_DIR}/External/paho.mqtt.cpp/install
    ${CMAKE_SOURCE_DIR}/External/libmodbus/install
)
find_package(PahoMqttCpp REQUIRED)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(LIBMODBUS REQUIRED libmodbus)

# Add open62541 OPC UA library
add_subdirectory(${CMAKE_SOURCE_DIR}/External/open62541)


# Main Target ##########################################
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Inc/Utils
    ${CMAKE_SOURCE_DIR}/Inc/Hw
    ${CMAKE_SOURCE_DIR}/External/open62541
    ${CMAKE_SOURCE_DIR}/External/libmodbus/install/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/External/libmodbus/install/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    json_external
    PahoMqttCpp::paho-mqttpp3
    pthread ssl crypto
    open62541
    modbus
)

# Unit tests ##########################################
option(BUILD_TESTS "Enable building unit tests" OFF)

if(BUILD_TESTS)
    # Add GoogleTest
    add_subdirectory(${CMAKE_SOURCE_DIR}/External/googletest)
    
    file(GLOB_RECURSE TEST_SOURCES
        ${CMAKE_SOURCE_DIR}/Tests/*.cpp
    )

    add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})
    target_include_directories(${PROJECT_NAME}_tests PUBLIC
        ${CMAKE_SOURCE_DIR}/Inc
        ${CMAKE_SOURCE_DIR}/Inc/Utils
        ${CMAKE_SOURCE_DIR}/Inc/Hw
    )

    target_link_libraries(${PROJECT_NAME}_tests
        gtest
        gtest_main
        pthread
        json_external
        PahoMqttCpp::paho-mqttpp3
        open62541
    )

    enable_testing()
    add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

endif()