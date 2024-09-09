cmake_minimum_required(VERSION 3.22.1)

set(TARGET_NAME "gfx")

project(${TARGET_NAME})

add_library(${TARGET_NAME} STATIC
        GlobalRegistry.h
        GlobalRegistry.cpp
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
        gfx/GFXShader.h
        gfx/GFXShader.cpp
        gfx/GFXTexture2D.h
        gfx/GFXTexture2D.cpp

        ProtectedID.h
        stb_impl.cpp)

# Assimp

set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_INSTALL_PDB OFF)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp)
target_link_libraries(${TARGET_NAME} assimp)

target_include_directories(${TARGET_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

        # lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/assimp/include
        ${CMAKE_CURRENT_BINARY_DIR}/lib/assimp/include) # Generated header files are placed here. https://github.com/assimp/assimp/issues/1566