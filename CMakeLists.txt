cmake_minimum_required(VERSION 3.10)

project(tpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")
set(SOURCES main.cpp)

add_executable(tpp ${SOURCES})

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/
    COMMAND python -m glad --profile compatibility --out-path ../ --generator c --quiet
    VERBATIM)
add_custom_target(glad_headers ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/)
add_dependencies(tpp glad_headers)

target_link_libraries(tpp glfw)
target_link_libraries(tpp GL)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -Wshadow)
endif()

