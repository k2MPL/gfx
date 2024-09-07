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
        Scene_TexturedTriangle.h
        Scene_TexturedTriangle.cpp
        Scene_TexturedCube.h
        Scene_TexturedCube.cpp
        
        interface_Logger.h
        interface_Logger.cpp
        interface_FileSystem.h
        interface_FileSystem.cpp

        gfx/GFXCore.h
        gfx/GFXPipeline.h
        gfx/GFXPipeline.cpp
        gfx/GFXShader.h
        gfx/GFXShader.cpp
        
        stb_impl.cpp)

target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb)