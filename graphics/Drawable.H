#pragma once

#include "Common/OpenGLHeaders.H"
#include "DrawableFactory.H"
#include "Shaders.H"

#include <string>
#include <memory>
#include <vector>
#include <array>

using Coordinate = std::array<GLfloat, 3>;

template <size_t VertexCt, size_t VertexSize=3>
struct Drawable
: RegisteredDrawable<Drawable<VertexCt, VertexSize>> {
    Drawable(std::shared_ptr<ShaderProgram> shaderProgram)
    : shaderProgram_(shaderProgram) {
        glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);
        glBindVertexArray(VAO_);
    }

    static_assert(VertexSize >= 3, "Not enough vertex Data");
    static constexpr size_t VertexDataLen = VertexCt*VertexSize;

    static constexpr size_t getVertexCt()   { return VertexCt; }
    static constexpr size_t getVertexSize() { return VertexSize; }

    template <typename... Args>
    void setAttributeArray(Args&&... args) {
        glVertexAttribPointer(std::forward<Args>(args)...);
        glEnableVertexAttribArray(0);
    }

    void setVertexOffsets(std::array<GLfloat, VertexCt * 3>& data) {
        std::memcpy(offsets_.data(), data.data(), VertexDataLen*sizeof(GLfloat));
        updatePositionData();
        _setVertexData();
    }

    void setVertexData(std::array<GLfloat, VertexDataLen>& data) {
        std::memcpy(vertices_.data(), data.data(), VertexDataLen*sizeof(GLfloat));
        _setVertexData();
    }

    void setIndices(std::vector<GLuint>&& indices) {
        indices_ = indices;
        _setIndices();
    }

    void scale(GLfloat factor) {
        for (auto& v : offsets_) v *= factor;
    }

    void draw() {
        _setVertexData();
        glUseProgram(*shaderProgram_);
        glBindVertexArray(VAO_);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    }

    void setPosition(Coordinate&& coord) {
        position_ = coord;
    }
    void setPosition(Coordinate& coord) {
        position_ = coord;
    }

    template<typename T>
    void move(std::array<T, getVertexSize()> delta) {
        position_[0] += delta[0];
        position_[1] += delta[1];
        position_[2] += delta[2];

        glBindVertexArray(VAO_);
        _setVertexData();
    }

    constexpr static std::string name() { return "Drawable"; }


protected:
    void updatePositionData() {
        for (size_t i = 0; i < getVertexCt(); ++i) {
            auto point  = vertices_.data() + i * getVertexSize();
            auto offset = offsets_.data()  + i * 3;
            point[0]    = position_[0] + offset[0];
            point[1]    = position_[1] + offset[1];
            point[2]    = position_[2] + offset[2];
        }
    }

    void _setVertexData() {
        updatePositionData();
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, VertexDataLen * sizeof(GLfloat),
                     vertices_.data(), GL_STATIC_DRAW);
    }
    
    void _setIndices() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint),
                     indices_.data(), GL_STATIC_DRAW);
    }
protected:
    std::shared_ptr<ShaderProgram> shaderProgram_;
    GLuint VBO_;
    GLuint VAO_;
    GLuint EBO_;
    std::array<GLfloat, VertexCt*3> offsets_;
    std::array<GLfloat, VertexDataLen> vertices_;
    std::vector<GLuint> indices_;
    Coordinate position_ = {0.0f, 0.0f, 0.0f};
};

