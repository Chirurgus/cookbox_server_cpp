# Created by Alexander Sorochynskyi
# On 15/05/18

cmake_minimum_required(VERSION 3.5.1)

project(recipe_server)


add_library(recipe_server SHARED
                          recipe.cpp
                          recipe_storage.cpp
                          recipe_server.cpp)

find_package(cpprestsdk REQUIRED)

set_property(TARGET recipe_server PROPERTY CXX_STANDARD 14)
target_compile_options(recipe_server PRIVATE -Wall)
target_include_directories(recipe_server PUBLIC ../include ../../sqlite_orm/include)
target_link_libraries(recipe_server sqlite3 cpprestsdk::cpprest boost_system)