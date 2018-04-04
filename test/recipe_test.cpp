#include <memory>

#include <gtest/gtest.h>

#include <iostream>

#include <recipe.h>
#include <recipe_storage.h>

namespace {

using Recipe = ::recipe::Recipe;
using RecipeDatabase = ::storage::RecipeDatabase;

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
	{
	}
	
	virtual ~RecipeTest() { }
	
	Recipe r ;

	std::string db_location {""};//in-memory database
	std::string db_v4_location {"test_recipe_v4.db"};

private:
	Recipe::id_type default_id {Recipe::no_id};
};// class RecipeTest

bool recipe_compare(const Recipe& r1, const Recipe& r2)
{
	bool ret { r1.id == r2.id
	        && r1.name == r2.name 
	        && r1.short_description == r2.short_description 
	        && r1.long_description == r2.long_description 
	        && r1.target_quantity == r2.target_quantity 
	        && r1.preparation_time == r2.preparation_time 
	        && r1.source == r2.source
		&& r1.ingredient_list.size() == r2.ingredient_list.size()
		&& r1.instruction_list.size() == r2.instruction_list.size()
		&& r1.comment_list.size() == r2.comment_list.size() };

	if (!ret) {
		return false;
	}

	for (std::vector<Recipe::Ingredient>::size_type i {0};
		i < r1.ingredient_list.size();
		++i)
	{
		if (r1.ingredient_list[i].quantity
			         != r2.ingredient_list[i].quantity
			|| r1.ingredient_list[i].description
				 != r2.ingredient_list[i].description)
		{
			return false;
		}
		else {
			// other_recipes should both be either null or not null
			// if they they are not null, they should be equal
			if (static_cast<bool>(r1.ingredient_list[i].other_recipe)
				!= static_cast<bool>(r2.ingredient_list[i].other_recipe)
			    || (r1.ingredient_list[i].other_recipe
				&& *r1.ingredient_list[i].other_recipe
				    != *r2.ingredient_list[i].other_recipe))
			{
				return false;
			}
		}
	}
	for (std::vector<std::string>::size_type i {0};
		i < r1.instruction_list.size();
		++i)
	{
		if (r1.instruction_list[i]
			         != r2.instruction_list[i])
		{
			return false;
		}
	}

	for (std::vector<std::string>::size_type i {0};
		i < r1.comment_list.size();
		++i)
	{
		if (r1.comment_list[i]
			         != r2.comment_list[i])
		{
			return false;
		}
	}
	return true;
}


TEST_F(RecipeTest,OpenSqlite) {
	RecipeDatabase db {db_location};
}

TEST_F(RecipeTest,OpenSqliteAndSyncSchema) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
}

TEST_F(RecipeTest,CheckSchemaCompatibleWithSqlitev4) {
	RecipeDatabase db {db_v4_location};
	auto changes {db.get_database()->sync_schema_simulate()};

	using namespace sqlite_orm;
	using Rdh = storage::RecipeDatabaseHelper;

	EXPECT_EQ(changes[Rdh::recipe_tbl],sync_schema_result::already_in_sync);
	EXPECT_EQ(changes[Rdh::ing_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Rdh::ins_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Rdh::cmnt_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Rdh::tag_tbl],sync_schema_result::already_in_sync);	
	EXPECT_EQ(changes[Rdh::tag_list_tbl],sync_schema_result::already_in_sync);	
}
	
