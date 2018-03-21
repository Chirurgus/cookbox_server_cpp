#ifndef GUARD_COOKBOX_SERVER_RECIPE_H
#define GUARD_COOKBOX_SERVER_RECIPE_H

#include <exception>
#include <memory>

#include <sqlite_orm/sqlite_orm.h>
#include <nlohmann/json.hpp>

namespace recipe {

using json = nlohmann::json;

struct Recipe {
	using id_type = int;
	using string_type = std::string;

	static constexpr id_type no_id {-1};

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
	
	struct Basic_ingredient {
		double quantity {1};
		std::string description;
		std::shared_ptr<Recipe::id_type> other_recipe {nullptr};
	};

	struct Ingredient : Basic_ingredient {
		Recipe::id_type recipe_id;
	};

	struct Instruction {
		Recipe::id_type recipe_id;
		int position;
		std::string instruction;
	};
	
	struct Comment {
		Recipe::id_type recipe_id;
		std::string comment;
	};
	// A tag
	struct Tag_list {
		Recipe::id_type tag_id;
		std::string tag;
	};
	// An item in a list of a recipe's tags
	struct Tag {
		Recipe::id_type tag_id;
		Recipe::id_type recipe_id;
	};
	
	// Exception class
	class Bad_recipe : public std::runtime_error{
		// inherit constructors
		using std::runtime_error::runtime_error;
	};


/*
	Recipe() {}
	Recipe(const json& j);

	json toJson() const;
*/

	id_type id {no_id};
	std::string name;
	std::string short_description;
	std::string long_description;
	double target_quantity {1};
	std::string target_description;
	double preparation_time {1};// in hours
	std::string source;
	
	std::vector<Basic_ingredient> ingredient_list;	
	std::vector<std::string> instruction_list;
	std::vector<std::string> comment_list;
};// struct Recipe

namespace storage {

	// Returns a storage object
	// has to be inline for auto to be deduced
	inline auto get_storage(const std::string& path_to_db) {
		using namespace sqlite_orm;
		return make_storage(path_to_db,
			    make_table(Recipe::recipe_tbl,
				       make_column(Recipe::id_col,
						   &Recipe::id,
						   primary_key()),
				       make_column(Recipe::name_col,
						   &Recipe::name,
						   default_value("")),
				       make_column(Recipe::short_desc_col,
						   &Recipe::short_description,
						   default_value("")),
				       make_column(Recipe::long_desc_col,
						   &Recipe::long_description,
						   default_value("")),
				       make_column(Recipe::tgt_qty_col,
						   &Recipe::target_quantity,
						   default_value(1)),
				       make_column(Recipe::tgt_desc_col,
						   &Recipe::target_description,
						   default_value("")),
				       make_column(Recipe::prep_time_col,
						   &Recipe::preparation_time,
						   default_value(0)),
				       make_column(Recipe::src_col,
						   &Recipe::source,
						   default_value(""))
				       ),
			    make_table(Recipe::ing_tbl,
				       make_column(Recipe::ing_id_col,
						   &Recipe::Ingredient::recipe_id),
				       foreign_key(&Recipe::Ingredient::recipe_id)
							.references(&Recipe::id),
				       make_column(Recipe::ing_qty_col,
						   &Recipe::Ingredient::quantity,
						   default_value(1)),
				       make_column(Recipe::ing_desc_col,
						   &Recipe::Ingredient::description,
						   default_value("")),
				       make_column(Recipe::ing_other_col,
						   &Recipe::Ingredient::other_recipe),
				       foreign_key(&Recipe::Ingredient::recipe_id)
							.references(&Recipe::id)
				       ),
			    make_table(Recipe::ins_tbl,
				       make_column(Recipe::ins_id_col,
						   &Recipe::Instruction::recipe_id),
				       foreign_key(&Recipe::Instruction::recipe_id)
							.references(&Recipe::id),
				       make_column(Recipe::ins_pos_col,
						   &Recipe::Instruction::position),
				       make_column(Recipe::ins_ins_col,
						   &Recipe::Instruction::instruction,
						   default_value(""))
				       ),
			    make_table(Recipe::cmnt_tbl,
				       make_column(Recipe::cmnt_id_col,
						   &Recipe::Comment::recipe_id),
				       foreign_key(&Recipe::Comment::recipe_id)
							.references(&Recipe::id),
				       make_column(Recipe::cmnt_cmnt_col,
						   &Recipe::Comment::comment,
						   default_value(""))
				       ),
			    make_table(Recipe::tag_list_tbl,
				       make_column(Recipe::tag_list_tag_id,
						   &Recipe::Tag_list::tag_id,
						   primary_key()),
				       make_column(Recipe::tag_list_tag,
						   &Recipe::Tag_list::tag,
						   unique())
				       ),
			    make_table(Recipe::tag_tbl,
				       make_column(Recipe::tag_tag_id,
						   &Recipe::Tag::tag_id),
				       foreign_key(&Recipe::Tag::tag_id)
							.references(&Recipe::Tag_list::tag_id),
				       make_column(Recipe::tag_recipe_id,
						   &Recipe::Tag::recipe_id),
				       foreign_key(&Recipe::Tag::recipe_id)	
							.references(&Recipe::Recipe::id)
				      )
		);
	}

}//namespace storage

class RecipeDatabase {
public:
	using storage_type = decltype(storage::get_storage(""));
	using id_type = Recipe::id_type;

	RecipeDatabase(const std::string& path_to_db);
	
	std::shared_ptr<storage_type> get_database();

	Recipe get(id_type id);
	id_type put(const Recipe& recip);
	void remove(id_type id);

private:
	id_type insert(Recipe recipe);
	void update(const Recipe& recipe);

	std::shared_ptr<storage_type> _database;

};// class RecipeDatabase

}// namespace recipe

#endif// ifndef GUARD_COOKBOX_SERVER_RECIPE_H
