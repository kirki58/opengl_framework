main: main.cpp
	g++ -I../includes $$(pkg-config --cflags glfw3) -o main main.cpp ../includes/glad/glad.c $$(pkg-config --static --libs glfw3)