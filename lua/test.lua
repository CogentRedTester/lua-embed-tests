local api = require "api"


local function foo()
    print("Intitialising")
    if api.test then api.test(1, 2, 3) end
end

foo()

api.register_parser("test", function(file)
    print("received "..file)
end)