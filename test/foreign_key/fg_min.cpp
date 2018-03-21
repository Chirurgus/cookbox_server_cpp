#include <string>
#include <sqlite_orm/sqlite_orm.h>

struct Student {
	int id;
	std::string name;
};

struct Marks {
	//int mark_id;
	int student_id;
	int mark;
};

using namespace sqlite_orm;

auto storage = make_storage(":memory:",
                                make_table("mark",
                                           make_column("id",
                                                       &Student::id,
                                                       primary_key()),
                                           make_column("name",
                                                       &Student::name)),
                                make_table("track",
                                           /*make_column("id",
                                                       &Marks::mark_id,
                                                       primary_key()),*/
                                           make_column("mark",
                                                       &Marks::mark),
                                           make_column("student_id",
                                                       &Marks::student_id),
                                           foreign_key(&Marks::student_id)
						.references(&Student::id))
				);


int main() {
	storage.sync_schema();
	return 0;
}
