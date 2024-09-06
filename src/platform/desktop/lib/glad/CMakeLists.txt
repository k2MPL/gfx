cmake_minimum_required(VERSION 3.22.1)

set(TARGET_NAME "glad")

project(${TARGET_NAME})

add_library(${TARGET_NAME} STATIC
        src/glad.c)

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)