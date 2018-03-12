cmake_minimum_required(VERSION 3.5)

project(cookbox_server VERSION 0.1.0 LANGUAGES CXX)

# Point this to sqlite_modern_cpp source dir
set(sqlite_modern_cpp_SOURCE_DIR ../sqlite_modern_cpp)
include_directories(${sqlite_modern_cpp_SOURCE_DIR}/hdr)

find_package(nlohmann_json REQUIRED)

find_package(Pistache CONFIG REQUIRED)
#set(pistache_DIR /usr/local/lib/cmake/pistache)
#include_directories(${pistache_INCLUDE_DIR})
 
add_executable(test.exe src/test.cpp)
# add_executable(cookbox_server.exe cookbox_server.cpp)# recipe.cpp)
set_property(TARGET test.exe PROPERTY CXX_STANDARD 14)

target_compile_options(test.exe PRIVATE -Wall)
target_link_libraries(test.exe pistache sqlite3)
