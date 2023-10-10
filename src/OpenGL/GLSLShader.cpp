#include "GLSLShader.h"

#include <iostream>

GLSLShader& GLSLShader::use()
{
    glUseProgram(this->ID);
    return *this;
}

void GLSLShader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int s_vertex, s_fragment, g_shader;
    // vertex GLSLShader
    s_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(s_vertex, 1, &vertexSource, NULL);
    glCompileShader(s_vertex);
    check_compile_errors(s_vertex, "VERTEX");
    // fragment GLSLShader
    s_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(s_fragment, 1, &fragmentSource, NULL);
    glCompileShader(s_fragment);
    check_compile_errors(s_fragment, "FRAGMENT");
    // if geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr)
    {
        g_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(g_shader, 1, &geometrySource, NULL);
        glCompileShader(g_shader);
        check_compile_errors(g_shader, "GEOMETRY");
    }
    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, s_vertex);
    glAttachShader(this->ID, s_fragment);
    if (geometrySource != nullptr)
        glAttachShader(this->ID, g_shader);
    glLinkProgram(this->ID);
    check_compile_errors(this->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(s_vertex);
    glDeleteShader(s_fragment);
    if (geometrySource != nullptr)
        glDeleteShader(g_shader);
}

void GLSLShader::set_float(const char* name, float value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void GLSLShader::set_integer(const char* name, int value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void GLSLShader::set_vector2f(const char* name, float x, float y, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void GLSLShader::set_vector2f(const char* name, const glm::vec2& value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void GLSLShader::set_vector3f(const char* name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void GLSLShader::set_vector3f(const char* name, const glm::vec3& value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void GLSLShader::set_vector4f(const char* name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void GLSLShader::set_vector4f(const char* name, const glm::vec4& value, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void GLSLShader::set_matrix4(const char* name, const glm::mat4& matrix, bool use_shader)
{
    if (use_shader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}


void GLSLShader::check_compile_errors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::GLSLShader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
