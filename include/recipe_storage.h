#ifndef GUARD_COOKBOX_SERVER_RECIPE_STORAGE_H
#define GUARD_COOKBOX_SERVER_RECIPE_STORAGE_H

#include <string>
#include <memory>

#include <sqlite_orm/sqlite_orm.h>

#include <recipe.h>

namespace storage {

struct RecipeDatabaseHelper {

	using id_type = recipe::Recipe::id_type;
	using string_type = std::string;

	static constexpr id_type no_id = recipe::Recipe::no_id;

	// SQL table/column names
	const static string_type recipe_tbl;
	const static string_type id_col;
	const static string_type name_col;
	const static string_type short_desc_col;
	const static string_type long_desc_col;
	const static string_type tgt_qty_col;
	const static string_type tgt_desc_col;
	const static string_type prep_time_col;
	const static string_type src_col;

	const static string_type ing_tbl;
	const static string_type ing_id_col;
	const static string_type ing_qty_col;
	const static string_type ing_desc_col;
	const static string_type ing_other_col;

	const static string_type ins_tbl;
	const static string_type ins_id_col;
	const static string_type ins_ins_col;
	const static string_type ins_pos_col;

	const static string_type cmnt_tbl;
	const static string_type cmnt_id_col;
	const static string_type cmnt_cmnt_col;

	const static string_type tag_list_tbl;
	const static string_type tag_list_tag_id;
	const static string_type tag_list_tag;

	const static string_type tag_tbl;
	const static string_type tag_tag_id;
	const static string_type tag_recipe_id;
};

// Classes mapped to storage
struct Db_recipe {
	Db_recipe() {};
	Db_recipe(const recipe::Recipe r)
		: id {r.id}
		, name {r.name}
		, short_description {r.short_description}
		, long_description {r.long_description}
		, target_quantity {r.target_quantity}
		, preparation_time {r.preparation_time}
		, source {r.source} {}

	RecipeDatabaseHelper::id_type id {RecipeDatabaseHelper::no_id};
	std::string name;
	std::string short_description;
	std::string long_description;
	double target_quantity {1};
	std::string target_description;
	double preparation_time {1};// in hours
	std::string source;
	
	recipe::Recipe toRecipe() const;
};

struct Db_ingredient {
	RecipeDatabaseHelper::id_type recipe_id;
	double quantity {1};
	std::string description;
	std::shared_ptr<RecipeDatabaseHelper::id_type> other_recipe {nullptr};

	recipe::Recipe::Ingredient toIngredient() const;
};

struct Db_instruction {
	RecipeDatabaseHelper::id_type recipe_id;
	std::vector<Db_instruction>::size_type position;
	std::string instruction;
};

struct Db_comment {
	RecipeDatabaseHelper::id_type recipe_id;
	std::string comment;
};
// A tag
struct Db_tag_list {
	RecipeDatabaseHelper::id_type tag_id;
	std::string tag;
};
// An item in a list of a recipe's tags
struct Db_tag {
	RecipeDatabaseHelper::id_type tag_id;
	RecipeDatabaseHelper::id_type recipe_id;
};

// Returns a storage object
// has to be inline for auto to be deduced
inline auto get_storage(const std::string& path_to_db) {
	using namespace sqlite_orm;
	using rdh = RecipeDatabaseHelper;

	return make_storage(path_to_db,
		    make_table(rdh::recipe_tbl,
			       make_column(rdh::id_col,
					   &Db_recipe::id,
					   primary_key()),
			       make_column(rdh::name_col,
					   &Db_recipe::name,
					   default_value("")),
			       make_column(rdh::short_desc_col,
					   &Db_recipe::short_description,
					   default_value("")),
			       make_column(rdh::long_desc_col,
					   &Db_recipe::long_description,
					   default_value("")),
			       make_column(rdh::tgt_qty_col,
					   &Db_recipe::target_quantity,
					   default_value(1)),
			       make_column(rdh::tgt_desc_col,
					   &Db_recipe::target_description,
					   default_value("")),
			       make_column(rdh::prep_time_col,
					   &Db_recipe::preparation_time,
					   default_value(0)),
			       make_column(rdh::src_col,
					   &Db_recipe::source,
					   default_value(""))
			       ),
		    make_table(rdh::ing_tbl,
			       make_column(rdh::ing_id_col,
					   &Db_ingredient::recipe_id),
			       make_column(rdh::ing_qty_col,
					   &Db_ingredient::quantity,
					   default_value(1)),
			       make_column(rdh::ing_desc_col,
					   &Db_ingredient::description,
					   default_value("")),
			       make_column(rdh::ing_other_col,
					   &Db_ingredient::other_recipe),
			       foreign_key(&Db_ingredient::recipe_id)
						.references(&Db_recipe::id),
			       foreign_key(&Db_ingredient::other_recipe)
						.references(&Db_recipe::id)
			       ),
		    make_table(rdh::ins_tbl,
			       make_column(rdh::ins_id_col,
					   &Db_instruction::recipe_id),
			       make_column(rdh::ins_pos_col,
					   &Db_instruction::position),
			       make_column(rdh::ins_ins_col,
					   &Db_instruction::instruction,
					   default_value("")),
			       foreign_key(&Db_instruction::recipe_id)
						.references(&Db_recipe::id)
			       ),
		    make_table(rdh::cmnt_tbl,
			       make_column(rdh::cmnt_id_col,
					   &Db_comment::recipe_id),
			       make_column(rdh::cmnt_cmnt_col,
					   &Db_comment::comment,
					   default_value("")),
			       foreign_key(&Db_comment::recipe_id)
						.references(&Db_recipe::id)
			       ),
		    make_table(rdh::tag_list_tbl,
			       make_column(rdh::tag_list_tag_id,
					   &Db_tag_list::tag_id,
					   primary_key()),
			       make_column(rdh::tag_list_tag,
					   &Db_tag_list::tag,
					   unique())
			       ),
		    make_table(rdh::tag_tbl,
			       make_column(rdh::tag_tag_id,
					   &Db_tag::tag_id),
			       make_column(rdh::tag_recipe_id,
					   &Db_tag::recipe_id),
			       foreign_key(&Db_tag::tag_id)
						.references(&Db_tag_list::tag_id),
			       foreign_key(&Db_tag::recipe_id)	
						.references(&Db_recipe::id)
			      )
	);
}// get_storage(.)

class RecipeDatabase {
public:
	using storage_type = decltype(get_storage(""));
	using id_type = RecipeDatabaseHelper::id_type;
	using Recipe = recipe::Recipe;

	RecipeDatabase(const std::string& path_to_db);
	
	std::shared_ptr<storage_type> get_database();

	// CRUD interface
	Recipe get(id_type id);
	id_type put(const Recipe& recipe);
	void remove(id_type id);

private:
	id_type insert(Recipe recipe);
	void update(const Recipe& recipe);

	std::shared_ptr<storage_type> _database;
};// class RecipeDatabase

}//namespace storage

#endif// ifndef GUARD_COOKBOX_SERVER_RECIPE_H
