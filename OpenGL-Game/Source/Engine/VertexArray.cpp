#include "VertexArray.h"

#include "VertexBuffer.h"

VertexArray::VertexArray() noexcept
	: m_NumVertexBuffers(0), m_NumIndices()
{
	glCreateVertexArrays(1, &m_RendererID);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, std::size_t stride, const BufferLayout& layout) noexcept
{
	glVertexArrayVertexBuffer(m_RendererID, m_NumVertexBuffers, vertexBuffer->m_RendererID, 0, stride);

	for (const AttributeLayout& attribLayout : layout)
	{
		glEnableVertexArrayAttrib(m_RendererID, attribLayout.Index);

		glVertexArrayAttribFormat(m_RendererID,
			attribLayout.Index,
			attribLayout.NumComponents,
			attribLayout.DataType,
			attribLayout.Normalized ? GL_TRUE : GL_FALSE,
			attribLayout.RelativeOffset
		);

		glVertexArrayAttribBinding(m_RendererID, attribLayout.Index, m_NumVertexBuffers);
	}

	m_NumVertexBuffers++;
	m_VertexBuffers.push_front(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<VertexBuffer>& indexBuffer, std::size_t count) noexcept
{
	glVertexArrayElementBuffer(m_RendererID, indexBuffer->m_RendererID);
	m_NumIndices = count;
	m_IndexBuffer = indexBuffer;
}

void VertexArray::Bind() const noexcept
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const noexcept
{
	glBindVertexArray(0);
}