# Created by Alexander Sorochynskyi
# On 15/05/18

cmake_minimum_required(VERSION 3.5.1)

add_executable(cb_test test.cpp)

find_package(GTest REQUIRED)

set_target_properties(cb_test PROPERTIES CXX_STANDARD 14)
target_compile_options(cb_test PRIVATE -Wall)
target_link_libraries(cb_test recipe_server ${GTEST_BOTH_LIBRARIES})