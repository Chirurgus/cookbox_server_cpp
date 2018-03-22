#include <memory>

#include <recipe.h>
#include <gtest/gtest.h>

namespace {

using Recipe = ::recipe::Recipe;

class RecipeTest : public ::testing::Test {
protected:
	RecipeTest() 
		: r {default_id,
			"Test recipe",
			"An easy one, to test your program",
			"Every since the good ol' 18' that I'm using this recipe to test my recipe programs.",
			3.14f,
			"g. of skill to get this working.",
			2,
			"Yours one and only Alexander",
			{ {10, "Years C++ experience ;)", nullptr}, 
			  {3, "Years in university", nullptr},
			  {50, "ish hours of your time", nullptr},
			  {1000, "motivations to do this", nullptr},
			  {5, "annoyances with not having a way to stock recipes",nullptr}},
			{ "Be anoyed with the way you oganaized recipes",
			  "Lean Java + Android",
			  "Want same thing on your PC",
			  "Want to sync data" },
			{ "Dont' hesitate a a little cinamon (on your system)",
			  "Goes best with coffe and marshmellows" }
		   }
		, no_id_r {r}
		, long_r {r}
		, basic_r {r}
		, empty_r {}
		, no_ing_r {r}
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

	std::string db_location {""};//in-memory database
	std::string db_v4_location {"test_recipe_v4.db"};

private:
	Recipe::id_type default_id {std::numeric_limits<Recipe::id_type>::max()};
};// class RecipeTest


TEST_F(RecipeTest,OpenSqlite) {
	recipe::RecipeDatabase db {db_location};
}

TEST_F(RecipeTest,OpenSqliteAndSyncSchema) {
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
}

TEST_F(RecipeTest,CheckSchemaCompatibleWithDbv4) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_v4_location};
	auto changes {db.get_database()->sync_schema_simulate()};

	EXPECT_EQ(changes[Recipe::recipe_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Recipe::ing_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Recipe::ins_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Recipe::cmnt_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Recipe::tag_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Recipe::tag_list_tbl],sync_schema_result::already_in_sync);	
}
	
TEST_F(RecipeTest,WriteRToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(r);
	EXPECT_NE(r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoIdToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	no_id_r.id = db.put(no_id_r);
	EXPECT_NE(no_id_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteLongToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(long_r);
	EXPECT_NE(long_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteBasicToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(basic_r);
	EXPECT_NE(basic_r.id,Recipe::no_id);
}
	
TEST_F(RecipeTest,WriteEmptyToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	empty_r.id = db.put(empty_r);
	EXPECT_NE(empty_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoIngToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	db.put(no_ing_r);
	EXPECT_NE(no_ing_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoInsToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(no_ing_r);
	EXPECT_NE(no_ins_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoCmntToSqlite) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(no_cmnt_r);
	EXPECT_NE(no_ins_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,ForeignKeyConstraintIngredient) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(r);

	Recipe::Ingredient ing{};
	ing.recipe_id = r.id;
	ing.quantity = 100;
	ing.description = "g of foreign keys";
	ing.other_recipe = nullptr;

	// should work
	db.get_database()->insert(ing);
	
	bool constraint_triggered {false};
	try {
		ing.recipe_id = r.id - 1;
		//should trigger foreign key constraint
		db.get_database()->insert(ing);
	}
	catch (...) {
		constraint_triggered = true;
	}
	 
	ASSERT_TRUE(constraint_triggered);
}

TEST_F(RecipeTest,ForeignKeyConstraintIngredientOther) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(r);
	db.put(long_r);
	
	Recipe::Ingredient ing{};
	ing.recipe_id = r.id;
	ing.quantity = 100;
	ing.description = "g of foreign keys";
	ing.other_recipe = std::make_shared<int>(long_r.id);

	// should work
	db.get_database()->insert(ing);
	
	bool constraint_triggered {false};
	try {
		ing.other_recipe = std::make_shared<int>(long_r.id -1);
		//should trigger foreign key constraint
		db.get_database()->insert(ing);
	}
	catch (...) {
		constraint_triggered = true;
	}
	 
	ASSERT_TRUE(constraint_triggered);
}


TEST_F(RecipeTest,ForeignKeyConstraintInstruction) {
	using namespace sqlite_orm;
	recipe::RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	db.put(r);

	// should work
	db.get_database()->insert(Recipe::Instruction {r.id, 100,  "g of foreign keys"});
	
	bool constraint_triggered {false};
	try {
		//should trigger foreign key constraint
		db.get_database()->insert(Recipe::Instruction {r.id - 1, 100,  "g of foreign keys"});
	}
	catch (...) {
		constraint_triggered = true;
	}
	 
	ASSERT_TRUE(constraint_triggered);

}

}// namespace

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc,argv);
	return RUN_ALL_TESTS();
}
