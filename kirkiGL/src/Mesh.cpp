#include <kirkiGL/Mesh.h>

//Mesh Constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures){
    this->Vertices = vertices;
    this->Indices = indices;
    this->Textures = textures;
    configMesh(GL_STATIC_DRAW);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, unsigned int draw_type){
    this->Vertices = vertices;
    this->Indices = indices;
    this->Textures = textures;
    configMesh(draw_type);
}

void Mesh::configMesh(unsigned int draw_type){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), draw_type);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), draw_type);

    //Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    //Normal Attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    //Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &program){
    unsigned int diffuseNr = 0;
    unsigned int specNr = 0;
    for (size_t i = 0; i < Textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        std::string tex_nr;
        std::string tex_type;
        if(Textures[i].type == 0){
            tex_type = "diffuse";
            tex_nr = std::to_string(diffuseNr++);
        }
        else if(Textures[i].type == 1){
            tex_type == "specular";
            tex_nr = std::to_string(specNr++);
        }
        program.setInt(("material." + tex_type + tex_nr).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
    }
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); //Unbind VAO
}