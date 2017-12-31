#pragma once

#include "Common/OpenGLHeaders.H"

#include <fstream>
#include <sstream>
#include <iostream>
#include <array>

struct Shader {

    Shader(const std::string& filename) {
        std::ifstream shaderFile(filename, std::ifstream::binary);
        std::stringstream shaderSource;
        shaderSource << shaderFile.rdbuf();
        shaderSource_ = std::move(shaderSource.str());
    }
    int compile() {
        if (shader_ != -1) {
            const char* source = shaderSource_.c_str();
            glShaderSource(shader_, 1, &source, nullptr);
            glCompileShader(shader_);

            char log[512];
            auto success = 0;
            glGetShaderiv(shader_, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader_, sizeof(log), nullptr, log);
                std::cerr << "ERROR::SHADER_COMPILATION::"
                          << log << std::endl;
            }
        } else {
            std::cerr << "ERROR:: Trying to compile unitialized shader" << std::endl;
            return -1;
        }
    }

    operator int() { return shader_; }

    ~Shader() {
        glDeleteShader(shader_);
    }

protected:
    int shader_ = -1;
    std::string shaderSource_;
};

struct VertexShader
: Shader {
    VertexShader(const std::string& filename)
    : Shader(filename) {
        shader_ = glCreateShader(GL_VERTEX_SHADER);
    }
};

struct FragmentShader
: Shader {
    FragmentShader(const std::string& filename)
    : Shader(filename) {
        shader_ = glCreateShader(GL_FRAGMENT_SHADER);
    }
};


struct ShaderProgram {

    template<typename... ShaderTypes>
    ShaderProgram(ShaderTypes&&... shaders) {
        program_ = glCreateProgram();

        std::array<Shader, sizeof...(ShaderTypes)> shaders_ = { shaders... };
        for (auto& shader : shaders_) {
            shader.compile();
            glAttachShader(program_, shader);
        }

        glLinkProgram(program_);

        char log[512]; 
        auto success = 0;
        glGetProgramiv(program_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program_, 512, nullptr, log);
            std::cerr << "PROGRAM LINKING FAILED::"
                      << log << std::endl;
        } else {
            std::cout << "LOG::Successfully linked shaders" << std::endl;
        }
    }

    template <typename... Args>
    void setUniform(const std::string& name, Args&&... args) {
        auto uniformLocation = glGetUniformLocation(program_, name.c_str());
        glUseProgram(program_);
        if constexpr (sizeof...(args) == 4) {
            glUniform4f(uniformLocation, std::forward<Args>(args)...);
        } else if constexpr (sizeof...(args) == 3) {
            glUniform3f(uniformLocation, std::forward<Args>(args)...);
        }
    }

    operator int() { return program_; }

protected:
    int program_;
};
