CC=clang++
CA=-Wall -Wextra -pedantic -std=c++17

all: cpp/lua.cpp cpp/api.cpp
	$(CC) $(CA) $^ -llua5.2

clean:
	rm *.out