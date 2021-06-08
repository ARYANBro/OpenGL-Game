#pragma once

#include <glad/glad.h>

#include <cstdint>
#include <forward_list>
#include <memory>

class VertexBuffer;

struct AttributeLayout
{
	int Index;
	int NumComponents;
	GLenum DataType;
	bool Normalized;
	std::size_t RelativeOffset;
};

using BufferLayout = std::forward_list<AttributeLayout>;

class VertexArray
{
public:

public:
	VertexArray() noexcept;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, std::size_t stride, const BufferLayout& layout) noexcept;
	void SetIndexBuffer(const std::shared_ptr<VertexBuffer>& indexBuffer, std::size_t count) noexcept;

	void Bind() const noexcept;
	void Unbind() const noexcept;

	std::size_t GetNumIndices() const noexcept { return m_NumIndices; }

private:
	friend class OpenGLRenderAPI;

private:
	std::uint_fast32_t m_RendererID;

	std::forward_list<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<VertexBuffer> m_IndexBuffer;

	std::size_t m_NumVertexBuffers;
	std::size_t m_NumIndices;
};