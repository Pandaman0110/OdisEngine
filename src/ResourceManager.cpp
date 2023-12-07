#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/gl.h>
#include <SOIL/SOIL.h>

#include "Log.h"

using namespace OdisEngine;

ResourceManager::ResourceManager(std::string font_path, std::string shader_path) : font_path(font_path), shader_path(shader_path)
{
}

GLSLShader& ResourceManager::load_shader(const std::string& v_shader_file_name, const std::string& f_shader_file_name, const std::string& g_shader_file_name, std::string name)
{
    shaders.insert({ name, load_shader_from_file(v_shader_file_name, f_shader_file_name, g_shader_file_name)});
    return get_shader(name);
}


Texture& ResourceManager::load_texture(const std::string& file_name, std::string name)
{
    textures.insert({ name, load_texture_from_file(file_name) });
    return get_texture(name);
}


Font& ResourceManager::load_font(const std::string& file_name, uint8_t height, std::string name)
{
    fonts.insert({ name, load_font_from_file(file_name, height) });
    return get_font(name);
}

FrameBuffer& ResourceManager::load_framebuffer(int w, int h, std::string name)
{
    frame_buffers.insert({ name, FrameBuffer{w, h} });
    return get_frame_buffer(name);
}

GLSLShader& ResourceManager::get_shader(const std::string& name)
{
    return shaders.at(name);
}

Texture& ResourceManager::get_texture(const std::string& name)
{
    return textures.at(name);
}


Font& ResourceManager::get_font(const std::string& name)
{
    return fonts.at(name);
}

FrameBuffer& ResourceManager::get_frame_buffer(const std::string& name)
{
    return frame_buffers.at(name);
}

void ResourceManager::clear()
{
    // (properly) delete all shaders	
    for (auto &iter : shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto &iter : textures)
        glDeleteTextures(1, (GLuint*)&iter.second.id);
    
    shaders.clear();
    textures.clear();
    fonts.clear();
}

GLSLShader ResourceManager::load_shader_from_file(const std::string& v_shader_file_name, const std::string& f_shader_file_name, const std::string& g_shader_file_name)
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
        if (!g_shader_file_name.empty())
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
    shader.compile(v_shader_code, f_shader_code, !g_shader_file_name.empty() ? g_shader_code : nullptr);
    return shader;
}

Texture ResourceManager::load_texture_from_file(const std::string& file_name)
{
    // load image
    int width, height, nr_channels;

   uint8_t* data = SOIL_load_image(file_name.c_str(), &width, &height, &nr_channels, 0);

    //now generate texture
    Texture texture{ width, height, data};

    // and finally free image data
    SOIL_free_image_data(data);

    return texture;
}

Font ResourceManager::load_font_from_file(const std::string& file_name, uint8_t height)
{
    FT_Library ft{};

    if (FT_Init_FreeType(&ft))
    {
        logger->get("OdisEngine")->log(LogLevel::warning, "Could not init FreeType Library");
    }

    FT_Face face{};
    Font font{};

    if (FT_New_Face(ft, (font_path + file_name).data(), 0, &face))
    {
        logger->get("OdisEngine")->log(LogLevel::warning, "Failed to load font");
    }

    ///0 lets it choose the width dynamically
    FT_Set_Pixel_Sizes(face, 0, height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (uint8_t c = 0; c < NUM_FONT_CHARACTERS; ++c)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            logger->get("OdisEngine")->logf(LogLevel::warning, "{}: {}", "Failed to load glyph num", c);
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2{face->glyph->bitmap.width, face->glyph->bitmap.rows },
            glm::ivec2{face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x
        };
        font.characters.at(c) = character;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return font;
}