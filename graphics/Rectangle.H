#pragma once

#include "Drawable.H"

struct Rectangle : Drawable<4> {
    Rectangle(std::shared_ptr<ShaderProgram> shaderProgram)
    : Drawable<4>(std::move(shaderProgram)) {
        offsets_ = {
                       -0.5f, 0.5f, 0.0f,  // top left
                       0.5f,  0.5f, 0.0f,  // top right
                       0.5f, -0.5f, 0.0f,  // bottom right
                      -0.5f, -0.5f, 0.0f,  // bottom left
                    };
        indices_ = {0, 1, 2, 0, 2, 3};
        _setVertexData();
        _setIndices();
        setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    void draw() {
        shaderProgram_->setUniform("color", 0.5f, 0.5f, 0.5f, 1.0f);
        static_cast<Drawable<4>*>(this)->draw();
    }
};

