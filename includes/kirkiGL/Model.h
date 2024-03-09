#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <gldebug.h>
#include <kirkiGL/Object.h>
#include <kirkiGL/Mesh.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Object{
    public:
        Model(const char* path, glm::vec3 init_pos, Shader& program) : Object(init_pos, program, SPACE_MODEL){
            this->LoadModel(path);
        }
        void Draw(Shader &program);
    private:
        std::vector<Texture> loaded_textures;
        std::string tex_dir;
        std::vector<Mesh> Meshes;
        void LoadModel(std::string path);
        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextureMaps(aiMaterial* mat, aiTextureType type, KGL_TEXTURE_TYPE kgl_type);
        unsigned int TextureFromFile(const char* path, const std::string &directory);
};
#endif