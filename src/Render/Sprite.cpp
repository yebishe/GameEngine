#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include "glad/glad.h"

#include "Sprite.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "glm/ext/matrix_transform.hpp"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


namespace Render {

    Sprite::Sprite(const std::shared_ptr<Texture2D> p_Texture, const std::string& initialSubTexture2D, const std::shared_ptr<ShaderProgram> p_ShaderProgram,
        const glm::vec2 &position, const glm::vec2 &size, const float rotation) :
                                m_pTexture(std::move(p_Texture)),
                                m_pShaderProgram(std::move(p_ShaderProgram)),
                                m_Position(position),
                                m_Size(size),
                                m_Rotation(rotation)
    {
        const GLfloat vertexCoords[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
        };

        const Texture2D::SubTexture2D& p_SubTexture = p_Texture->getSubTexture(std::move(initialSubTexture2D));

        const GLfloat textureCoords[] = {
            p_SubTexture.leftBottomUV.x, p_SubTexture.leftBottomUV.y,
            p_SubTexture.leftBottomUV.x, p_SubTexture.rightTopUV.y,
            p_SubTexture.rightTopUV.x, p_SubTexture.rightTopUV.y,
            p_SubTexture.rightTopUV.x, p_SubTexture.leftBottomUV.y,
        };

        const GLuint indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        // VAO
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // vertex coords VBO
        m_vertexCoordsBuffer.init(vertexCoords, 8 * sizeof(GLfloat));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // texture coords VBO
        m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        // index buffer EBO
        m_indexBuffer.init(indices, 6 * sizeof(GLuint));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }

    Sprite::~Sprite() {

        glDeleteVertexArrays(1, &m_VAO);

    }

    void Sprite::render() const {
        m_pShaderProgram->use();

        glm::mat4 modelMat(1.f);

        modelMat = glm::translate(modelMat, glm::vec3(m_Position, 0.0f));
        modelMat = glm::translate(modelMat, glm::vec3(0.5f * m_Size.x, 0.5f * m_Size.y, 0.0f));
        modelMat = glm::rotate(modelMat, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMat = glm::translate(modelMat, glm::vec3(-0.5f * m_Size.x, -0.5f * m_Size.y, 0.0f));
        modelMat = glm::scale(modelMat, glm::vec3(m_Size, 1.0f));

        glBindVertexArray(m_VAO);
        m_pShaderProgram->setMatrix4("modelMatrix", modelMat);

        glActiveTexture(GL_TEXTURE0);
        m_pTexture->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void Sprite::setPosition(glm::vec2 position) {
        m_Position = position;
    }

    void Sprite::setSize(glm::vec2 size) {
        m_Size = size;
    }

    void Sprite::setRotation(float rotation) {
        m_Rotation = rotation;
    }
}

