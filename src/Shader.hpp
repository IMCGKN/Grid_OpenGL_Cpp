#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* v_File, const char* f_File)
    {
        std::string vertexStringSource;
        std::string fragmentStringSource;
        std::ifstream VertexFile;
        std::ifstream FragmentFile;
        VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            VertexFile.open(v_File);
            FragmentFile.open(f_File);
            std::stringstream vertexss;
            std::stringstream fragmentss;
            vertexss << VertexFile.rdbuf();
            fragmentss << FragmentFile.rdbuf();

            vertexStringSource = vertexss.str();
            fragmentStringSource = fragmentss.str();
        }
        catch(std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }

        const char* vs_source = vertexStringSource.c_str();
        const char* fs_source = fragmentStringSource.c_str();

        int success;
        char infoLog[512];

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vs_source, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "Error in vertex shader\n : " << infoLog;
        }

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fs_source, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "Error in fragment shader\n : " << infoLog;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void SetInt(std::string name, int i)
    {
        Use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), i);
    }

    void SetFloat(std::string name, float i)
    {
        Use();
        glUniform1f(glGetUniformLocation(ID, name.c_str()), i);
    }

    void SetMat4(std::string name, glm::mat4 v)
    {
        Use();
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
    }

    void Use()
    {
        glUseProgram(ID);
    }

    void Delete()
    {
        glDeleteProgram(ID);
    }
};
