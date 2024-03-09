#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <kirkiGL/Shader.h>
#include <string>

enum KGL_SPACE{
    SPACE_MODEL = 0, SPACE_VIEW, SPACE_PROJECTION  
};
class Object{
    public:
        Object(glm::vec3 init_pos,glm::vec3 init_rot_axis, float init_rot_deg, glm::vec3 init_scale,Shader& program, KGL_SPACE spc);
        Object(glm::vec3 init_pos, Shader& program, KGL_SPACE spc);
        void Transform(glm::vec3 transform_vector);
        void Rotate(float deg, glm::vec3 rotate_axis);
        void Scale(glm::vec3 scale_vector);
        void SetSpace(KGL_SPACE space);
    private:
        glm::mat4 trans;
        std::string space;
        Shader* Program;
};
#endif