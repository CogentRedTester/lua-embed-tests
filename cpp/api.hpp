#pragma once
#ifndef _API_HPP
#define _API_HPP

#include <lua5.2/lua.hpp>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <memory>

// this class much be stored in a pointer to avoid closing the lua_state early
class Parser {
    // std::unique_ptr<lua_State> L;
    lua_State * L;
    // std::shared_ptr<lua_State> L;

    public:
    std::string id;
    std::unordered_set<std::string> parser_functions;

    Parser();
    ~Parser();

    void initialise(std::string file);                 // [-0,+0,e]
    void setup_api();
    void run_parser(std::string parse_name, std::string path);
};

typedef std::unique_ptr<Parser> Parser_Ptr;

extern std::unordered_map<std::string,Parser_Ptr> PARSERS;

int error_handler(lua_State *L);
bool test_error(lua_State* state, int err);

#endif //_API_HPP