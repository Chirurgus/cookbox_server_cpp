project(cookbox_server VERSION 0.1.0 LANGUAGES CXX)

enable_testing()
find_package(GTest REQUIRED)

add_executable(test.exe ${TEST_DIR}/test.cpp ${SRC_DIR}/recipe.cpp ${SRC_DIR}/recipe_storage.cpp)

gtest_add_tests(test.exe SOURCES recipe_test.cpp storage_test.cpp)
set_target_properties(test.exe PROPERTIES
				CXX_STANDARD 14
				CXX_STANDARD_REQUIRED ON)
target_compile_options(test.exe PRIVATE -Wall)
target_link_libraries(test.exe ${cpprestsdk_LINK_LIBRARIES} sqlite3 ${GTEST_BOTH_LIBRARIES})