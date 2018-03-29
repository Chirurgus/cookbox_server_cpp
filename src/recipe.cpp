#include <memory>

#include <recipe.h>

namespace recipe {

// (re)definition needed to avoid link errors
constexpr Recipe::id_type Recipe::no_id;

}// namespace recipe

/*
recipe::Recipe::Recipe(const json& j)
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

json recipe::Recipe::toJson() const
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

