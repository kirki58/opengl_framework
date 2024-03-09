#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <kirkiGL/Shader.h>
// Vertex struct
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

enum KGL_TEXTURE_TYPE{
    KGL_TEXTURE_DIFFUSE = 0,
    KGL_TEXTURE_SPECULAR
};

// Texture struct
struct Texture {
    unsigned int Id;
    KGL_TEXTURE_TYPE type; // diffuse, specular
    std::string path;
};

class Mesh {
    public:
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, unsigned int draw_type = GL_STATIC_DRAW);
        void Draw(Shader &shader);
        void SetDrawType(unsigned int draw_type){
            this->Draw_Type = draw_type;
        }
    private:
        unsigned int VAO, VBO, EBO;
        unsigned int Draw_Type;
        void configMesh();
};

#endif