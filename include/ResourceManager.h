#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <memory>

#include <string_view>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Texture2d.h"
#include "FrameBuffer.h"
#include "GLSLShader.h"
#include "Font.h"

namespace OdisEngine
{
    class ResourceManager
    {
    public:
        ResourceManager(std::string font_path, std::string shader_path);

        GLSLShader& load_shader(const std::string& v_shader_file_name, const std::string& f_shader_file_name, const std::string& g_shader_file_name, std::string name);
        Texture& load_texture(const std::string& file_name, std::string name);
        Font& load_font(const std::string& file_name, uint8_t height, std::string name);
        FrameBuffer& load_framebuffer(int w, int h, std::string name);

        GLSLShader& get_shader(const std::string& name);
        Texture& get_texture(const std::string& name);
        Font& get_font(const std::string& name);
        FrameBuffer& get_frame_buffer(const std::string& name);
        
     
        void clear();
    private:
        std::map<std::string, GLSLShader> shaders;
        std::map<std::string, Texture> textures;
        std::map<std::string, FrameBuffer> frame_buffers;
        std::map<std::string, Font> fonts;

        std::string shader_path = "shaders/";
        std::string font_path = "fonts/";

        GLSLShader load_shader_from_file(const std::string& v_shader_file_name, const std::string& f_shader_file_name, const std::string& g_shader_file_name);
        Texture load_texture_from_file(const std::string& file_name);
        Font load_font_from_file(const std::string& file_name, uint8_t height);
    };
}

extern std::unique_ptr<OdisEngine::ResourceManager> resource_manager;

#endif
