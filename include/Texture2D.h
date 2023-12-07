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
        unsigned int id;
        // texture image dimensions
        int w, h; // width and height of loaded image in pixels
        // texture Format
        int internal_format; // format of texture object
        int image_format; // format of loaded image
        // texture configuration
        int wrap_s; // wrapping mode on S axis
        int wrap_t; // wrapping mode on T axis
        int filter_min; // filtering mode if texture pixels < screen pixels
        int filter_max; // filtering mode if texture pixels > screen pixels
        //name
        std::string name;
        // constructor (sets default texture modes)
        Texture(int w, int h, uint8_t* data)
            : w(w), h(h), internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR)
        {
            glGenTextures(1, &id);

            // create Texture
            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, w, 0, image_format, GL_UNSIGNED_BYTE, data);
            // set Texture wrap and filter modes
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

            // unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void erase()
        {
            glDeleteTextures(1, &id);
        }
    
        // binds the texture as the current active GL_TEXTURE_2D texture object
        void bind() const
        {
            glBindTexture(GL_TEXTURE_2D, id);
        }
    };
}
#endif