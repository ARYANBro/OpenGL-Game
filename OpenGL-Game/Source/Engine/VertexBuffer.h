#pragma once

#include <glad/glad.h>

#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer() noexcept;
	~VertexBuffer() noexcept;

	void SetData(void* data, std::size_t size) const noexcept;

private:
	friend class VertexArray;
private:
	GLuint m_RendererID;
};
