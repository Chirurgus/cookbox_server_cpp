#include <string>
#include <sqlite_orm/sqlite_orm.h>

	
struct Recipe {

struct Basic_ingredient {
	double quantity;
	std::string description;
	std::shared_ptr<int> other_recipe {nullptr};
};
struct Ingredient : public Basic_ingredient {
	int recipe_id;
};

struct Instruction {
	int recipe_id;
	int position;
	std::string instruction;
};

struct Comment {
	int recipe_id;
	std::string comment;
	};
// A tag
struct Tag_list {
	int tag_id;
	std::string tag;
};
// An item in a list of a recipe's tags
struct Tag {
	int tag_id;
	int recipe_id;
};

	int id {-1};
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

// Returns a storage object
// has to be inline for auto to be deduced
inline auto get_storage(const std::string& path_to_db) {
	using namespace sqlite_orm;
	return make_storage(path_to_db,
		    make_table("recipe",
			       make_column("id",
					   &Recipe::id,
					   primary_key()),
			       make_column("name",
					   &Recipe::name,
					   default_value("")),
			       make_column("short_description",
					   &Recipe::short_description,
					   default_value("")),
			       make_column("long_description",
					   &Recipe::long_description,
					   default_value("")),
			       make_column("target_quantity",
					   &Recipe::target_quantity,
					   default_value(1)),
			       make_column("target_description",
					   &Recipe::target_description,
					   default_value("")),
			       make_column("preparation_time",
					   &Recipe::preparation_time,
					   default_value(0)),
			       make_column("source",
					   &Recipe::source,
					   default_value(""))

			       ),
		    make_table("ingredient_list",
			       make_column("recipe_id",
					   &Recipe::Ingredient::recipe_id),
			       make_column("quantity",
					   &Recipe::Ingredient::quantity,
					   default_value(1)),
			       make_column("description",
					   &Recipe::Ingredient::description,
					   default_value("")),
			       make_column("other_recipe",
				       	   &Recipe::Ingredient::other_recipe),
			       foreign_key(&Recipe::Ingredient::recipe_id)
			       			.references(&Recipe::id),
			       foreign_key(&Recipe::Ingredient::other_recipe)
			       			.references(&Recipe::id)
			       ),
		    make_table("instruction_list",
			       make_column("recipe_id",
					   &Recipe::Instruction::recipe_id),
			       make_column("position",
					   &Recipe::Instruction::position),
			       make_column("instruction",
					   &Recipe::Instruction::instruction,
					   default_value("")),
			       foreign_key(&Recipe::Instruction::recipe_id)
			       			.references(&Recipe::id)
			       ),
		    make_table("comment_list",
			       make_column("recipe_id",
					   &Recipe::Comment::recipe_id),
			       make_column("comment",
					   &Recipe::Comment::comment,
					   default_value("")),
			       foreign_key(&Recipe::Comment::recipe_id)
						.references(&Recipe::id)
			       ),
		    make_table("tag",
			       make_column("id",
					   &Recipe::Tag_list::tag_id,
					   primary_key()),
			       make_column("tag",
					   &Recipe::Tag_list::tag,
					   unique())
			       ),
		    make_table("tag_list",
			       make_column("tag_id",
					   &Recipe::Tag::tag_id),
			       make_column("recipe_id",
					   &Recipe::Tag::recipe_id),
			       foreign_key(&Recipe::Tag::tag_id)
			       			.references(&Recipe::Tag_list::tag_id),
			       foreign_key(&Recipe::Tag::recipe_id)
			       			.references(&Recipe::Recipe::id)
			      )
	);
}

 auto storage = get_storage(":memory:");

int main() {
	storage.sync_schema();
	return 0;
}