TEST_F(RecipeTest,WriteRecipeSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	r.id = db.put(r);
	EXPECT_NE(r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoIdSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_id_r {r};
	no_id_r.id = Recipe::no_id;

	no_id_r.id = db.put(no_id_r);
	EXPECT_NE(no_id_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteLongSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe long_r {r};

	long_r.id = db.put(long_r);
	EXPECT_NE(long_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteBasicSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe basic_r {r};

	basic_r.ingredient_list.clear();
	basic_r.instruction_list.clear();
	basic_r.comment_list.clear();

	basic_r.id = db.put(basic_r);
	EXPECT_NE(basic_r.id,Recipe::no_id);
}
	
TEST_F(RecipeTest,WriteEmptySqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe empty_r {};
	empty_r.id = db.put(empty_r);

	EXPECT_NE(empty_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoIngredientsSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_ing_r {r};
	no_ing_r.ingredient_list.clear();

	no_ing_r.id = db.put(no_ing_r);
	EXPECT_NE(no_ing_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoInstructionsSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_ins_r {r};
	no_ins_r.instruction_list.clear();

	no_ins_r.id = db.put(no_ins_r);
	EXPECT_NE(no_ins_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,WriteNoCommentsSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_cmnt_r {r};
	no_cmnt_r.comment_list.clear();

	no_cmnt_r.id = db.put(no_cmnt_r);
	EXPECT_NE(no_cmnt_r.id,Recipe::no_id);
}

TEST_F(RecipeTest,RecipeCompare) {
	ASSERT_TRUE(recipe_compare(r,r));
}	

TEST_F(RecipeTest,ReadRecipeSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	r.id = db.put(r);
	ASSERT_NE(r.id,Recipe::no_id);
	
	Recipe r1 {db.get(r.id)};
	
	ASSERT_TRUE(recipe_compare(r1,r));
}
	
TEST_F(RecipeTest,ReadNoIdSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_id_r {r};
	no_id_r.id = Recipe::no_id;

	no_id_r.id = db.put(no_id_r);
	EXPECT_NE(no_id_r.id,Recipe::no_id);
	
	Recipe r1 {db.get(no_id_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_id_r));
}

TEST_F(RecipeTest,ReadLongSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe long_r {r};

	long_r.id = db.put(long_r);
	EXPECT_NE(long_r.id,Recipe::no_id);
	
	Recipe r1 {db.get(long_r.id)};
	ASSERT_TRUE(recipe_compare(r1, long_r));
}

TEST_F(RecipeTest,ReadBasicSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe basic_r {r};
	basic_r.ingredient_list.clear();
	basic_r.instruction_list.clear();
	basic_r.comment_list.clear();

	basic_r.id = db.put(basic_r);
	EXPECT_NE(basic_r.id,Recipe::no_id);

	Recipe r1 {db.get(basic_r.id)};
	ASSERT_TRUE(recipe_compare(r1, basic_r));
}
	
TEST_F(RecipeTest,ReadEmptySqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe empty_r {};
	empty_r.id = db.put(empty_r);

	EXPECT_NE(empty_r.id,Recipe::no_id);

	Recipe r1 {db.get(empty_r.id)};
	ASSERT_TRUE(recipe_compare(r1, empty_r));
}

TEST_F(RecipeTest,ReadNoIngredientsSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_ing_r {r};
	no_ing_r.ingredient_list.clear();

	no_ing_r.id = db.put(no_ing_r);
	EXPECT_NE(no_ing_r.id,Recipe::no_id);

	Recipe r1 {db.get(no_ing_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_ing_r));
}

TEST_F(RecipeTest,ReadNoInstructionsSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_ins_r {r};
	no_ins_r.instruction_list.clear();

	no_ins_r.id = db.put(no_ins_r);
	EXPECT_NE(no_ins_r.id,Recipe::no_id);

	Recipe r1 {db.get(no_ins_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_ins_r));
}

TEST_F(RecipeTest,ReadNoCommentsSqlite) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_cmnt_r {r};
	no_cmnt_r.comment_list.clear();

	no_cmnt_r.id = db.put(no_cmnt_r);
	EXPECT_NE(no_cmnt_r.id,Recipe::no_id);

	Recipe r1 {db.get(no_cmnt_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_cmnt_r));
}

TEST_F(RecipeTest,InsertJustRecipe) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	storage::Db_recipe r1 {r};	
	r1.id = Recipe::no_id;

	r1.id = db.get_database()->insert(r1);
	ASSERT_NE(r1.id, Recipe::no_id) << "Couldn't insert recipe";
	
	auto r2 {db.get_database()->get<storage::Db_recipe>(r1.id)};
	ASSERT_TRUE(recipe_compare(r1.toRecipe(),r2.toRecipe()));	
}

TEST_F(RecipeTest,InsertIngredient) {
	using namespace sqlite_orm;
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	r.ingredient_list.clear();
	r.id = db.put(r);

	storage::Db_ingredient i1 {
		r.id,
		10,
		"hello",
		nullptr
	};

	db.get_database()->insert(i1);
	auto i_list {db.get_database()->get_all<storage::Db_ingredient>(
			where(c(&storage::Db_ingredient::recipe_id) == r.id)
	)};

	ASSERT_EQ(i_list.size(),1);
	auto i2 {i_list.front()};
	
	
	EXPECT_EQ(i1.recipe_id,i2.recipe_id);
	EXPECT_EQ(i1.quantity,i2.quantity);
	EXPECT_EQ(i1.description,i2.description);
	EXPECT_EQ(i1.other_recipe,i2.other_recipe);
}

TEST_F(RecipeTest,InsertInstruction) {
	using namespace sqlite_orm;	
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	r.instruction_list.clear();
	r.id = db.put(r);

	storage::Db_instruction i1 {r.id, 1, "hello"};

	db.get_database()->insert(i1);
	auto i_list {db.get_database()->get_all<storage::Db_instruction>(
			where(c(&storage::Db_instruction::recipe_id) == r.id)
	)};

	ASSERT_EQ(i_list.size(),1);
	auto i2 {i_list.front()};
	
	EXPECT_EQ(i1.recipe_id,i2.recipe_id);
	EXPECT_EQ(i1.position,i2.position);
	EXPECT_EQ(i1.instruction,i2.instruction);
}

TEST_F(RecipeTest,ForeignKeyConstraintIngredient) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	r.id = db.put(r);

	storage::Db_ingredient ing{
		r.id,
		100,
		"g of foreign keys",
		nullptr
	};

	// should work
	db.get_database()->insert(ing);
	
	bool constraint_triggered {false};
	try {
		ing.recipe_id = r.id + 1;
		//should trigger foreign key constraint
		db.get_database()->insert(ing);
	}
	catch (...) {
		constraint_triggered = true;
	}
	 
	ASSERT_TRUE(constraint_triggered);
}

TEST_F(RecipeTest,ForeignKeyConstraintIngredientOther) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe other_r {r};

	r.id = db.put(r);
	other_r.id = db.put(other_r);

	storage::Db_ingredient ing {
		r.id,
		100,
		"g of foreign keys",
		std::make_shared<Recipe::id_type>(other_r.id)
	};

	// should work
	db.get_database()->insert(ing);
	
	bool constraint_triggered {false};
	try {
		ing.other_recipe = std::make_shared<int>(other_r.id + 1);
		//should trigger foreign key constraint
		db.get_database()->insert(ing);
	}
	catch (...) {
		constraint_triggered = true;
	}
	 
	ASSERT_TRUE(constraint_triggered);
}


TEST_F(RecipeTest,ForeignKeyConstraintInstruction) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	r.id = db.put(r);

	storage::Db_instruction ins {r.id, 100,  "g of foreign keys"};

	// should work
	db.get_database()->insert(ins);
	
	bool constraint_triggered {false};
	try {
		ins.recipe_id = r.id + 1;

		//should trigger foreign key constraint
		db.get_database()->insert(ins);
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

