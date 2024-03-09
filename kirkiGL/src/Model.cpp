#include <kirkiGL/Model.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Model::Draw(Shader &program){
    for (size_t i = 0; i < Meshes.size(); i++)
    {
        Meshes[i].Draw(program);
    }
}
void Model::LoadModel(std::string path){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        std::cout << "[ERROR ASSIMP]: " << importer.GetErrorString() << std::endl;
        return;
    }
    this->tex_dir = path.substr(0, path.find_last_of('/'));

    this->ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene){
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
           aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
           // node->mMeshes[i] has the required indices of a node in the scene struct.
           // node->mMeshes array has numbers but scene->mMeshes array actually has got the meshes
           this->Meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        this->ProcessNode(node->mChildren[i], scene);
    }
}
Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene){
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = glm::vec3(mesh->mNormals->x, mesh->mNormals->y, mesh->mNormals->z);
        if(mesh->mTextureCoords[0]){
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face= mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = this->LoadMaterialTextureMaps(material, aiTextureType_DIFFUSE, KGL_TEXTURE_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specMaps = this->LoadMaterialTextureMaps(material, aiTextureType_SPECULAR, KGL_TEXTURE_SPECULAR);
        textures.insert(textures.end(), specMaps.begin(), specMaps.end());
        // concetanating to vectors into one should be done by inserting each vector to the general vector FROM THE END
        // doing so increases performance and reduces the likelihood of reallocating memory.

    }
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextureMaps(aiMaterial* mat, aiTextureType type, KGL_TEXTURE_TYPE kgl_type){
    std::vector<Texture> maps;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (size_t j = 0; j < this->loaded_textures.size(); j++)
        {
            if(std::strcmp( loaded_textures[j].path.data(), str.C_Str() ) == 0 ){
                maps.push_back(loaded_textures[j]);
                skip = true;
                break;
            }
        }
        if(!skip){
            //if texture does not exist in this->loaded_textures
            Texture texture;
            texture.Id = this->TextureFromFile(str.C_Str(), this->tex_dir);
            texture.path = str.C_Str();
            texture.type = kgl_type;
            maps.push_back(texture);
            loaded_textures.push_back(texture);
        }
    }
    return maps;
     
}

unsigned int Model::TextureFromFile(const char* path, const std::string &directory){
    std::string file = std::string(path);
    file = directory + '/' + file;

    unsigned int textureID;
    CHECK(glGenTextures(1, &textureID));

    int width, height, nrComp;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrComp, 0);
    if(data){
        GLenum format;
        if (nrComp == 1)
            format = GL_RED;
        else if (nrComp == 3)
            format = GL_RGB;
        else if (nrComp == 4)
            format = GL_RGBA;

        CHECK(glBindTexture(GL_TEXTURE_2D, textureID));
        CHECK(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        CHECK(glGenerateMipmap(GL_TEXTURE_2D));

        CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        stbi_image_free(data);
    }
    else{
        std::cout << "Texture failed to load from path: " << file << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}