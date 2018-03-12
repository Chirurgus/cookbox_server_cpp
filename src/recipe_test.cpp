#include "../sqlite_modern_cpp/hdr/sqlite_modern_cpp.h"

#include "recipe_test.h"

namespace recipe_test {

using Recipe = recipe::Recipe;

// returns true iff all tests sucessful
bool run_tests(std::ostream& os) {
	try {
		write_to_db();
	}
	catch (Recipe_test_fail& e) {
		os << "Exception thrown: " << e.what() << std::endl;		
		return false;
	}
	return true;
}

//return recipes for testing
std::vector<Recipe> get_test_recipe() {
	std::vector<Recipe> ret;

	Recipe::id_type id = std::numeric_limits<Recipe::id_type>::max();

	Recipe r1;
	r1.id = id--;// a big number
	r1.name = "a test recipe";
	r1.short_description = "dummy recipe for tests";
	r1.long_description = "One of may great recipes that "
				"you can, and should use for testing"
				"the app. Multiple recipes can be "
				"r1urned to test varios cases, like null "
				"id for new recipes";
	r1.target_quantity = 3.5;
	r1.target_description = "times better testing";
	r1.preparation_time = .5;//in hours
	r1.source = "Yours one and only, me";
	
	r1.ingredient_list.push_back(Recipe::Ingredient {1, "passion"});
	r1.ingredient_list.push_back(Recipe::Ingredient {3.5, "g. enthusiasm"});
	r1.ingredient_list.push_back(Recipe::Ingredient {200, "hours of work!! ><"});

	r1.instruction_list.push_back("Be frustrated with oragazing recipes");
	r1.instruction_list.push_back("Remember that you know how to program");
	r1.instruction_list.push_back("Spend a year of your life doing this");
	r1.instruction_list.push_back("Be happy :)");

	r1.comment_list.push_back("This one is my favorite");
	r1.comment_list.push_back("You can add a little cinamon");
	
	ret.push_back(r1);	
	
	return ret;
}

// get test recipes, but without an id, to test new incertions
std::vector<Recipe> get_test_recipe_no_id() {
	std::vector<Recipe> ret {get_test_recipe()};
	for (auto& i : ret) {
		i.id = Recipe::no_id;
	}	
	return ret;
}

void write_to_db() {
	sqlite::database db {"recipe.db"};	
	try {
		for (auto i : get_test_recipe()) {
			db << i;
		}
		for (auto i : get_test_recipe_no_id()) {
			db << i;
			if (i.id == Recipe::no_id) {
				throw Recipe_test_fail
					{"writing no_id recpe to db didn't set id"};
			}
		}
	}
	catch(std::exception& e) {
		throw Recipe_test_fail {e.what()};	
	}
}

}// namespace recipe_test
