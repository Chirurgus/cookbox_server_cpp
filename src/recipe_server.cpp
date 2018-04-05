
#include <recipe_server.h>


server::RecipeServer::RecipeServer(Address addr, size_t n_thr = 2)
        : _httpEndpoint(std::make_shared<Http::Endpoint>(addr))
{
	auto opts = Http::Endpoint::options()
	    .threads(n_thr)
	    .flags(Tcp::Options::InstallSignalHandler);
	_httpEndpoint->init(opts);
	setupRoutes();
}

void server::RecipeServer::start()
{
	_httpEndpoint->setHandler(_router.handler());
	_httpEndpoint->serve();
}

void server::RecipeServer::shutdown()
{
	_httpEndpoint->shutdown();
}

void server::RecipeServer::setupRoutes()
{
	using namespace Rest;

	// Insert new recipe -> retuns new id
	Routes::Post(_router, "/recipe", Routes::bind());
	// Get recipe
	Routes::Get(_router, "/recipe/:id", Routes::bind());
	// Get all Recipe ids
	Routes::Get(_router, "/recipe", Routes::bind());
	// Get a list of Recipes that got updated since a given time stamp
	Routes::Get(_router, "/recipe/sync", Routes::bind());
	// Update an existing Recipe, id is included in json
	Routes::Put(_router, "/recipe", Routes::bind());
	// Delete an existing Reipe
	Routes::Delete(_router, "/recipe/:id", Routes::bind());
}
    
