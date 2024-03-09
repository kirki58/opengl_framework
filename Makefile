main: src/main.cpp main-debug
	@echo "Creating main executable"
	@g++ -Iincludes $$(pkg-config --cflags glfw3 assimp) -o main src/main.cpp includes/glad/glad.c kirkiGL/build/*.o $$(pkg-config --static --libs glfw3 assimp)

main-debug: src/main.cpp Mesh.o
	@echo "Creating main debug executable"
	@g++ -Iincludes $$(pkg-config --cflags glfw3 assimp) -o main-debug src/main.cpp includes/glad/glad.c kirkiGL/build/*.o -g $$(pkg-config --static --libs glfw3 assimp)

Mesh.o : kirkiGL/src/Mesh.cpp Model.o
	@echo "Creating object file 4/4"
	@g++ -Iincludes $$(pkg-config --cflags assimp) -c kirkiGL/src/Mesh.cpp -o kirkiGL/build/Mesh.o -g

Model.o : kirkiGL/src/Model.cpp Object.o
	@echo "Creating object file 3/4"
	@g++ -Iincludes $$(pkg-config --cflags assimp) -c kirkiGL/src/Model.cpp -o kirkiGL/build/Model.o -g

Object.o : kirkiGL/src/Object.cpp Shader.o
	@echo "Creating object file 2/4"
	@g++ -Iincludes $$(pkg-config --cflags assimp) -c kirkiGL/src/Object.cpp -o kirkiGL/build/Object.o -g

Shader.o : kirkiGL/src/Shader.cpp
	@echo "Creating object file 1/4"
	@g++ -Iincludes $$(pkg-config --cflags assimp) -c kirkiGL/src/Shader.cpp -o kirkiGL/build/Shader.o -g