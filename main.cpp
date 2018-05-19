/* 
 * Created by Alexander Sorochynskyi
 * On 2/05/2018
 */

#include <iostream>
#include <memory>

#include <recipe.h>
#include <recipe_server.h>

#include <cpprest/http_client.h>
#include <cpprest/uri_builder.h>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

int main(int argc, char* argv[]) {
    experimental::listener::http_listener listener {U("http://localhost/recipe")};
    listener.support(methods::GET, [](http_request r) {
    std::cout << "REQUEST!";
    r.reply(status_codes::OK);
});
    
    listener.open()
    .then([]() {
        std::cout << "LISTENING!" << std::endl;
    }).wait();

    while(true) {};

    return 0;
}
/*
	const std::string addr = "http://localhost";


    server::RecipeHandler server(addr,"");

	server.start().then([addr]() {
		std::cout << "Cookbox server started\n"
			"listtening at: " << addr << std::endl;
	});

	while(true) {}

	return 0;
	*/
