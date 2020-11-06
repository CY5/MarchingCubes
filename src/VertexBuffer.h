//
// Created by karan on 04/07/20.
//

#ifndef SETUP_VERTEXBUFFER_H
#define SETUP_VERTEXBUFFER_H

class VertexBuffer {

private:
    unsigned int m_RendererID;

public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

};


#endif //SETUP_VERTEXBUFFER_H
