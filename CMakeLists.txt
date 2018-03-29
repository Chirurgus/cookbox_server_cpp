cmake_minimum_required(VERSION 3.5)

project(cookbox_server VERSION 0.1.0 LANGUAGES CXX)

set(sqlite_orm_SOURCE_DIR ../sqlite_orm)
set(SRC_DIR src)
set(INC_DIR include)
set(TEST_DIR test)

include_directories(${sqlite_orm_SOURCE_DIR}/include)
include_directories(${INC_DIR})

find_package(nlohmann_json REQUIRED)

find_package(Pistache CONFIG REQUIRED)

#add_subdirectory(test/googletest)

add_executable(cookbox_server.exe ${SRC_DIR}/cookbox_server.cpp ${SRC_DIR}/recipe.cpp ${SRC_DIR}/recipe_storage.cpp)
set_target_properties(cookbox_server.exe PROPERTIES
					CXX_STANDARD 14
					CXX_STANDARD_REQUIRED ON)
target_compile_options(cookbox_server.exe PRIVATE -Wall)
target_link_libraries(cookbox_server.exe pistache sqlite3)


enable_testing()
find_package(GTest REQUIRED)

add_executable(test.exe ${TEST_DIR}/recipe_test.cpp ${SRC_DIR}/recipe.cpp ${SRC_DIR}/recipe_storage.cpp)

set_target_properties(test.exe PROPERTIES
				CXX_STANDARD 14
				CXX_STANDARD_REQUIRED ON)
target_compile_options(test.exe PRIVATE -Wall)
target_link_libraries(test.exe pistache sqlite3 ${GTEST_BOTH_LIBRARIES})

