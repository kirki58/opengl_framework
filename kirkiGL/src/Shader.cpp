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
unsigned int Shader::CompileShader(const char* shader_source, unsigned int type){

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    int success;
    int len;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    char* infoLog = (char*)alloca(sizeof(char) * len); // "alloca" function allocates memory in stack so no need to free it.

    if(!success){
        glGetShaderInfoLog(shader, len, &len, infoLog);
        std::cout << "ERROR: SHADER COMPILATION ERROR FOR " << (type == GL_VERTEX_SHADER ? "VERTEX SHADER\n":"FRAGMENT SHADER\n") <<  infoLog << std::endl;
    }
    return shader;
}

// LinkShaders function Links 2 compiled shader programs (vertex and fragment) links them together to a single program and removes them.
// Takes as input the returned IDs by CompileShader function of vertex, and fragment shader programs.
unsigned int Shader::LinkShaders(unsigned int vertex_shader, unsigned int fragment_shader){
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    int len;

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

    // Since we use infoLog to perform 2 checks, we need to allocate on the heap memory as we cant free or reallocate allocated memory on stack manually
    // Program linking check
    char* infoLog = nullptr;
    if(!success){
        infoLog = (char*)malloc(sizeof(char) * len);
        glGetProgramInfoLog(program, len, &len, infoLog);
        std::cout << "ERROR: SHADER PROGRAM LINKING FAILED!\n" << infoLog << std::endl;
        free(infoLog);
    }
    // Program validation check
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if(!success){
        infoLog = (char*)alloca(sizeof(char) * len);
        std::cout << "ERROR: SHADER PROGRAM VALIDATION FAILED!" << infoLog << std::endl;
        free(infoLog);
    }
    return program;
}

// Shader Constructor

Shader::Shader(const char* vertex_path, const char* fragment_path){
    char* vertexShaderSource = this->ReadGLSL(vertex_path);
    unsigned int vs = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    free(vertexShaderSource);

    char* fragShaderSource = this->ReadGLSL(fragment_path);
    unsigned int fs = CompileShader(fragShaderSource, GL_FRAGMENT_SHADER);
    free(fragShaderSource);

    this->ID = this->LinkShaders(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
}
// Use this shader instance
void Shader::use(){
    glUseProgram(this->ID);
}

//Set bool uniform value for this program
void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i( glGetUniformLocation(ID, name.c_str()), (int)value );
}

//Set int uniform value for this program
void Shader::setInt(const std::string &name, int value) const{
    glUniform1i( glGetUniformLocation(ID, name.c_str()), value );
}

//Set float uniform value for this program
void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f( glGetUniformLocation(ID, name.c_str()), value );
}

//Set 4X4 float matrix uniform for this program
void Shader::setMat4f(const std::string &name, glm::mat4 mat4f) const{
    glUniformMatrix4fv( glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4f));
}

//Set 3X1 float vector uniform form this program (passing in x, y, z values explicitly)
void Shader::setVec3f(const std::string &name, float x, float y, float z) const{
    glUniform3f( glGetUniformLocation(ID, name.c_str()), x, y, z);
}

//Overload function of the above, you pass in the glm::vec3 vector directly this time.
void Shader::setVec3f(const std::string &name, glm::vec3 vector3) const{
    glUniform3f( glGetUniformLocation(ID, name.c_str()), vector3.x, vector3.y, vector3.z );
}
