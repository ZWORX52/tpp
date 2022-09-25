cmake_minimum_required(VERSION 3.10)

project(tpp)

# had to manually remake compile_commands.json for some reason
# :shruggie:
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")
set(CXX "/usr/bin/g++")
set(SOURCES main.cpp)
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${INCLUDE})
add_executable(tpp ${SOURCES})

add_custom_command(
    OUTPUT ${SRC}/glad.c
    COMMAND python -m glad --profile compatibility --out-path ../ --generator c --quiet
    VERBATIM)
add_library(glad ${SRC}/glad.c)
add_dependencies(tpp glad)

target_link_libraries(tpp glfw)
target_link_libraries(tpp GL)
target_link_libraries(tpp glad)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -Wshadow)
endif()

