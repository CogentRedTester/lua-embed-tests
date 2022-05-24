local api = require "api"


local function foo()
    print("Intitialising")
    if api.test then api.test(1) end
end

foo()

api.register_parser("script2", function(file)
    print("received", file)
end)