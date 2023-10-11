#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/gl.h>
#include <SOIL/SOIL.h>

GLSLShader& ResourceManager::load_shader(const char* v_shader_file_name, const char* f_shader_file_name, const char* g_shader_file_name, std::string name)
{
    shaders.insert({ name, load_shader_from_file(v_shader_file_name, f_shader_file_name, g_shader_file_name) });
    return shaders.at(name);
}

GLSLShader& ResourceManager::get_shader(std::string name)
{
    return shaders.at(name);
}

Texture2D& ResourceManager::load_texture(const char* file, bool alpha, std::string name)
{
    textures.insert({ name, load_texture_from_file(file, alpha) });
    return textures.at(name);
}

Texture2D& ResourceManager::get_texture(std::string name)
{
    return textures.at(name);
}

void ResourceManager::clear()
{
    // (properly) delete all shaders	
    for (auto &iter : shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto &iter : textures)
        glDeleteTextures(1, (GLuint*)&iter.second.ID);
}

GLSLShader ResourceManager::load_shader_from_file(const char* v_shader_file_name, const char* f_shader_file_name, const char* g_shader_file_name)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;
    try
    {
        // open files
        std::ifstream vertex_shader_file(shader_path + v_shader_file_name);
        std::ifstream fragment_shader_file(shader_path + f_shader_file_name);
        std::stringstream v_shader_stream, f_shader_stream;
        // read file's buffer contents into streams
        v_shader_stream << vertex_shader_file.rdbuf();
        f_shader_stream << fragment_shader_file.rdbuf();
        // close file handlers
        vertex_shader_file.close();
        fragment_shader_file.close();
        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
        // if geometry shader path is present, also load a geometry shader
        if (g_shader_file_name != nullptr)
        {
            std::ifstream geometry_shader_file(shader_path + g_shader_file_name);
            std::stringstream g_shader_stream;
            g_shader_stream << geometry_shader_file.rdbuf();
            geometry_shader_file.close();
            geometry_code = g_shader_stream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();
    const char* g_shader_code = geometry_code.c_str();
    // 2. now create shader object from source code
    GLSLShader shader;
    shader.compile(v_shader_code, f_shader_code, g_shader_file_name != nullptr ? g_shader_code : nullptr);
    return shader;
}

Texture2D ResourceManager::load_texture_from_file(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.interal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }
    // load image
    int width, height, nr_channels;

    unsigned char* data = SOIL_load_image(file, &width, &height, &nr_channels, 0);

    //now generate texture
    texture.generate(width, height, data);

    // and finally free image data
    SOIL_free_image_data(data);

    return texture;
}