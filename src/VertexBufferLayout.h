//
// Created by karan on 05/07/20.
//

#ifndef SETUP_VERTEXBUFFERLAYOUT_H
#define SETUP_VERTEXBUFFERLAYOUT_H

#include <GL/glew.h>
#include <vector>
#include "Renderer.h"

struct VertexBufferElement
{
    int           attribute;
    unsigned  int type;
    unsigned  int count;
    unsigned  char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT:          return 4 ;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }

    std::string toString() const
    {
        return "{attribute: "+std::to_string(attribute)+", type: "+std::to_string(type)+
        ",count: "+std::to_string(count)+",normalize: "+std::to_string(normalized)+"}";
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:

    VertexBufferLayout()
        :m_Stride(0){}

    template<typename T>
    void Push(int attribute,unsigned int count);
   

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const {return m_Stride;}
};




#endif //SETUP_VERTEXBUFFERLAYOUT_H
