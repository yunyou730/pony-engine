#include "Shader.h"
#include "GLCheck.h"
#include "Core/Log.h"

namespace pony {
    Shader::Shader(const char *vertexSrc, const char *fragmentSrc) {
        GLuint vert = compile(GL_VERTEX_SHADER, vertexSrc);
        GLuint frag = compile(GL_FRAGMENT_SHADER, fragmentSrc);
        if (!vert || !frag) {
            if (vert) glDeleteShader(vert);
            if (frag) glDeleteShader(frag);
            return;
        }

        _program = glCreateProgram();
        GL_CHECK(glAttachShader(_program, vert));
        GL_CHECK(glAttachShader(_program, frag));
        GL_CHECK(glLinkProgram(_program));

        GLint success;
        glGetProgramiv(_program, GL_LINK_STATUS, &success);
        if (!success) {
            char info[512];
            glGetProgramInfoLog(_program, sizeof(info), nullptr, info);
            pony::Log::w("Shader link error: %s", info);
            glDeleteProgram(_program);
            _program = 0;
        }

        glDeleteShader(vert);
        glDeleteShader(frag);
    }

    Shader::~Shader() {
        pony::Log::i("Shader::~Shader()");
        if (_program) {
            GL_CHECK(glDeleteProgram(_program));
            _program = 0;
        }
    }

    GLuint Shader::compile(GLenum type, const char *source) {
        GLuint shader = glCreateShader(type);
        GL_CHECK(glShaderSource(shader, 1, &source, nullptr));
        GL_CHECK(glCompileShader(shader));

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info[512];
            glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
            pony::Log::w("Shader compile error: %s", info);
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    void Shader::use() const {
        glUseProgram(_program);
    }

    GLint Shader::loc(const char *name) const {
        return glGetUniformLocation(_program, name);
    }

    void Shader::setFloat(const char *name, float v) {
        glUniform1f(loc(name), v);
    }

    void Shader::setVec2(const char *name, float x, float y) {
        glUniform2f(loc(name), x, y);
    }

    void Shader::setVec3(const char *name, float x, float y, float z) {
        glUniform3f(loc(name), x, y, z);
    }

    void Shader::setVec4(const char *name, float x, float y, float z, float w) {
        glUniform4f(loc(name), x, y, z, w);
    }

    void Shader::setMat4(const char *name, const float *mat) {
        glUniformMatrix4fv(loc(name), 1, GL_FALSE, mat);
    }

}
