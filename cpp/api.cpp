#include "api.hpp"
#include <lua5.2/lua.hpp>
#include <iostream>
// API api;

int print_error(lua_State* state, int error) {
    switch (error)
    {
    case LUA_ERRSYNTAX:
        std::cerr << "LUA_ERRSYNTAX";
        break;
    case LUA_ERRMEM:
        std::cerr << "LUA_ERRMEM";
        break;
    case LUA_ERRRUN:
        std::cerr << "LUA_ERRRUN";
        break;
    default:
        std::cerr << "Error code: " << error;
        break;
    }

    // The error message is on top of the stack.
    // Fetch it, print it and then pop it off the stack.
    const char* message = lua_tostring(state, -1);
    std::cerr << '\t' << message << std::endl;
    lua_pop(state, 1);
    return 0;
}

// https://github.com/mpv-player/mpv/blob/master/player/lua.c
int error_handler(lua_State *L) {
    if (luaL_loadstring(L, "return debug.traceback('', 3)") == 0) { // e fn|err
        lua_call(L, 0, 1); // e backtrace
        const char *tr = lua_tostring(L, -1);
        std::cerr << tr << std::endl;
    }
    lua_pop(L, 1); // e

    return 1;
}

bool test_error(lua_State* state, int err) {
    if (err) print_error(state, err);
    return err;
}

// void lua_ocall(lua_State * L, int nargs, int nresults) {
    
//     int top = lua_gettop(L);
//     lua_pcall(L, nargs, nresults, top);
// }

Parser::Parser() {
    L = nullptr;
}

void Parser::initialise(std::string file) {
    id = file;
    L = luaL_newstate();
    if (L == NULL) {
        std::cerr << "Failed to create state" << std::endl;
        L = nullptr;
        return;
    }
    int err;

    // Make standard libraries available in the Lua object
    luaL_openlibs(L);
    setup_api();

    lua_pushcfunction(L, error_handler);

    // Load the program; this supports both source code and bytecode files.
    err = luaL_loadfile(L, file.c_str());
    if ( test_error(L, err) ) return;

    // Finally, execute the program by calling into it.
    // Change the arguments if you're not running vanilla Lua code.

    err = lua_pcall(L, 0, LUA_MULTRET, 1);
    if ( test_error(L, err) ) return;

    // pops the error handler function
    lua_pop(L, 1);
}

Parser::~Parser() {
    if (L == nullptr ) return;
    std::cout << "Closing Lua State" << std::endl;
    lua_close(L);
}

void Parser::run_parser(std::string parse_name, std::string path) {
    lua_pushcfunction(L, error_handler);    //fn
    int err_hndler = lua_gettop(L);

    lua_getglobal(L, "lua_api");            // fn table
    lua_getfield(L, -1, "run_parser");      // fn table fn
    lua_remove(L, -2);                      // fn fn
    lua_pushstring(L, parse_name.c_str());  // fn fn str
    lua_pushstring(L, path.c_str());        // fn fn str str
    lua_pcall(L, 2, 0, err_hndler);         // fn
    lua_pop(L, 1);                          //
}

static int test(lua_State * L) {
    int num_args = lua_gettop(L);
    std::cout << "function called with " << num_args << " arguments" << std::endl;
    return 0;
}

static int register_parser(lua_State * L) {
    std::string id = lua_tostring(L, lua_upvalueindex(1));

    luaL_checktype(L, 1, LUA_TSTRING);
    std::string fn_name = lua_tostring(L, 1);
    PARSERS[id].get()->parser_functions.insert(fn_name);

    return 0;
}

std::vector<luaL_Reg> api {
    {"test", test},
    {"register_parser_raw", register_parser},
    {"run_parser", NULL},
    {NULL, NULL}
};

void Parser::setup_api() {
    int err;

    // create new registry    
    // lua_newtable(L);        // {}
    // lua_newtable(L);        // {} {}
    // lua_setfield(L, -2, "parsers"); // { parsers = {} }
    // lua_setfield(L, LUA_REGISTRYINDEX, "_api");

    // create the api table
    lua_createtable(L, 0, api.size());
    lua_pushstring(L, id.c_str());
    luaL_setfuncs(L, api.data(), 1);
    lua_setglobal(L, "lua_api");

    err = luaL_dofile(L, "lua/api.lua");
    if ( test_error(L, err) ) return;
}