#include <string>
#include <sqlite_orm/sqlite_orm.h>

using namespace sqlite_orm;
	struct Instruction {
		int recipe_id;
		std::string instruction;
	};
	struct Ingredient {
		int recipe_id;
		std::string description;
		int other_recipe_id;
	};
	// A tag
	struct Tag_list {
		int tag_id;
		std::string tag;
	};
	struct Tag {
		int recipe_id;
		int tag_id;
	};

struct Recipe {
	int id;
	std::string name;
};

auto storage = make_storage(":memory:",
		    make_table("recipe",
			       make_column("id",
					   &Recipe::id,
					   primary_key()),
			       make_column("name",
					   &Recipe::name,
					   default_value(""))
			       ),
		    make_table("instruction_list",
			       make_column("id",
					   &Instruction::recipe_id),
			       foreign_key(&Instruction::recipe_id)
						.references(&Recipe::id),
			       make_column("instruction",
					   &Instruction::instruction,
					   default_value(""))
			       )/*,

		    make_table(Recipe::ing_tbl,
			       make_column(Recipe::ing_id_col,
					   &Recipe::Ingredient::recipe_id,
					   foreign_key(&Recipe::Ingredient::recipe_id)
						.references(&Recipe::id)),
			       make_column(Recipe::ing_desc_col,
					   &Recipe::Ingredient::description,
					   default_value("")),
			       make_column(Recipe::ing_other_col,
					   &Recipe::Ingredient::other_recipe,
					   foreign_key(&Recipe::Ingredient::recipe_id)
						.references(&Recipe::id))
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
					   &Recipe::Tag::tag_id,
					   foreign_key(&Recipe::Tag::tag_id)
						.references(&Recipe::Tag_list::tag_id)),
			       make_column(Recipe::tag_recipe_id,
					   &Recipe::Tag::recipe_id,
					   foreign_key(&Recipe::Tag::recipe_id)	
						.references(&Recipe::Recipe::id))
			      )
			*/
);

int main() {
	storage.sync_schema();
	return 0;
}

