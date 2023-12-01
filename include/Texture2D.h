#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>

#include <glad/gl.h>

namespace OdisEngine
{
    // Texture2D is able to store and configure a texture in OpenGL.
    // It also hosts utility functions for easy management.
    class Texture
    {
    public:
        // holds the ID of the texture object, used for all texture operations to reference to this particular texture
        uint32_t ID;
        // texture image dimensions
        uint32_t w, h; // width and height of loaded image in pixels
        // texture Format
        uint32_t interal_format; // format of texture object
        uint32_t image_format; // format of loaded image
        // texture configuration
        uint32_t wrap_s; // wrapping mode on S axis
        uint32_t wrap_t; // wrapping mode on T axis
        uint32_t filter_min; // filtering mode if texture pixels < screen pixels
        uint32_t filter_max; // filtering mode if texture pixels > screen pixels
        //name
        std::string name;
        // constructor (sets default texture modes)
        Texture()
            : w(0), h(0), interal_format(GL_RGB), image_format(GL_RGB), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR)
        {
            glGenTextures(1, reinterpret_cast<GLuint*>(&this->ID));
        }
        // generates texture from image data
        void generate(uint32_t w, uint32_t h, uint8_t* data)
        {
            this->w = w;
            this->h = h;
            // create Texture
            glBindTexture(GL_TEXTURE_2D, this->ID);
            glTexImage2D(GL_TEXTURE_2D, 0, this->interal_format, this->w, this->w, 0, this->image_format, GL_UNSIGNED_BYTE, data);
            // set Texture wrap and filter modes
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max);
            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // binds the texture as the current active GL_TEXTURE_2D texture object
        void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, this->ID);
        }
    };
}
#endif