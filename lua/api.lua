local api = _G.lua_api
package.loaded["api"] = api


local parsers = {}
function api.register_parser(name, fn)
    parsers[name] = fn
    api.register_parser_raw(name)
end

function api.run_parser(name, file)
    if not parsers[name] then error("parser does not exist") end
    parsers[name](file)
end
