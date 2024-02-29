#pragma once
#include <kirkiGL/Object.h>

class Model : public Object{
    public:
        Model(char* path, glm::vec3 init_pos, Shader& program) : Object(init_pos, program, SPACE_MODEL){
            this->LoadModel(path);
        }
    private:
        void LoadModel(char* path);
};