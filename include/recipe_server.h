/* 
	based on pistache example.  
*/


#include <algorithm>
#include <memory>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

namespace server {

class RecipeServer {
public:
	RecipeServer(Address addr, size_t n_thr = 2);
    	void start();
	void shutdown();

private:
	void setupRoutes();



	std::shared_ptr<Http::Endpoint> _httpEndpoint;
	Rest::Router _router;
};

}// namespace server
