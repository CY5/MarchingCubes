//
// Created by karan on 05/07/20.
//

#ifndef SETUP_VERTEXARRAY_H
#define SETUP_VERTEXARRAY_H

#include "VertexBuffer.h"

class VertexBufferElement;

class VertexArray
{
private:
    unsigned int m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferElement& element);
    void Bind() const;
    void UnBind() const;
};


#endif //SETUP_VERTEXARRAY_H
