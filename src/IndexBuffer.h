//
// Created by karan on 04/07/20.
//

#ifndef SETUP_INDEXBUFFER_H
#define SETUP_INDEXBUFFER_H


class IndexBuffer {

private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void UnBind() const;

    inline unsigned int GetCount() const { return m_Count; }
};


#endif //SETUP_INDEXBUFFER_H
