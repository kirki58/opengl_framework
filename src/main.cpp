#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <kirkiGL/Model.h>
#include <kirkiGL/Shader.h>

/*
We need to put every OpenGL function in CHECK(opengl_func) macro so we get a proper debug message from it.
MAKE SURE TO CHANGE THE MODE SPECIFIED IN THE gldebug.h TO "RELEASE" BEFORE PRODUCTION.
*/
#include <gldebug.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

Shader program("res/shaders/kirkiGL_defaults/object/default.vert", "res/shaders/kirkiGL_defaults/object/default.frag");
int main(void)
{

    glfwInit(); // Initialize GLFW library

    // WINDOW HINTS ALLOWS US TO CONFIGURE OUR WINDOW BEFORE ACTUALLY CREATING IT
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // WE'RE USING OPENGL 3.3 SO MAJOR VERSION SHOULD BE SET TO 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // MINOR VERSION SHOULD BE SET TO 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //WE DON'T WANT OUR APPLICATION TO BE BACKWARDS COMPATIBLE (3.3+) SO WE USE CORE PROFILE
    GLFWwindow* window;

    // Check initialize GLFW3
    if (!glfwInit()){
        std::cout << "Failed to Initialize GLFW" << std::endl;
        return -1;
    }

    //Create a window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Project", NULL, NULL);
    if (window == NULL){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Check initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    Model backpack("../res/models/backpack/backpack.obj", glm::vec3(0.0f, 0.0f, 0.0f), program);

    //Frame loop
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        CHECK(glClear(GL_COLOR_BUFFER_BIT));
        program.use();
        backpack.Draw(program);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}