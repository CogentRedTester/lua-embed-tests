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

class Script {
    // std::unique_ptr<lua_State> L;
    lua_State * L;
    // std::shared_ptr<lua_State> L;

    public:
    std::string id;
    std::unordered_set<std::string> parser_functions;

    Script();
    Script(const std::string & id);
    ~Script();

    void initialise(const std::string & file);                 // [-0,+0,e]
    void setup_api();
    void run_parser(const std::string & parse_name, const std::string & path);
};

typedef std::unique_ptr<Script> Script_Ptr;

extern std::unordered_map<std::string,Script> SCRIPTS;

int error_handler(lua_State *L);
bool test_error(lua_State* state, int err);

#endif //_API_HPP