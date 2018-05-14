if (MSVC) 
	cmake_minimum_required(VERSION 3.7.0)
else()
	cmake_minimum_required(VERSION 3.5.1)
endif()

project(cookbox_server VERSION 0.1.0 LANGUAGES CXX)

set(sqlite_orm_SOURCE_DIR ../sqlite_orm)

if (MSVC) 
	# These are needed for cmake not in VS to find the librareis
	set(sqlite3_DIR "C:/Documents/Projects/vcpkg/installed/x64-windows/share/sqlite3")
	#set(cpprest_SOURCE_DIR C:/Documents/Projects/cpprestsdk)
	#set(cpprest_INC_DIR ${cpprest_SOURCE_DIR}/Release/include)
	set(ZLIB_INCLUDE_DIR "C:/Documents/Projects/vcpkg/installed/x64-windows/include")
	set(ZLIB_LIBRARY "C:/Documents/Projects/vcpkg/installed/x64-windows/lib")
	set(GTEST_INCLUDE_DIR "C:/Documents/Projects/vcpkg/installed/x64-windows/include")
	set(GTEST_LIBRARY "C:/Documents/Projects/vcpkg/installed/x64-windows/lib/manual-link/gtest.lib")
	set(GTEST_MAIN_LIBRARY "C:/Documents/Projects/vcpkg/installed/x64-windows/lib/manual-link/gtest_main.lib")
	set(cpprestsdk_DIR "C:/Documents/Projects/vcpkg/installed/x64-windows/share/cpprestsdk")

	set(OPENSSL_INCLUDE_DIR "C:/Documents/Projects/vcpkg/installed/x64-windows/lib")
	set(GTEST_INCLUDE_DIR "C:/Documents/Projects/vcpkg/installed/x64-windows/lib/manual-link")
else()

endif()

# cpprest asks to link agains this
set(cpprestsdk_LINK_LIBRARIES cpprestsdk::cpprest cpprestsdk::cpprestsdk_boost_internal cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_openssl_internal)

set(SRC_DIR src)
set(INC_DIR include)
set(TEST_DIR test)

#include_directories(${cpprest_INC_DIR})
include_directories(${sqlite_orm_SOURCE_DIR}/include)
include_directories(${INC_DIR})


# point to scripts
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(cpprestsdk REQUIRED)
# as far as I can tell, orm_sqlite is fine without this
#find_package(sqlite3 REQUIRED)

add_executable(cookbox_server.exe ${SRC_DIR}/cookbox_server.cpp ${SRC_DIR}/recipe.cpp ${SRC_DIR}/recipe_storage.cpp ${SRC_DIR}/recipe_server.cpp)
set_target_properties(cookbox_server.exe PROPERTIES
					CXX_STANDARD 14
					CXX_STANDARD_REQUIRED ON)
target_compile_options(cookbox_server.exe PRIVATE -Wall)
target_link_libraries(cookbox_server.exe sqlite3 cpprestsdk::cpprest boost_system) #${cpprestsdk_LINK_LIBRARIES})


set(BUILD_TESTS ON)

if(BUILD_TESTS) 
	add_subdirectory(test)
endif()
