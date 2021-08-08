#pragma once

class Window;
class Shader;
class VertexArray;
class Event;

typedef unsigned int GLenum;

class OpenGLRenderAPI
{
public:
	OpenGLRenderAPI() noexcept = default;
	OpenGLRenderAPI(Window& window);

	void Init(Window& window);
	void Deinit() noexcept;
	void OnEvent(const Event& event) const noexcept;

	static void SetClearColor(float r, float g, float b, float a) noexcept;
	static void EnableBlending(GLenum blendEquation) noexcept;
	static void Clear() noexcept;
	static void Render(const VertexArray& vertexArray, const Shader& shader) noexcept;

	const char* GetVendorName() const noexcept { return m_Vendor; }
	const char* GetRendererName() const noexcept { return m_Renderer; }
	const char* GetVersionName() const noexcept { return m_Version; }

private:
	const Window* m_Window;

	const char* m_Vendor;
	const char* m_Renderer;
	const char* m_Version;
};