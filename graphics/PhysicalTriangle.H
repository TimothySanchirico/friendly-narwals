#pragma once

#include "Drawable.H"
#include "PhysicalObject.H"
#include "PhysicalProperties.H"

struct PhysicalTriangle : Drawable<3>, PhysicalObject<PhysicalTriangle, Gravity>{
    PhysicalTriangle(std::shared_ptr<ShaderProgram> shaderProgram)
    : Drawable<3>(std::move(shaderProgram)) {
        offsets_ = {
                       0.5f,  0.5f, 0.0f,  // top right
                       0.5f, -0.5f, 0.0f,  // bottom right
                      -0.5f, -0.5f, 0.0f,  // bottom left
                    };
        indices_ = {0, 1, 2};
        _setVertexData();
        _setIndices();
        setAttributeArray(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    void draw() {
        shaderProgram_->setUniform("color", 0.3f, 0.1f, 0.8f, 1.0f);
        static_cast<Drawable<3>*>(this)->draw();
    }

    const std::string name() { return "Just a Triangle"; }
};
