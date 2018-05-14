/* 
 * Created by Alexander Sorochynskyi
 * On 2/05/2018
 */

#include <iostream>

#include <recipe_server.h>

int main(int argc, char *argv[]) try {

	const std::string addr = "http://localhost";


    server::RecipeHandler server(addr,"");

	server.start().then([addr]() {
		std::cout << "Cookbox server started\n"
			"listtening at: " << addr << std::endl;
	});

	while(true) {}

	return 0;
}
catch (std::runtime_error& e) {
	std::cout << e.what() << std::endl;
}
