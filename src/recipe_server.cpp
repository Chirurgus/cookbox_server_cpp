/*
 * Writen by Alexander Sorochynskyi
 * 6/04/18
 *
 */

#include <utility>
#include <exception>
#include <chrono>
#include <functional>

#include <cpprest/json.h>
#include <cpprest/http_msg.h>

#include <recipe.h>
#include <recipe_storage.h>

#include <recipe_server.h>

using namespace web;//Stroustrup forgive me

// helper functions
namespace {

	// by Sean D. Cline from http_listener_test.cpp gist
	void respond(const http::http_request& request,
				 const http::status_code& status,
				 const json::value& response) {
		json::value resp;
		resp[U("status")] = json::value::number(status);
		resp[U("response")] = response;

		// Pack in the current time for debugging purposes.
		/*
		time_t now = time(0);
		utility::stringstream_t ss;
		ss << put_time(localtime(&now), L"%Y-%m-%dT%H:%S:%MZ");
		resp[U("server_time")] = json::value::string(ss.str());
		*/

		request.reply(status, resp);
	}

	void respond_badrequest(const http::http_request& request) {
			respond(request, http::status_codes::BadRequest, json::value("Invalid uri"));
	};
	
	void respond_internalerror(const http::http_request& request, std::string what = "Internal error") {
		respond(request, http::status_codes::InternalError, json::value(what.c_str()));
	}
}

server::RecipeHandler::RecipeHandler(const std::string& addr,
									 const std::string& db_location)
	: _listener{uri{utility::conversions::to_string_t(addr)}}
	, _db_location {db_location}
{
	_listener.support(http::methods::POST, std::bind(&RecipeHandler::routePost,this,std::placeholders::_1));
	_listener.support(http::methods::GET, std::bind(&RecipeHandler::routeGet,this,std::placeholders::_1));
	_listener.support(http::methods::PUT, std::bind(&RecipeHandler::routePut,this,std::placeholders::_1));
	_listener.support(http::methods::DEL, std::bind(&RecipeHandler::routeDelete,this,std::placeholders::_1));
}

pplx::task<void> server::RecipeHandler::start()
{
	return _listener.open();
}

pplx::task<void> server::RecipeHandler::shutdown()
{
	return _listener.close();
}

std::string server::RecipeHandler::databaseLocation() const
{
	return _db_location;
}

void server::RecipeHandler::routeGet(server::RecipeHandler::http_request request)
{
	auto path = uri::split_path(request.request_uri().path());

	if (path.size() == 2 && path[0] == U("recipe")) {
		try {
			auto id = std::stoll(path[1]);
			get(request, id);
			return;
		}
		catch (std::invalid_argument& err) {
			respond_badrequest(request);
		}
	}

	if (path.size() == 1 && path[0] == U("recipe")) {
		//get_ids(request);
		respond(request, http::status_codes::NotImplemented, json::value::string(U("not implemented")));
		return;
	}

	respond_badrequest(request);
}


void server::RecipeHandler::routePut(server::RecipeHandler::http_request request)
{
	auto path = uri::split_path(request.request_uri().path());

	if (path.size() == 2 && path[0] == U("recipe")) {
		try {
			auto id {std::stoll(path[1])};
			update(request, id);
			return;
		}
		catch (std::invalid_argument& err) {
			//
		}
	}
	respond_badrequest(request);
}

void server::RecipeHandler::routePost(http_request request)
{
	auto path = uri::split_path(request.request_uri().path());

	if (path.size() == 1 && path.front() == U("recipe")) {
			insert(request);
			return;
	}

	respond_badrequest(request);
}

void server::RecipeHandler::routeDelete(server::RecipeHandler::http_request request)
{
	auto path = uri::split_path(request.request_uri().path());

	if (path.size() == 2 && path.front() == U("recipe")) {
		try {
			auto id {std::stoll(path[1])};
			remove(request, id);
			return;
		}
		catch (std::invalid_argument& err) {
			//
		}
	}
	respond_badrequest(request);
}

