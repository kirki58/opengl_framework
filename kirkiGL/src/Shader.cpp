#include <kirkiGL/Shader.h>

// ReadGLSL reads the content of GLSL files (.vert and .frag etc. for kirkiGL) into memort buffer and returns it.
char* Shader::ReadGLSL(const char* path){
    // ReadGLSL function reads files in a C-Style way instead of using C++ fstream
    FILE* file = fopen(path, "r");
    if(file == NULL){
        std::cout << "[ERROR] Couldn't open GLSL file!" << std::endl;
        return nullptr;
    }

    //Get file size for later use
    fseek(file, 0L, SEEK_END);
    unsigned int len = (unsigned int) ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* string = (char*) malloc(sizeof(char) * len); // [WARNING!] DON'T FORGET TO FREE THIS HEAP ALLOCATION!
    int i = 0;
    char c;

    while( (c = fgetc(file)) != EOF ){
        string[i] = c;
        i++;
    }
    string[i] = '\0';

    return string;
}

// CompileShader function compiles and returns the ID of the compiled shader program which can be used later for linking multiple shaders.
// Takes in parameters as the source code (returned by ReadGLSL) and the shader type (GL_VERTEX_SHADER ,GL_FRAGMENT_SHADER)
unsigned int Shader::CompileShader(const char* shader_source, GLenum type){

    unsigned int shader = glCreateShader(type);
    CHECK(glShaderSource(shader, 1, &shader_source, NULL));
    CHECK(glCompileShader(shader));

    int success;
    int len;
    CHECK(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    CHECK(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len));

    char* infoLog = (char*)alloca(sizeof(char) * len); // "alloca" function allocates memory in stack so no need to free it.

    if(!success){
        CHECK(glGetShaderInfoLog(shader, len, &len, infoLog));
        std::cout << "ERROR: SHADER COMPILATION ERROR FOR " << (type == GL_VERTEX_SHADER ? "VERTEX SHADER\n":"FRAGMENT SHADER\n") <<  infoLog << std::endl;
    }
    return shader;
}

// LinkShaders function Links 2 compiled shader programs (vertex and fragment) links them together to a single program and removes them.
// Takes as input the returned IDs by CompileShader function of vertex, and fragment shader programs.
unsigned int Shader::LinkShaders(unsigned int vertex_shader, unsigned int fragment_shader){
    unsigned int program = glCreateProgram();
    CHECK(glAttachShader(program, vertex_shader));
    CHECK(glAttachShader(program, fragment_shader));
    CHECK(glLinkProgram(program));
    CHECK(glValidateProgram(program));

    CHECK(glDeleteShader(vertex_shader));
    CHECK(glDeleteShader(fragment_shader));

    int success;
    int len;

    CHECK(glGetProgramiv(program, GL_LINK_STATUS, &success));
    CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));

    // Since we use infoLog to perform 2 checks, we need to allocate on the heap memory as we cant free or reallocate allocated memory on stack manually
    // Program linking check
    char* infoLog = (char*)malloc(sizeof(char) * len);
    if(!success){
        glGetProgramInfoLog(program, len, &len, infoLog);
        std::cout << "ERROR: SHADER PROGRAM LINKING FAILED!\n" << infoLog << std::endl;
    }
    free(infoLog);
    // Program validation check
    CHECK(glGetProgramiv(program, GL_VALIDATE_STATUS, &success));
    CHECK(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len));
    if(!success){
        infoLog = (char*)alloca(sizeof(char) * len);
        std::cout << "ERROR: SHADER PROGRAM VALIDATION FAILED!" << infoLog << std::endl;
    }
    return program;
}

// Shader Constructor

Shader::Shader(const char* vertex_path, const char* fragment_path){
    glfwInit(); // Initialize GLFW library

    // WINDOW HINTS ALLOWS US TO CONFIGURE OUR WINDOW BEFORE ACTUALLY CREATING IT
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // WE'RE USING OPENGL 3.3 SO MAJOR VERSION SHOULD BE SET TO 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // MINOR VERSION SHOULD BE SET TO 3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //WE DON'T WANT OUR APPLICATION TO BE BACKWARDS COMPATIBLE (3.3+) SO WE USE CORE PROFILE
    GLFWwindow* window;

    // Check initialize GLFW3
    if (!glfwInit()){
        std::cout << "Failed to Initialize GLFW" << std::endl;
    }

    //Create a window
    window = glfwCreateWindow(800, 600, "My Project", NULL, NULL);
    if (window == NULL){
        glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Check initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    char* vertexShaderSource = ReadGLSL(vertex_path);
    unsigned int vs = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    free(vertexShaderSource);

    char* fragShaderSource = ReadGLSL(fragment_path);
    unsigned int fs = CompileShader(fragShaderSource, GL_FRAGMENT_SHADER);
    free(fragShaderSource);

    this->ID = LinkShaders(vs, fs);
    CHECK(glDeleteShader(vs));
    CHECK(glDeleteShader(fs));
}
// Use this shader instance
void Shader::use(){
    CHECK(glUseProgram(this->ID));
}

//Set bool uniform value for this program
void Shader::setBool(const std::string &name, bool value) const {
    CHECK(glUniform1i( glGetUniformLocation(ID, name.c_str()), (int)value ));
}

//Set int uniform value for this program
void Shader::setInt(const std::string &name, int value) const{
    CHECK(glUniform1i( glGetUniformLocation(ID, name.c_str()), value ));
}

//Set float uniform value for this program
void Shader::setFloat(const std::string &name, float value) const{
    CHECK(glUniform1f( glGetUniformLocation(ID, name.c_str()), value ));
}

//Set 4X4 float matrix uniform for this program
void Shader::setMat4f(const std::string &name, glm::mat4 mat4f) const{
    glUniformMatrix4fv( glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4f));
}

void Shader::setMat4f(const std::string &name, float* value) const{
    glUniformMatrix4fv( glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

//Set 3X1 float vector uniform form this program (passing in x, y, z values explicitly)
void Shader::setVec3f(const std::string &name, float x, float y, float z) const{
    CHECK(glUniform3f( glGetUniformLocation(ID, name.c_str()), x, y, z));
}

//Overload function of the above, you pass in the glm::vec3 vector directly this time.
void Shader::setVec3f(const std::string &name, glm::vec3 vector3) const{
    CHECK(glUniform3f( glGetUniformLocation(ID, name.c_str()), vector3.x, vector3.y, vector3.z ));
}
