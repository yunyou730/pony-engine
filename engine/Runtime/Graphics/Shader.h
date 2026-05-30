#pragma once

#include "Core/GLCompat.h"

namespace pony {
namespace gfx {

class Shader {
public:
    Shader(const char *vertexSrc, const char *fragmentSrc);
    ~Shader();
    
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;
    
    bool isValid() const { return _program != 0; }
    GLuint program() const { return _program; }
    
    void use() const;
    
    void setFloat(const char *name, float v);
    void setVec2(const char *name, float x, float y);
    void setVec3(const char *name, float x, float y, float z);
    void setVec4(const char *name, float x, float y, float z, float w);
    void setMat4(const char *name, const float *mat);
    
private:
    GLuint _program = 0;
    
    GLint loc(const char *name) const;
    static GLuint compile(GLenum type, const char *source);
};

}
}
