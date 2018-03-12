/* 
	based on pistache example.  
*/

#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>

using namespace std;
using namespace Pistache;

class RecipeEndpoint {
public:
    RecipeEndpoint(Address addr, size_t n_thr = 2)
        : _httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    {
        auto opts = Http::Endpoint::options()
            .threads(n_thr)
            .flags(Tcp::Options::InstallSignalHandler);
        _httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        _httpEndpoint->setHandler(router.handler());
        _httpEndpoint->serve();
    }

    void shutdown() {
        _httpEndpoint->shutdown();
    }

private:
    void setupRoutes() {
        using namespace Rest;
	
/*
	// Insert new recipe -> retuns new id
        Routes::Post(_router, "/recipe", Routes::bind(&StatsEndpoint::doRecordMetric, this));
	// Get recipe
        Routes::Get(_router, "/recipe/:id", Routes::bind(&StatsEndpoint::doRecordMetric, this));
	// Get all Recipe ids
        Routes::Get(_router, "/recipe", Routes::bind(&StatsEndpoint::doRecordMetric, this));
	// Get a list of Recipes that got updated since a given time stamp
        Routes::Get(_router, "/recipe/sync", Routes::bind(&StatsEndpoint::doRecordMetric, this));
	// Update an existing Recipe, id is included in json
        Routes::Put(_router, "/recipe", Routes::bind(&StatsEndpoint::doRecordMetric, this));
	// Delete an existing Reipe
        Routes::Delete(_router, "/recipe/:id", Routes::bind(&StatsEndpoint::doRecordMetric, this));
	

        Routes::Post(router, "/record/:name/:value?", Routes::bind(&StatsEndpoint::doRecordMetric, this));
        Routes::Get(router, "/value/:name", Routes::bind(&StatsEndpoint::doGetMetric, this));
        Routes::Get(router, "/ready", Routes::bind(&Generic::handleReady));
        Routes::Get(router, "/auth", Routes::bind(&StatsEndpoint::doAuth, this));

*/
    }
    
    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;

    std::shared_ptr<Http::Endpoint> _httpEndpoint;
    Rest::Router _router;
};

int main(int argc, char *argv[]) {
    Port port(9080);

    int thr = 2;

    if (argc >= 2) {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr);

    stats.init(thr);
    stats.start();

    stats.shutdown();
}
