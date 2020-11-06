//
// Created by karan on 04/07/20.
//

#ifndef SETUP_RENDERER_H
#define SETUP_RENDERER_H

#include <GL/glew.h>
#include <csignal>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError(); \
    x; \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


bool  GLLogCall(const char* function, const char* file, int line);
void GLClearError();
void ErrorCallback(int, const char* err_str);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Draw(const VertexArray& va, const Shader& shader,  const GLint count) const;
};


#endif //SETUP_RENDERER_H
