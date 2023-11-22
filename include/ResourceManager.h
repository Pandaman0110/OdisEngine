#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <string_view>

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

        GLSLShader& load_shader(const std::string& v_shader_file_name, const std::string& f_shader_file_name, const std::string& g_shader_file_name, std::string name);
        Texture2D& load_texture(const std::string& file_name, bool alpha, std::string name);
        Font& load_font(const std::string& file_name, uint8_t height, std::string name);

        GLSLShader& get_shader(const std::string& name);
        Texture2D& get_texture(const std::string& name);
        Font& get_font(const std::string& name);

     
        void clear();
    private:
        std::map<std::string, GLSLShader> shaders;
        std::map<std::string, Texture2D> textures;
        std::map<std::string, Font> fonts;

        std::string shader_path = "shaders/";
        std::string font_path = "fonts/";

        GLSLShader load_shader_from_file(const std::string& v_shader_file_name, const std::string& f_shader_file_name, const std::string& g_shader_file_name);
        Texture2D load_texture_from_file(const std::string& file_name, bool alpha);
        Font load_font_from_file(const std::string& file_name, uint8_t height);
    };
}

#endif
