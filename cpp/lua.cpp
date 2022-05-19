#include <lua5.2/lua.hpp>
#include <iostream>
#include <string>
#include <filesystem>

#include "api.hpp"

std::unordered_map<std::string,Parser_Ptr> PARSERS;

int main(int argc, char** argv) {
    if ( argc <= 1 ) {
        std::cout << "Usage: run lua file(s)\n";
        std::cout << "Loads and executes Lua programs.\n";
        return 1;
    }

    // Execute all programs on the command line
    for ( int n = 1; n < argc; ++n ) {
        auto parser = std::make_unique<Parser>();
        std::string id = argv[n];
        PARSERS[id] = std::move(parser);

        PARSERS[id]->initialise(argv[n]);
    }

    std::cout << std::endl;
    for (auto &&[name, parser] : PARSERS) {
        for (auto &&fn : parser.get()->parser_functions) {
            parser.get()->run_parser(fn, "examples/amadeus/monitor.yaml");
        }
        
    }
    
}