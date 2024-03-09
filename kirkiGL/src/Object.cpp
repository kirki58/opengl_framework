#include <kirkiGL/Object.h>

Object::Object(glm::vec3 init_pos,glm::vec3 init_rot_axis, float init_rot_deg, glm::vec3 init_scale,Shader& program, KGL_SPACE spc){
    this->trans = glm::mat4(1.0f);
    this->trans = glm::translate(trans, init_pos);
    this->trans = glm::rotate(trans, glm::radians(init_rot_deg), init_rot_axis);
    this->trans = glm::scale(trans, init_scale);
    this->SetSpace(spc);
    program.setMat4f(this->space, glm::value_ptr(trans));
    this->Program = &program;
}
Object::Object(glm::vec3 init_pos, Shader& program, KGL_SPACE spc){
    this->trans = glm::mat4(1.0f);
    this->trans = glm::translate(trans, init_pos);
    this->SetSpace(spc);
    program.setMat4f(space, glm::value_ptr(trans));
    this->Program = &program;
}
void Object::Transform(glm::vec3 transform_vector){
    this->trans = glm::translate(trans, transform_vector);
    this->trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
    this->Program->setMat4f(space, glm::value_ptr(trans));
}
void Object::Rotate(float deg, glm::vec3 rotate_axis){
    this->trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
    this->trans = glm::rotate(trans, glm::radians(deg), rotate_axis);
    this->trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
    this->Program->setMat4f(space, glm::value_ptr(trans));
}
void Object::Scale(glm::vec3 scale_vector){
    this->trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));
    this->trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->trans = glm::scale(trans, scale_vector);
    this->Program->setMat4f(space, glm::value_ptr(trans));
}
void Object::SetSpace(KGL_SPACE space){
    if(space == 0) this->space = "model";
    else if(space == 1) this->space = "view";
    else if (space == 2) this->space = "projection";
}