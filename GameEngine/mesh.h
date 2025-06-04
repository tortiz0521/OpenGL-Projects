#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

using namespace glm;
using namespace std;

struct Vertex 
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

struct Texture
{
    unsigned int id;
    string type;
    string path; // Store a path for the texture to compare with other textures. Resusing textures is taxing!
};

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // 1. bind Vertex Array Object | Triangle 1!
        glBindVertexArray(VAO);
        // 2. copy our vertices list in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // For ELEMENT BUFFER OBJECTS, we need to specify that we are using a GL_ELEMENT_ARRAY_BUFFER!!!
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // 4. then set the vertex attributes pointers
        // Vertex layout information.
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Normals layout information.
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // Texture Coords layout information.
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindVertexArray(0);
    };

public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    };

    void Draw(Shader &shader)
    {
        // We go through the process of setting each texture for the specific mesh BECAUSE we have to change the textures we are using for every mesh.
        unsigned int diffNum = 1, specNum = 1;
        for(unsigned int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i); // Activate the correct texture unit!

            string num, name = textures[i].type;
            if(name == "texture_diffuse")
                num = to_string(diffNum++);
            else if(name == "texture_specular")
                num = to_string(specNum++);

            shader.setInt("material." + name + num, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Draw the mesh!
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Since we have indices for each mesh, we use glDrawElements instead of glDrawArrays.
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    };
};

#endif