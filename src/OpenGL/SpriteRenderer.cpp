#include "SpriteRenderer.h"

#include "glad/gl.h"

SpriteRenderer::SpriteRenderer(GLSLShader& shader) : shader(shader)
{
    // configure VAO/VBO
    unsigned int vbo;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quad_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quad_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quad_vao);
}

void SpriteRenderer::draw_texture(Texture2D& texture, vec2 position)
{
    // prepare transformations
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * texture.width, 0.5f * texture.height, 0.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * texture.width, -0.5f * texture.height, 0.0f));

    model = glm::scale(model, glm::vec3(texture.width, texture.height, 1.0f));

    shader.set_matrix4("model", model);
    shader.set_vector3f("sprite_color", vec3(1.0f, 1.0f, 1.0f));

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::draw_texture(Texture2D& texture, vec2 position, float rotation)
{
    // prepare transformations
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * texture.width, 0.5f * texture.height, 0.0f));
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * texture.width, -0.5f * texture.height, 0.0f));

    model = glm::scale(model, glm::vec3(texture.width, texture.height, 1.0f));

    shader.set_matrix4("model", model);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->quad_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
