#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"

//#define STB_IMAGE_IMPLEMENTATION // Preprocessor will modify the header file so it only contains relevant source code, effectively turning it into a .cpp.
//#include "stb_image.h"

using namespace std;
using namespace glm;

class Model
{
public:
    Model(const char *path)
    {
        loadModel(path);
    }

    void Draw(Shader &shader) // This function is used to draw each respective mesh using the necessary shader for the model.
    {
        for(unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].Draw(shader);
        }
    };

private:
    vector<Mesh> meshes; // Model data
    string directory; // Stored directory for later use of texture finding.
    vector<Texture> textures_loaded; // Optimization list to compare to while attempting to load new textures.

    void loadModel(string path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        // Check to see that the scene was able to load the model properly.
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }

        this->directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    };

    /*  
        IMPORTANT NOTE: Each node contains meshes that exist in the scene to allow for reusability!
        A mesh exists as a member of a scene, but each node controls its own meshes transform data.
        This creates a parent-child relationship with meshes.
    */
    void processNode(aiNode *node, const aiScene *scene) // Recursive function that processes each node!
    {
        // A breadth-first search algorithm:
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        //cout << "made it past the MESH loading." << endl;

        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
        //cout << "made it past the NODE loading" << endl;
    };

    // Method to be called when each mesh needs to be processed.
    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        
        // Setup each vertex.
        for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex v;

            v.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            v.normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            // Check to see if there are any texture coordinates.
            if(mesh->mTextureCoords[0]) {
                vec2 tex;
                // Assimp allows for up to 8 texture coordinates to be held per vertex, we only want to use 1.
                tex.x = mesh->mTextureCoords[0][i].x;
                tex.y = mesh->mTextureCoords[0][i].y;
                v.texCoord = tex;
            }
            else {
                v.texCoord = vec2(0.0f, 0.0f);
            }

            vertices.push_back(v);
        }

        // Setup each index.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++) { // Look at each face saved in the meshes data.
            for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) { // Look at the indices that make a face, and put them into our indices list.
                indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }

        // Setup the materials.
        if(mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            // Load the diffuse maps from the scenes material.
            vector<Texture> diffMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());
            // Load the specular maps from the scenes material.
            vector<Texture> specMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specMaps.begin(), specMaps.end());
        }

        return Mesh(vertices, indices, textures);
    };

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString s;
            mat->GetTexture(type, i, &s);
            bool skip = false;
            for(unsigned int j = 0; j < textures_loaded.size(); j++) {
                if(strcmp(textures_loaded[j].path.data(), s.C_Str()) == 0) { // Compare the path of already loaded textures with the texture we are attempting to load from this material.
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            if(!skip) {
                Texture t;
                t.id = loadTexture(s.C_Str(), directory);
                t.type = typeName;
                t.path = s.C_Str();
                textures.push_back(t);
                textures_loaded.push_back(t); // Add to already loaded textures.
            }
        }

        return textures;
    };

    unsigned int loadTexture(char const *path, string directory)
    {
        unsigned int texID;
        glGenTextures(1, &texID);

        int width, height, nrComponents;
        string p = directory + "/" + string(path);
        unsigned char *data = stbi_load(p.c_str(), &width, &height, &nrComponents, 0);
        if(data) {
            GLenum format;
            if(nrComponents == 1)
                format = GL_RED;
            else if(nrComponents == 3)
                format = GL_RGB;
            else if(nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            cout << "Texture failed to load at path: " << path << endl;
        }

        stbi_image_free(data);
        return texID;
    };
};

#endif