all: compile

build:
	flex -o token.cpp token.l
	bison -d -o parser.cpp parser.y
compile: build
	g++ token.cpp parser.cpp main.cpp fastcgi.cpp -std=c++17

