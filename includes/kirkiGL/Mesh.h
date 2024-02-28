#pragma once

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
};

class Mesh {
    public:
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
        std::vector<Texture> Textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, unsigned int draw_type);
        void Draw(Shader &shader);
    private:
        unsigned int VAO, VBO, EBO;
        void configMesh(unsigned int draw_type);
};
