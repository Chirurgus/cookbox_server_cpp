#include <exception>
#include <iostream>

#include "recipe_test.h"

int main() {
	if (recipe_test::run_tests(std::cout)) {
		std::cout << "All tests sucessful." << std::endl;
		return 0;
	}
	else {
		std::cout << "Test failed" << std::endl;
		return 1;
	}
}
