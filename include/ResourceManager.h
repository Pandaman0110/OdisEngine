#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Texture2d.h"
#include "GLSLShader.h"
#include "Font.h"

namespace OdisEngine
{
    class ResourceManager
    {
    public:
        ResourceManager(std::string font_path, std::string shader_path);

        GLSLShader& load_shader(std::string v_shader_file, std::string f_shader_file, std::string g_shader_file, std::string name);
        GLSLShader& get_shader(std::string name);
        Texture2D& load_texture(std::string file_name, std::string name, bool alpha);
        Texture2D& get_texture(std::string file_name);
        Font& load_font(std::string file_name, std::string name, uint8_t h);
        Font& get_font(std::string file_name, std::string name);
     
        void clear();
    private:
        std::map<std::string, GLSLShader> shaders;
        std::map<std::string, Texture2D> textures;
        std::map<std::string, Font> fonts;

        std::string shader_path = "shaders/";
        std::string font_path = "fonts/";

        GLSLShader load_shader_from_file(std::string v_shader_file, std::string f_shader_file, std::string g_shader_file = "");
        Texture2D load_texture_from_file(std::string file_name, bool alpha);
        Font load_font_from_file(std::string file_name, uint8_t height);
    };
}

#endif
