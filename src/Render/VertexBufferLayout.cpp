#include "VertexBufferLayout.h"

namespace Render {
    VertexBufferLayout::VertexBufferLayout() : m_stride(0){

    }

    void VertexBufferLayout::reserveElements(const size_t count) {
        m_layoutElements.reserve(count);
    }


    void VertexBufferLayout::addElementLayoutFloat(const unsigned int count, const bool normalized) {
        m_layoutElements.push_back( {count, GL_FLOAT, normalized, count * sizeof(GLfloat) } );
        m_stride += m_layoutElements.back().size;
    }

}
