all: Game.cpp
	g++ -I OpenGL\ Libraries/include/ -I ./ -L OpenGL\ Libraries/lib/ Game.cpp
