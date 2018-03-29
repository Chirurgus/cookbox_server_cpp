#ifndef GUARD_COOKBOX_SERVER_RECIPE_H
#define GUARD_COOKBOX_SERVER_RECIPE_H

#include <exception>

#include <nlohmann/json.hpp>

namespace recipe {

using json = nlohmann::json;

struct Recipe {
	using id_type = int;

	static constexpr id_type no_id {-1};
	
	struct Ingredient {
		double quantity {1};
		std::string description;
		std::shared_ptr<recipe::Recipe::id_type> other_recipe {nullptr};
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
	
	std::vector<Ingredient> ingredient_list;	
	std::vector<std::string> instruction_list;
	std::vector<std::string> comment_list;
};// struct Recipe

}// namespace recipe

#endif// ifndef GUARD_COOKBOX_SERVER_RECIPE_H
