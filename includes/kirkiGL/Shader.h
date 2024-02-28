#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Shader{
    private:
        unsigned int ID;
        
        char* ReadGLSL(const char* path);
        unsigned int CompileShader(const char* shader_source, unsigned int type);
        unsigned int LinkShaders(unsigned int vertex_shader, unsigned int fragment_shader);
    public:
        Shader(const char* vertex_path, const char* fragment_path);
        void use();

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4f(const std::string &name, glm::mat4 mat4f) const;
        void setVec3f(const std::string &name, float x, float y, float z) const;
        void setVec3f(const std::string &name, glm::vec3 vector3)const;
};