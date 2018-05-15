# Created by Alexander Sorochynskyi
# On 15/05/18

cmake_minimum_required(VERSION 3.5.1)

project(cookbox_server VERSION 0.1.0
                       LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(test)

add_executable(cb_server main.cpp)

set_target_properties(cb_server PROPERTIES CXX_STANDARD 14)
target_compile_options(cb_server PRIVATE -Wall)
target_link_libraries(cb_server recipe_server)