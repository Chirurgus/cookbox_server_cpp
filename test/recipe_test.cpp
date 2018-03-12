#include <this/package/foo.h>
#include <gtest/gtest.h>

namespace {

}// namespace

class RecipeTest : public ::testing::Test {
protectd:
	RecipeTest() 
		: r {std::numeric_limits<Recipe::id_type>::max(),
			"Test recipe",
			"An easy one, to test your program",
			"Every since the good ol' 18' that I'm using this recipe to test my recipe programs.",
			3.14f,
			"g. of skill to get this working."
			2,
			"Yours one and only Alexander",
			{ {10, "Years C++ experience ;)"}, 
			  {3, "Years in university"},
			  {50, "ish hours of your time"},
			  {1000, "motivations to do this"},
			  {5, "annoyances with not having a way to stock recipes"} },
			{ "Be anoyed with the way you oganaized recipes",
			  "Lean Java + Android",
			  "Want same thing on your PC",
			  "Want to sync data" },
			{ "Dont' hesitate a a little cinamon (on your system)",
			  "Goes best with coffe and marshmellows" }
		   }
		, no_id_r {r};
		, long_r {r};
		, basic_r {r};
		, empty_r {};
		, no_ing_r {r};
		, no_cmnt_r {r}	
	{
		no_id_r.id = Recipe::no_id;
		
		long_r.id = r.id - 1;
	
		/*
		long_r.ingredient_list.push_back();
		long_r.instruction_list.push_back();
		long_r.comment_list.push_back();
		*/

		basic_r.id = long_r.id - 1;
		basic_r.ingredient_list.clear();
		basic_r.instruction_list.clear();
		basic_r.comment_list.clear();

		no_ing_r.id = basic_r.id - 1;
		no_ing_r.ingredient_list.clear();

		no_ins_r.id = no_ing_r.id - 1;
		no_ins_r.instruction_list.clear();

		no_cmnt_r.id = no_ins_r.id - 1;
		no_cmnt_r.comment_list.clear();
	}
	
	virtual ~RecipeTest() { }
	
	Recipe r;
	Recipe no_id_r;
	Recipe long_r;
	Recipe basic_r;
	Recipe empty_r;
	Recipe no_ing_r;
	Recipe no_ins_r;
	Recipe no_cmnt_r;
};// class RecipeTest


TEST_F(RecipeTest,WriteRecipeToSqlite) {
	sqlite::database db{"recipe.db"};
	db << r;
	EXPECT_NE(r.id,Recipe::no_id);

	db << no_id_r;
	EXPECT_NE(no_id_r.id,Recipe::no_id);
	
	db << long_r;
	EXPECT_NE(long_r.id,Recipe::no_id);

	db << basic_r;
	EXPECT_NE(basic_r.id,Recipe::no_id);
	
	db << empty_r;
	EXPECT_NE(empty_r.id,Recipe::no_id);
	
	db << no_ing_r;
	EXPECT_NE(no_ing_r.id,Recipe::no_id);
	
	db << no_ins_r;
	EXPECT_NE(no_ins_r.id,Recipe::no_id);
	
	db << no_cmnt_r;
	EXPECT_NE(no_ins_r.id,Recipe::no_id);
}

}// namespace

int main(int argc, char** argv) {
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS;
}
