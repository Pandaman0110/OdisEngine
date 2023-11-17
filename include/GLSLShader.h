#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OdisEngine
{
    // General purpose shader object. Compiles from file, generates
    // compile/link-time error messages and hosts several utility 
    // functions for easy management.

    class GLSLShader
    {
    public:
        // state
        unsigned int ID;
        // constructor
        GLSLShader() { }
        // sets the current shader as active
        GLSLShader& use();
        // compiles the shader from given source code
        void    compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr); // note: geometry source code is optional 
        // utility functions
        void    set_float(const char* name, float value, bool use_shader = false);
        void    set_integer(const char* name, int value, bool use_shader = false);
        void    set_vector2f(const char* name, float x, float y, bool use_shader = false);
        void    set_vector2f(const char* name, const glm::vec2& value, bool use_shader = false);
        void    set_vector3f(const char* name, float x, float y, float z, bool use_shader = false);
        void    set_vector3f(const char* name, const glm::vec3& value, bool use_shader = false);
        void    set_vector4f(const char* name, float x, float y, float z, float w, bool use_shader = false);
        void    set_vector4f(const char* name, const glm::vec4& value, bool use_shader = false);
        void    set_matrix4(const char* name, const glm::mat4& matrix, bool use_shader = false);
    private:
        // checks if compilation or linking failed and if so, print the error logs
        void    check_compile_errors(unsigned int object, std::string type);
    };
}

#endif