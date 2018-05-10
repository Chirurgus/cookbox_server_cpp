/*
 * Created on 8/5/18
 * by Alexander Sorochynskyi
 */

#ifndef GUARD_COOKBOX_TEST_TEST_COMMON_H
#define GUARD_COOKBOX_TEST_TEST_COMMON_H

#include <gtest/gtest.h>

#include <recipe.h> 

struct RecipeTestBase : public ::testing::Test{
	RecipeTestBase()
		: r {recipe::Recipe::no_id,
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
		{}
	virtual ~RecipeTestBase() {}
	const recipe::Recipe r;
};

bool recipe_compare(const recipe::Recipe& r1, const recipe::Recipe& r2)
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

#endif // ifndef GUARD_COOKBOX_TEST_TEST_COMMON_H
