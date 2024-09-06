cmake_minimum_required(VERSION 3.22.1)

set(TARGET_NAME "gfx")

project(${TARGET_NAME})

add_library(${TARGET_NAME} STATIC
        Engine.h
        Engine.cpp
        Scene.h
        Scene.cpp
        Scene_Triangle.h
        Scene_Triangle.cpp
        Scene_TexturedCube.h
        Scene_TexturedCube.cpp)

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)