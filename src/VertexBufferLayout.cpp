//
// Created by karan on 05/07/20.
//
#include "VertexBufferLayout.h"

template<>
void VertexBufferLayout::Push<float>(int attribute,unsigned int count)
{
    m_Elements.push_back({attribute,GL_FLOAT,count, GL_FALSE});
   // m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
   m_Stride = 0.0;
}

template<>
void VertexBufferLayout::Push<unsigned int>(int attribute,unsigned int count)
{
    m_Elements.push_back({attribute,GL_UNSIGNED_INT,count, GL_FALSE});
    //m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    m_Stride = 0.0;
}

template<>
void VertexBufferLayout::Push<unsigned char>(int attribute,unsigned int count)
{
    m_Elements.push_back({attribute,GL_UNSIGNED_BYTE,count, GL_TRUE});
    //m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    m_Stride = 0.0;
}