// Insert new recipe -> retuns new id  "/recipe"
void server::RecipeHandler::insert(server::RecipeHandler::http_request request)
{
	using namespace recipe;
	using namespace storage;
	using namespace web::http;

	pplx::task<json::value> body{ request.extract_json() };
	body.then([=](pplx::task<json::value> t) {
		try {
			RecipeDatabase db{databaseLocation()};
			auto j_recipe{t.get()};
			Recipe recipe{j_recipe};
			auto id{db.put(recipe)};
			json::value j_id{};
			j_id[U("id")] = json::value::number(id);
			respond(request, status_codes::OK, j_id);
		}
		catch (RecipeDatabase::Database_error& e) {
			respond_internalerror(request);
		}
		catch (...) {
			respond_internalerror(request);
		}
	});
}

// Get recipe "/recipe/:id",
void server::RecipeHandler::get(server::RecipeHandler::http_request request,
								const id_type id)
{
	using namespace recipe;
	using namespace storage;
	using namespace web::http;
	
	try {
		RecipeDatabase db {databaseLocation()};
		web::json::value j_recipe {db.get(id).toJson()};
		respond(request, status_codes::OK, j_recipe);
	}
	catch (RecipeDatabase::Not_found_error& e) {
		respond_internalerror(request);
	}
	catch (RecipeDatabase::Database_error& e) {
		respond_internalerror(request);
	}
	catch (...) {
		respond_internalerror(request);
	}
}

/*
// Get all Recipe ids "/recipe",
void server::RecipeHandler::get_ids(server::RecipeHandler::http_request request)
{
	using namespace storage;
	using namespace web::http;

	RecipeDatabase db {databaseLocation()};
	db.get_database()->sync_storage();
	try {
		auto ids {db.ids()};
		json::value j_ids = json::value::array();
		for (std::vector<id_type>::size_type i {0}; i < ids.size(); ++i) {
			j_ids[i] = ids[i];
		}
		respond(request, status_codes::OK, j_ids);
	}
	catch (RecipeDatabase::Database_error& e) {
		respond_internalerror(request);
	}
	catch (...) {
		respond_internalerror(request);
	}
}
*/

/*
// Get a list of Recipes that got updated since a given time stamp "/recipe/sync",
void server::RecipeHandler::sync(http_request request)
{
}
*/

// Update an existing Recipe, id is included in json "/recipe"
void server::RecipeHandler::update(server::RecipeHandler::http_request request, const id_type id)
{
	using namespace recipe;
	using namespace storage;
	using namespace web::http;

	pplx::task<json::value> body{ request.extract_json() };
	body.then([=](pplx::task<web::json::value> t) {
		try {
			RecipeDatabase db {databaseLocation()};
			auto j_recipe {t.get()};
			Recipe recipe {j_recipe};
			db.put(recipe);
			respond(request, status_codes::OK, json::value {id});
		}
		catch (RecipeDatabase::Database_error& e) {
			respond_internalerror(request);
		}
		catch (...) {
			respond_internalerror(request);
		}
	});
}

// Delete an existing Reipe "/recipe/:id",
void server::RecipeHandler::remove(server::RecipeHandler::http_request request, const id_type id)
{
	using namespace recipe;
	using namespace storage;
	using namespace web::http;

	try {
		RecipeDatabase db {databaseLocation()};
		db.remove(id);
		respond(request, status_codes::OK, web::json::value{ id });
	}
	catch (RecipeDatabase::Not_found_error& e) {
		respond(request, status_codes::NotFound, json::value::string(U("No recipe with this id found.")));
	}
	catch (RecipeDatabase::Database_error& e) {
		respond_internalerror(request, "Couldn't access the databse.");
	}
	catch (...) {
		respond_internalerror(request);
	}
}


    