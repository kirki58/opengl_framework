main: src/main.cpp
	g++ -Iincludes $$(pkg-config --cflags glfw3) -o main src/main.cpp includes/glad/glad.c kirkiGL/build/*.o $$(pkg-config --static --libs glfw3)