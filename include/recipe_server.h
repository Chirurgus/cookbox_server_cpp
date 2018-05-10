/* 
 * Created by Alexander Sorochynskyi
 * On 2/05/2018
 */

#ifndef GUARD_COOKBOX_SERVER_RECIPE_SERVER_H
#define GUARD_COOKBOX_SERVER_RECIPE_SERVER_H

// STD lib includes
#include <algorithm>
#include <memory>
#include <string>

// Ohter lib includes
#include <pplx/pplxtasks.h>
#include <cpprest/http_listener.h>

// My includes
#include <recipe.h>

namespace server {

class RecipeHandler {
public:
	using id_type = recipe::Recipe::id_type;


	RecipeHandler(const std::string& addr,
		     const std::string& db_location);

	pplx::task<void> start();
	pplx::task<void> shutdown();

	std::string databaseLocation() const;

private:
	using http_listener = web::http::experimental::listener::http_listener;
	using http_request = web::http::http_request;

	void routeGet(http_request request);
	void routePut(http_request request);
	void routePost(http_request request);
	void routeDelete(http_request request);

	// Insert new recipe -> retuns new id
	void insert(http_request request); 
	// Get recipe
	void get(http_request request, const id_type id );
	// Get all Recipe ids
	void get_ids(http_request request);
	/*
	// Get a list of Recipes that got updated since a given time stamp
	void sync(http_request request);
	*/
	// Update an existing Recipe, id is included in json
	void update(http_request request, const id_type id);
	// Delete an existing Reipe
	void remove(http_request request, const id_type id);

	http_listener _listener;
	std::string _db_location;
};

}// namespace server

#endif// ifndef  GUARD_COOKBOX_SERVER_RECIPE_SERVER_H
