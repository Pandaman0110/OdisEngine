#include "ShapeRenderer.h"

#include <array>

#include <glad/gl.h>

#include "Mesh.h"

using namespace OdisEngine;

ShapeRenderer::ShapeRenderer(GLSLShader& shader) : shader(shader)
{
    std::vector<float> rect_vertices
    {
         0.5f, 0.5f, 0.0f,
         0.5f,-0.5f, 0.0f,
        -0.5f,-0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
    };

    std::vector<unsigned int> rect_indices
    {
        0, 1, 3,
        1, 2, 3
    };

    //Mesh mesh { rect_vertices, rect_indices };

    unsigned int vbo, ebo;

    //ready 
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(this->vao);

    //do vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, rect_vertices.size() * sizeof(float), rect_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, rect_indices.size() * sizeof(float), rect_indices.data(), GL_STATIC_DRAW);

    //do vao
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //clean
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ShapeRenderer::~ShapeRenderer()
{
    glDeleteVertexArrays(1, &this->vao);
}

void ShapeRenderer::draw_rect(vec2 pos, vec2 size, float rotation, ColorRGB color, float alpha)
{

    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));

    shader.set_matrix4("model", model);
    shader.set_vector4f("shape_color", vec4(color.r, color.g, color.b, alpha));

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}