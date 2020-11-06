//
// Created by karan on 05/07/20.
//

#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferElement& element)
{
    Bind();
    vb.Bind();
    GLCall(glEnableVertexAttribArray(element.attribute));
    GLCall(glVertexAttribPointer(element.attribute, element.count, element.type,
             element.normalized, 0,0));
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
    GLCall(glBindVertexArray(0));
}
