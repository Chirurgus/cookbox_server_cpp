#include <memory>

#include "recipe.h"

namespace recipe {

const Recipe::string_type Recipe::recipe_tbl {"recipe"};
const Recipe::string_type Recipe::id_col {"id"};
const Recipe::string_type Recipe::name_col {"name"};
const Recipe::string_type Recipe::short_desc_col {"short_description"};
const Recipe::string_type Recipe::long_desc_col {"long_description"};
const Recipe::string_type Recipe::tgt_qty_col {"target_quantity"};
const Recipe::string_type Recipe::tgt_desc_col {"target_description"};
const Recipe::string_type Recipe::prep_time_col {"preparation_time"};
const Recipe::string_type Recipe::src_col {"source"};

const Recipe::string_type Recipe::ing_tbl {"ingredient_list"};
const Recipe::string_type Recipe::ing_id_col {"recipe_id"};
const Recipe::string_type Recipe::ing_qty_col {"quantity"};
const Recipe::string_type Recipe::ing_desc_col {"description"};
const Recipe::string_type Recipe::ing_other_col {"other_recipe"};

const Recipe::string_type Recipe::ins_tbl {"instruction_list"};
const Recipe::string_type Recipe::ins_id_col {"recipe_id"};
const Recipe::string_type Recipe::ins_ins_col {"position"};
const Recipe::string_type Recipe::ins_pos_col {"instruction"};

const Recipe::string_type Recipe::cmnt_tbl {"comment_list"};
const Recipe::string_type Recipe::cmnt_id_col {"recipe_id"};
const Recipe::string_type Recipe::cmnt_cmnt_col {"comment"};

const Recipe::string_type Recipe::tag_list_tbl {"tag"};
const Recipe::string_type Recipe::tag_list_tag_id {"id"};
const Recipe::string_type Recipe::tag_list_tag {"tag"};

const Recipe::string_type Recipe::tag_tbl {"tag_list"};
const Recipe::string_type Recipe::tag_tag_id {"tag_id"};
const Recipe::string_type Recipe::tag_recipe_id {"recipe_id"};

/*
Recipe::Recipe(const json& j)
{
	id = j[0];
	name = j[1];
	short_description = j[2];
	long_description = j[3];
	target_quantity = j[4];
	target_description = j[5];
	preparation_time = j[6];
	source = j[7];
	ingridient_list = j["ingridient_list"];
	instruction_list = j["instruction_list"];
	comment_list = j["comment_list"];
	//ingredients
	for (const auto& i : j[8]) {
		Ingredient ing {i[0],i[1],i[2]};
		ingredient_list.push_back(ing);
	}	
	//instructions
	for (const auto& i : j[9]) {
		std::string ins {i};
		instruction_list.push_back(ins);
	}
	//comments
	for (const auto& c : j[10]) {
		std::string cmnt {c};
		comment_list.push_back(c);
	}
}

json Recipe::toJson() const
{
	json j;
	j["id"] = id;
	j["name"] = name;
	j["short_description"] = short_description;
	j["long_description"] = long_description;
	j["target_quantity"] = target_quantity;
	j["target_description"] = target_description;
	j["preparation_time"] = preparation_time;
	j["source"] = source;
	j["ingredient_list"] = ingredient_list
	j["instruction_list"] =	instruction_list;
	j["comment_list"] = comment_list;
	return j;
}
*/

sqlite::database_binder operator<<(sqlite::database db, Recipe& r)
{
	sqlite::database_binder ret {db << "begin;"};
	try {

	if (r.id != Recipe::no_id) { 
		db << "insert or replace into "
			+ Recipe::recipe_tbl + " ("
			+ Recipe::id_col + ","
			+ Recipe::name_col + ","
			+ Recipe::short_desc_col + ","
			+ Recipe::long_desc_col + ","
			+ Recipe::tgt_qty_col + ","
			+ Recipe::tgt_desc_col + ","
			+ Recipe::prep_time_col + ","
			+ Recipe::src_col + ") "
			+ "values(?,?,?,?,?,?,?,?);"
		   << r.id << r.name << r.short_description << r.long_description
		   << r.target_quantity << r.target_description
		   << r.preparation_time << r.source;
	}
	else {
		db << std::string {"insert into "}
			+ Recipe::recipe_tbl + " ("
			+ Recipe::name_col + ","
			+ Recipe::short_desc_col + ","
			+ Recipe::long_desc_col + ","
			+ Recipe::tgt_qty_col + ","
			+ Recipe::tgt_desc_col + ","
			+ Recipe::prep_time_col + ","
			+ Recipe::src_col + ") "
			+ "values(?,?,?,?,?,?,?);"
		   << r.name << r.short_description << r.long_description
		   << r.target_quantity << r.target_description
		   << r.preparation_time << r.source;
		r.id = db.last_insert_rowid();
	}
	
	db << "delete from ingredient_list where id = ?;"
	   << r.id;
	
	for (auto i : r.ingredient_list) {
		db << "insert into ingredient_list values(?,?,?,?);"
		   << r.id
		   << i.quantity
		   << i.description
		   << (i.other_recipe == Recipe::no_id ?
			nullptr : std::unique_ptr<Recipe::id_type>
					 {new Recipe::id_type {i.other_recipe}}
			);
	}

	db << "delete from instruction_list where id = ?;"
	   << r.id;
	for (std::vector<Recipe>::size_type i {0}; i < r.instruction_list.size(); ++i) {
		db << "insert into instruction_list values(?,?,?)"
		   << r.id
		   << i
		   << r.instruction_list[i];
	}

	db << "delete from comment_list where id = ?;"
	   << r.id;
	for (auto i : r.comment_list) {
		db << "insert into comment_list values(?,?)"
		   << r.id
		   << i;
	}	

	db << "commit;";
	} 
	catch (sqlite::sqlite_exception& e) {
		//log it somewhere
		db << "rollback;";
	}
	return ret;
}

}// namespace recipe
