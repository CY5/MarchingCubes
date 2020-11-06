//
// Created by karan on 04/07/20.
//

#include "Renderer.h"

#include <iostream>


void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool  GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << " [OpenGl Error] (" << error << ")"
                  << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
void ErrorCallback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}


void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Renderer::Draw(const VertexArray &va, const Shader &shader, const GLint count) const
{
    shader.Bind();
    va.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));

}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

