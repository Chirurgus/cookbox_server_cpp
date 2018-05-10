#include <memory>

#include <recipe.h>

namespace recipe {

// (re)definition needed to avoid link errors
constexpr Recipe::id_type Recipe::no_id;

}// namespace recipe

// helper functions
namespace {
	std::string wstr_to_str(const std::wstring& wstr) {
		return std::string(wstr.begin(), wstr.end());
	}
}
recipe::Recipe::Recipe(const json& j)
{
	id = j.at(U("id")).as_integer();
	name = wstr_to_str(j.at(U("name")).as_string());
	short_description = wstr_to_str(j.at(U("short_description")).as_string());
	long_description = wstr_to_str(j.at(U("long_description")).as_string());
	target_quantity = j.at(U("target_quantity")).as_double();
	target_description = wstr_to_str(j.at(U("target_description")).as_string());
	preparation_time = j.at(U("preparation_time")).as_double();
	source = wstr_to_str(j.at(U("source")).as_string());
	for (const auto& i : j.at(U("ingredient_list")).as_array()) {
		Ingredient ing {i.at(U("quantity")).as_double(),
						wstr_to_str(i.at(U("description")).as_string()),
						nullptr};
		if (i.has_field(U("other_recipe"))) {
			ing.other_recipe = std::make_shared<Recipe::id_type>(
										i.at(U("other_recipe")).as_integer()
			);
		}
		ingredient_list.push_back(ing);
	}
	for (const auto& i : j.at(U("instruction_lsit")).as_array()) {
		instruction_list.push_back(wstr_to_str(i.as_string()));
	}
	for (const auto& c : j.at(U("comment_list")).as_array()) {
		comment_list.push_back(wstr_to_str(c.as_string()));
	}
}

// This assumes that recipes contain only 1 byte chars
recipe::Recipe::json recipe::Recipe::toJson() const
{
	json j;
	j[U("id")] = json::number(id);
	j[U("name")] = json::string(std::wstring{name.begin(),name.end()});
	j[U("short_description")] = json::string(std::wstring{short_description.begin(), short_description.end()});
	j[U("long_description")] = json::string(std::wstring{long_description.begin(), long_description.end()});
	j[U("target_quantity")] = json::number(target_quantity);
	j[U("target_description")] = json::string(std::wstring{target_description.begin(), target_description.end()});
	j[U("preparation_time")] = json::number(preparation_time);
	j[U("source")] = json::string(std::wstring{source.begin(),source.end()});
	j[U("ingredient_list")] = json::array();
	for (std::vector<Ingredient>::size_type i{0}; i < ingredient_list.size(); ++i) {
		json j_ing;
		j_ing[U("quantity")] = json::number(ingredient_list[i].quantity);
		j_ing[U("description")] = json::string(std::wstring{ingredient_list[i].description.begin(), ingredient_list[i].description.end()});
		if (ingredient_list[i].other_recipe) {
			j_ing[U("other_recipe")] = json::number(*ingredient_list[i].other_recipe);
		}
		j[U("ingredient_list")][i] = j_ing;
	}
	for (std::vector<std::string>::size_type i{ 0 }; i < instruction_list.size(); ++i) {
		j[U("instruction_list")][i] = json::string(std::wstring{instruction_list[i].begin(),instruction_list[i].end()});
	}
	for (std::vector<std::string>::size_type i{ 0 }; i < comment_list.size(); ++i) {
		j[U("comment_list")][i] = json::string(std::wstring{comment_list[i].begin(),comment_list[i].end()});
	}
	return j;
}
