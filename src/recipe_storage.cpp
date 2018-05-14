/*
 * Created by Alexander Sorochynskyi
 * on 6/04/18
 *
 */

#include <recipe_storage.h>

namespace storage {

const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::recipe_tbl {"recipe"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::id_col {"id"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::name_col {"name"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::short_desc_col {"short_description"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::long_desc_col {"long_description"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tgt_qty_col {"target_quantity"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tgt_desc_col {"target_description"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::prep_time_col {"preparation_time"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::src_col {"source"};

const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ing_tbl {"ingredient_list"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ing_id_col {"recipe_id"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ing_qty_col {"quantity"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ing_desc_col {"description"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ing_other_col {"other_recipe"};

const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ins_tbl {"instruction_list"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ins_id_col {"recipe_id"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ins_ins_col {"position"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::ins_pos_col {"instruction"};

const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::cmnt_tbl {"comment_list"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::cmnt_id_col {"recipe_id"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::cmnt_cmnt_col {"comment"};

const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tag_list_tbl {"tag"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tag_list_tag_id {"id"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tag_list_tag {"tag"};

const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tag_tbl {"tag_list"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tag_tag_id {"tag_id"};
const RecipeDatabaseHelper::string_type RecipeDatabaseHelper::tag_recipe_id {"recipe_id"};


};// namespace storage

recipe::Recipe storage::Db_recipe::toRecipe() const
{
	recipe::Recipe r{};
	r.id = id;
	r.name = name;
	r.short_description = short_description;
	r.long_description = long_description;
	r.target_quantity = target_quantity;
	r.target_description = target_description;
	r.preparation_time = preparation_time;
	r.source = source;
	return r;
}

recipe::Recipe::Ingredient storage::Db_ingredient::toIngredient() const
{
	return recipe::Recipe::Ingredient {
		quantity,
		description,
		other_recipe
	};
}

constexpr storage::RecipeDatabaseHelper::id_type storage::RecipeDatabaseHelper::no_id;

storage::RecipeDatabase::RecipeDatabase(const std::string& path_to_db)
	: _database {std::make_shared<storage_type>(storage::get_storage(path_to_db))}
{
	get_database()->sync_schema();
}

std::shared_ptr<typename storage::RecipeDatabase::storage_type>
	storage::RecipeDatabase::get_database()
{
	return _database;
}

recipe::Recipe storage::RecipeDatabase::get(RecipeDatabase::id_type id) try
{
	using namespace sqlite_orm;

	Recipe ret {get_database()->get<Db_recipe>(id).toRecipe()};
	auto ingredients = get_database()->get_all<Db_ingredient>(
				where(c(&Db_ingredient::recipe_id) == id)
	);
	for (const auto& i : ingredients) {
		ret.ingredient_list.push_back(i.toIngredient());
	}
	auto instructions = get_database()->get_all<Db_instruction>(
				where(c(&Db_instruction::recipe_id) == id),
				order_by(&Db_instruction::position)
	);
	for (const auto& i : instructions) {
		ret.instruction_list.push_back(i.instruction);
	}
	auto comments = get_database()->get_all<Db_comment>(
				where(c(&Db_comment::recipe_id) == id)
	);
	for (const auto& c : comments) {
		ret.comment_list.push_back(c.comment);
	}
	return ret;

}
catch (std::system_error& e) {
	throw Not_found_error {e};
}
catch (std::runtime_error& e) {
	throw Database_error {e};
}

typename storage::RecipeDatabase::id_type
	storage::RecipeDatabase::put(const recipe::Recipe& recipe) try
{
	using namespace sqlite_orm;

	auto transaction_guard {get_database()->transaction_guard()};
	
	int ret;
	if (get_database()->
		count<Db_recipe>(where(c(&Db_recipe::id) == recipe.id)) == 0
	   )
	{
		ret = insert(recipe);	
	}
	else {
		update(recipe);
		ret = recipe.id;
	}

	transaction_guard.commit();// rollback() is called if an exception is thrown
	return ret;
} catch (std::runtime_error& e) {
	throw Database_error {e};
}

typename storage::RecipeDatabase::id_type
	storage::RecipeDatabase::insert(recipe::Recipe recipe)
{
	const Recipe::id_type id {get_database()->insert(Db_recipe {recipe})};


	recipe.id = id;
	update(recipe);

	return id;
}

void storage::RecipeDatabase::update(const recipe::Recipe& recipe)
{
	using namespace sqlite_orm;
	
	get_database()->update(Db_recipe {recipe});

	get_database()->remove_all<Db_ingredient>(
		where(c(&Db_ingredient::recipe_id) == recipe.id)
	);
	for (const auto& i : recipe.ingredient_list) {
		Db_ingredient ing {
			recipe.id,
			i.quantity,
			i.description,
			i.other_recipe
		};

		get_database()->insert(ing);
	}

	get_database()->remove_all<Db_instruction>(
		where(c(&Db_instruction::recipe_id) == recipe.id)
	);
	for (std::vector<std::string>::size_type i {0};
		i < recipe.instruction_list.size();
		++i)
	{
		get_database()->insert(
			Db_instruction {recipe.id,i,recipe.instruction_list[i]}
		);
	}

	get_database()->remove_all<Db_comment>(
		where(c(&Db_comment::recipe_id) == recipe.id)
	);
	for (const auto& cmnt : recipe.comment_list) {
		get_database()->insert(
			Db_comment {recipe.id, cmnt}
		);
	}
}

void storage::RecipeDatabase::remove(id_type id) try
{
	using namespace sqlite_orm;

	auto transaction_guard {get_database()->transaction_guard()};

	get_database()->remove_all<Db_ingredient>(
		where(c(&Db_ingredient::recipe_id) == id)
	);
	get_database()->remove_all<Db_instruction>(
		where(c(&Db_instruction::recipe_id) == id)
	);
	get_database()->remove_all<Db_comment>(
		where(c(&Db_comment::recipe_id) == id)
	);
	get_database()->remove_all<Db_recipe>(
		where(c(&Db_recipe::id) == id)
	);

	transaction_guard.commit();// rollback() is called in destructor if an exception is thrown
} catch (std::runtime_error& e) {
	throw Database_error {e};
}

/*
std::vector<typename storage::RecipeDatabase::id_type> storage::RecipeDatabase::ids()
try {
	using namespace sqlite_orm;
	
	std::vector<id_type> ret {};
	for (auto& r : get_database()->iterate<Db_recipe>()) {
		ret.push_back(r.id);
	}	

	return ret;
}
catch (std::runtime_error& e) {
	throw Database_error {e};
}
*/



