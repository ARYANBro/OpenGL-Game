#include "VertexBuffer.h"

VertexBuffer::VertexBuffer() noexcept
{
	glCreateBuffers(1, &m_RendererID);
}

VertexBuffer::~VertexBuffer() noexcept
{
	glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SetData(void* data, std::size_t size) const noexcept
{
	glNamedBufferData(m_RendererID, size, data, GL_STATIC_DRAW);
}
