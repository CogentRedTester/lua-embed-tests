#include <lua5.2/lua.hpp>
#include <iostream>
#include <string>
#include <filesystem>

#include "api.hpp"

std::unordered_map<std::string,Script> SCRIPTS;

int main() {
    // load all files in the ./scripts/ directory
    for (const auto &file : std::filesystem::directory_iterator("./scripts/")) {
        auto path = file.path();
        if (path.extension() != ".lua") continue;

        std::string id = path.filename();
        auto parser = Script(id);
        SCRIPTS[id] = std::move(parser);

        SCRIPTS[id].initialise(path);
    }

    std::cout << std::endl;
    for (auto &&[id, script] : SCRIPTS) {
        for (auto &&parser : script.parser_functions) {

            // pass each parser each file in the directory
            for (auto &&file : std::filesystem::recursive_directory_iterator("./examples")) {
                if (file.is_directory()) continue;
                script.run_parser(parser, file.path());
            }
        }
    }
    
}