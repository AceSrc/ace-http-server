build-toy: token.l parser.y
	flex -o ./toy/token.cpp ./toy/token.l
	bison -d -o ./toy/parser.cpp ./toy/parser.y
compile-toy: build-toy
	g++ ./toy/token.cpp ./toy/parser.cpp ./toy/main.cpp ./toy/fastcgi.cpp -std=c++17

run: compile
	./a.out

