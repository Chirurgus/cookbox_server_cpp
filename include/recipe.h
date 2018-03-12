#ifndef GUARD_COOKBOX_SERVER_RECIPE_H
#define GUARD_COOKBOX_SERVER_RECIPE_H

#include <exception>

#include "../sqlite_modern_cpp/hdr/sqlite_modern_cpp.h"
#include "../json/single_include/nlohmann/json.hpp"

namespace recipe {

using json = nlohmann::json;

struct Recipe {
	using id_type = long long;
	using string_type = std::string;
	static constexpr id_type no_id {-1ll};

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

	struct Ingredient {
		double quantity {1};
		std::string description;
		Recipe::id_type other_recipe {no_id};
	};
	
	// Exception class
	class Bad_recipe : public std::runtime_error{
		// inherit constructors
		using std::runtime_error::runtime_error;
	};


	Recipe() {}
/*
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
	
	std::vector<Ingredient> ingredient_list;	
	std::vector<std::string> instruction_list;
	std::vector<std::string> comment_list;
};// struct Recipe

// inserts, or updates a recipe in a given db
sqlite::database_binder operator<<(sqlite::database db, Recipe r);

}// namespace Recipe

#endif// ifndef GUARD_COOKBOX_SERVER_RECIPE_H
