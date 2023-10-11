#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "Texture2d.h"
#include "GLSLShader.h"

const std::string shader_path = "shaders/";

class ResourceManager
{
public:
    ResourceManager() {};

    GLSLShader& load_shader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name);
    // retrieves a stored sader
    GLSLShader& get_shader(std::string name);
    // loads (and generates) a texture from file
    Texture2D& load_texture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    Texture2D& get_texture(std::string name);
    // properly de-allocates all loaded resources
    void clear();
private:
    std::map<std::string, GLSLShader> shaders;
    std::map<std::string, Texture2D> textures;

    // loads and generates a shader from file
    GLSLShader load_shader_from_file(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file = nullptr);
    // loads a single texture from file
    Texture2D load_texture_from_file(const char* file, bool alpha);
};

#endif
