/*
 * Created on 8/5/18
 * by Alexander Sorochynskyi
 */

#include <memory>


#include <iostream>

#include "test_common.h"

#include <recipe.h>
#include <recipe_storage.h>

using Recipe = ::recipe::Recipe;
using RecipeDatabase = ::storage::RecipeDatabase;

class RecipeStorageTest : public RecipeTestBase {
public:
	std::string db_location {""};//in-memory database
	std::string db_v4_location {"test_recipe_v4.db"};
};// class RecipeStorageTest

TEST_F(RecipeStorageTest,Open) {
	RecipeDatabase db {db_location};
}

TEST_F(RecipeStorageTest,OpenAndSyncSchema) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
}

TEST_F(RecipeStorageTest,CheckSchemaCompatibleWithv4) {
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
	
TEST_F(RecipeStorageTest,WriteRecipe) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	r.id = db.put(r);
	EXPECT_NE(r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,WriteNoId) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_id_r {r};
	no_id_r.id = Recipe::no_id;

	no_id_r.id = db.put(no_id_r);
	EXPECT_NE(no_id_r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,WriteLong) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe long_r {r};

	long_r.id = db.put(long_r);
	EXPECT_NE(long_r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,WriteBasic) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe basic_r {r};

	basic_r.ingredient_list.clear();
	basic_r.instruction_list.clear();
	basic_r.comment_list.clear();

	basic_r.id = db.put(basic_r);
	EXPECT_NE(basic_r.id,Recipe::no_id);
}
	
TEST_F(RecipeStorageTest,WriteEmpty) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe empty_r {};
	empty_r.id = db.put(empty_r);

	EXPECT_NE(empty_r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,WriteNoIngredients) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_ing_r {r};
	no_ing_r.ingredient_list.clear();

	no_ing_r.id = db.put(no_ing_r);
	EXPECT_NE(no_ing_r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,WriteNoInstructions) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_ins_r {r};
	no_ins_r.instruction_list.clear();

	no_ins_r.id = db.put(no_ins_r);
	EXPECT_NE(no_ins_r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,WriteNoComments) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_cmnt_r {r};
	no_cmnt_r.comment_list.clear();

	no_cmnt_r.id = db.put(no_cmnt_r);
	EXPECT_NE(no_cmnt_r.id,Recipe::no_id);
}

TEST_F(RecipeStorageTest,RecipeCompare) {
	ASSERT_TRUE(recipe_compare(r,r));
}	

TEST_F(RecipeStorageTest,ReadRecipe) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	auto id = db.put(r);

	ASSERT_NE(id,Recipe::no_id);
	
	Recipe r1 {db.get(r.id)};
	
	ASSERT_TRUE(recipe_compare(r1,r));
}
	
TEST_F(RecipeStorageTest,ReadNoId) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_id_r {r};
	no_id_r.id = Recipe::no_id;

	no_id_r.id = db.put(no_id_r);
	EXPECT_NE(no_id_r.id,Recipe::no_id);
	
	Recipe r1 {db.get(no_id_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_id_r));
}

TEST_F(RecipeStorageTest,ReadLong) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe long_r {r};

	long_r.id = db.put(long_r);
	EXPECT_NE(long_r.id,Recipe::no_id);
	
	Recipe r1 {db.get(long_r.id)};
	ASSERT_TRUE(recipe_compare(r1, long_r));
}

TEST_F(RecipeStorageTest,ReadBasic) {
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
	
TEST_F(RecipeStorageTest,ReadEmpty) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe empty_r {};
	empty_r.id = db.put(empty_r);

	EXPECT_NE(empty_r.id,Recipe::no_id);

	Recipe r1 {db.get(empty_r.id)};
	ASSERT_TRUE(recipe_compare(r1, empty_r));
}

TEST_F(RecipeStorageTest,ReadNoIngredients) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe no_ing_r {r};
	no_ing_r.ingredient_list.clear();

	no_ing_r.id = db.put(no_ing_r);
	EXPECT_NE(no_ing_r.id,Recipe::no_id);

	Recipe r1 {db.get(no_ing_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_ing_r));
}

TEST_F(RecipeStorageTest,ReadNoInstructions) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_ins_r {r};
	no_ins_r.instruction_list.clear();

	no_ins_r.id = db.put(no_ins_r);
	EXPECT_NE(no_ins_r.id,Recipe::no_id);

	Recipe r1 {db.get(no_ins_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_ins_r));
}

TEST_F(RecipeStorageTest,ReadNoComments) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();

	Recipe no_cmnt_r {r};
	no_cmnt_r.comment_list.clear();

	no_cmnt_r.id = db.put(no_cmnt_r);
	EXPECT_NE(no_cmnt_r.id,Recipe::no_id);

	Recipe r1 {db.get(no_cmnt_r.id)};
	ASSERT_TRUE(recipe_compare(r1, no_cmnt_r));
}

TEST_F(RecipeStorageTest,InsertJustRecipe) {
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	storage::Db_recipe r1 {r};	
	r1.id = Recipe::no_id;

	r1.id = db.get_database()->insert(r1);
	ASSERT_NE(r1.id, Recipe::no_id) << "Couldn't insert recipe";
	
	auto r2 {db.get_database()->get<storage::Db_recipe>(r1.id)};
	ASSERT_TRUE(recipe_compare(r1.toRecipe(),r2.toRecipe()));	
}

TEST_F(RecipeStorageTest,InsertIngredient) {
	using namespace sqlite_orm;
	RecipeDatabase db {db_location};
	db.get_database()->sync_schema();
	
	Recipe r1 {r};

	r1.ingredient_list.clear();
	r1.id = db.put(r);

	storage::Db_ingredient i1 {
		r1.id,
		10,
		"hello",
		nullptr
	};

	db.get_database()->insert(i1);
	auto i_list {db.get_database()->get_all<storage::Db_ingredient>(
			where(c(&storage::Db_ingredient::recipe_id) == r1.id)
	)};

	ASSERT_EQ(i_list.size(),1);
	auto i2 {i_list.front()};
	
	
	EXPECT_EQ(i1.recipe_id,i2.recipe_id);
	EXPECT_EQ(i1.quantity,i2.quantity);
	EXPECT_EQ(i1.description,i2.description);
	EXPECT_EQ(i1.other_recipe,i2.other_recipe);
}

TEST_F(RecipeStorageTest,InsertInstruction) {
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

TEST_F(RecipeStorageTest,ForeignKeyConstraintIngredient) {
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

TEST_F(RecipeStorageTest,ForeignKeyConstraintIngredientOther) {
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


TEST_F(RecipeStorageTest,ForeignKeyConstraintInstruction) {
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
