
#include <sqlite_orm/sqlite_orm.h>

#include <string>
#include <iostream>
#include <cassert>

using namespace sqlite_orm;

using std::cout;
using std::endl;

struct RapArtist {
	    int id;
	        std::string name;
};

struct Track {
	std::string track;
	int artist_id;
};

int main(int argc, char **argv) {
	    
	    auto storage = make_storage(":memory:",
			    make_table("rap_artists",
				    make_column("id", &RapArtist::id, primary_key()),
				    make_column("name", &RapArtist::name)),
			    make_table("tracks",
				    make_column("track",&Track::track),
				    foreign_key(&Track::artist_id).references(&RapArtist::id)),
				    make_column("artist_id",&Track::artist_id)
				   );
	    cout << "in memory db opened" << endl;
	    storage.sync_schema();
	    assert(!storage.count<RapArtist>());
	    storage.insert(RapArtist{ -1, "The Weeknd" });
	    storage.transaction([&]{
			storage.insert(RapArtist{ -1, "Drake" });
			return true;
	    });
				    
	    cout << "rap artists count = " << storage.count<RapArtist>() << endl;
				        
	   //transaction also work in memory..
	   storage.transaction([&]{
		storage.insert(RapArtist{ -1, "Kanye West" });
			      return false;
		  });
	   cout << "rap artists count = " << storage.count<RapArtist>() << " (no Kanye)" << endl;
	  return 0;
}
