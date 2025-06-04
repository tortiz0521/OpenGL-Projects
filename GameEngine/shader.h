#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

using namespace glm;
using namespace std;

class Shader
{
private:
    void checkCompileError(unsigned int shader, string type)
    {
        // Error Logging data variables.
        int  success;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(type != "PROGRAM")
        {
            if(!success) 
            {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::"<< type << "::COMPILATION_FAILED\n" << infoLog << endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shader, 512, NULL, infoLog);
                cout << "ERROR::SHADER::PROGRAM_LINK_FAILED\n" << infoLog << endl;
            }
        }
    };

public:
    unsigned int ID; // The program name/ID.

    Shader(const char *vSource, const char *fSource)
    {
        string vertex, fragment;
        ifstream vfile, ffile;

        vfile.exceptions(ifstream::failbit | ifstream::badbit);
        ffile.exceptions(ifstream::failbit | ifstream::badbit);
        
        try {
            stringstream vStream, fStream;
            vfile.open(vSource);
            vStream << vfile.rdbuf();
            vfile.close();

            ffile.open(fSource);
            fStream << ffile.rdbuf();
            ffile.close();

            vertex = vStream.str();
            fragment = fStream.str();
        } catch(ifstream::failure &e) {
            cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
        } 

        const char *vCode = vertex.c_str(), *fCode = fragment.c_str(); 
        unsigned int v, f;

        // Create vertex shader.
        v = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(v, 1, &vCode, NULL);
        glCompileShader(v);
        checkCompileError(v, "VERTEX");

        // Create fragment shader.
        f = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(f, 1, &fCode, NULL);
        glCompileShader(f);
        checkCompileError(f, "FRAGMENT");

        // Create the shader program.
        // Tell OpenGL to use our Shader Program when we issue render calls, delete no-longer-needed shaders!
        ID = glCreateProgram();
        glAttachShader(ID, v);
        glAttachShader(ID, f);
        glLinkProgram(ID);
        checkCompileError(ID, "PROGRAM");

        glDeleteShader(v);
        glDeleteShader(f);
    };

    void use()
    {
        glUseProgram(ID);
    };

    // Utility functions: query a uniform variable location, and set its value;
    void setBool(const string &name, bool value) const // This const declares that this method is a constant member method. It is not allowed to modify the object's state.
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }; 

    void setInt(const string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    };

    void setFloat(const string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    };

    void setFloat4f(const string &name, array<float, 4> value) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[4]);
    };

    void setVec3(const string &name, vec3 value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value));
    };

    void setVec4(const string &name, vec4 value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value_ptr(value));
    };

    void setMatrix4fv(const string &name, mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value_ptr(value));
    };
};

#endif