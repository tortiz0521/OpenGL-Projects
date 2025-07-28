#include "../headers/resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "../headers/stb_image.h"

using namespace std;

// Instantiate the static variables:
map<string, Texture2D> ResourceManager::Textures;
map<string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::loadShaderFromFile(const char *vShader, const char *fShader, const char *gShader)
{
    string vertex, fragment, geo; // Output from shader files.
    ifstream vfile, ffile, gfile; // File streams for shader files.

    vfile.exceptions(ifstream::failbit | ifstream::badbit); // Handle exceptions during the file read.
    ffile.exceptions(ifstream::failbit | ifstream::badbit);
    if(gShader != nullptr)
        gfile.exceptions(ifstream::failbit | ifstream::badbit);


    try {
        stringstream vStream, fStream, gStream; // Vars to hold the string streams from shader code

        // Vertex shader file stream.
        vfile.open(vShader);
        vStream << vfile.rdbuf();
        vfile.close();
        vertex = vStream.str();

        // Fragment shader file stream.
        ffile.open(fShader);
        fStream << ffile.rdbuf();
        ffile.close();
        fragment = fStream.str();

        // Geometry shader file stream.
        if(gShader != nullptr) {
            gfile.open(gShader);
            gStream << gfile.rdbuf();
            gfile.close();
            geo = gStream.str();
        }
    } catch(ifstream::failure &e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
    }

    // c_str() conversion from type string. sstream cannot directly support c_str() conversion.
    const char *vCode = vertex.c_str(), *fCode = fragment.c_str(), *gCode;
    if(gShader != nullptr)
        gCode = geo.c_str();
    
    Shader shader;
    shader.Compile(vCode, fCode, gShader != nullptr ? gCode : nullptr);
    return shader;
}

Shader ResourceManager::GetShader(string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, string name)
{
    Textures[name] = loadTextureFromFile(file);
    return Textures[name];
}

Texture2D ResourceManager::loadTextureFromFile(const char *file)
{
    Texture2D texture;

    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
    if(data) {
        if(nrChannels == 1) {
            texture.Internal_Format = GL_RED;
            texture.Image_Format = GL_RED;
        }
        else if(nrChannels == 3) {
            texture.Internal_Format = GL_RGB;
            texture.Image_Format = GL_RGB;
        }
        else if(nrChannels == 4) {
            if (strcmp(file, "textures/paddle.png") == 0)
                cout << "HERE" << endl;
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }

        texture.Generate(width, height, data);
    }
    else {
        cout << "Texture failed to load at path: " << file << endl;
    }

    stbi_image_free(data);
    return texture;
}

Texture2D ResourceManager::GetTexture(string name)
{
    return Textures[name];
}

// Delete all Shaders and Textures (to most likely be run at the end of the programs life).
void ResourceManager::Clear()
{
    for(auto iter : Shaders)
        glDeleteProgram(iter.second.ID);

    for(auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}