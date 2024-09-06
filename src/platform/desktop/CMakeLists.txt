cmake_minimum_required(VERSION 3.22.1)

set(TARGET_NAME "gfx_desktop")

project(${TARGET_NAME})

add_definitions(-DGFX_DESKTOP)

# Global include that selects GL (3.3 or ES)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)

add_executable(${TARGET_NAME} 
    main.cpp)

# GLFW
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw)
target_link_libraries(${TARGET_NAME} glfw)
add_definitions(-DGLFW_INCLUDE_NONE)

# Glad
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad glad)
target_link_libraries(${TARGET_NAME} glad)

# GFX
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. gfx)
target_link_libraries(${TARGET_NAME} gfx)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}

    #lib
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include

    #gfx
    ${CMAKE_CURRENT_SOURCE_DIR}/../..)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/../../data/
                                                           ${CMAKE_BINARY_DIR}/data/)