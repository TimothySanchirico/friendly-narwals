#pragma once

#include "Common/OpenGLHeaders.H"
#include "Drawable.H"

#include <string>

template <size_t VertexCt, size_t VertexSize=5>
struct TexturedDrawable
: Drawable<VertexCt, VertexSize> {

    TexturedDrawable() : Drawable<VertexCt> {

    }

    constexpr static std::string name() { return "TexturedDrawable"; }
// From Drawable
protected:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    std::array<GLfloat, VertexSize*VertexCt> vertices_;
    std::vector<GLuint> indices_;

protected:
    GLuint texture_;
};

