#pragma once

class Window;

class OpenGLRenderAPI
{
public:
	OpenGLRenderAPI(const Window& window);

	void SetClearColor(float r, float g, float b, float a) const noexcept;
	void Clear() const noexcept;

	const char* GetVendorName() const noexcept { return m_Vendor; }
	const char* GetRendererName() const noexcept { return m_Renderer; }
	const char* GetVersionName() const noexcept { return m_Version; }

private:
	const Window& m_Window;

	const char* m_Vendor;
	const char* m_Renderer;
	const char* m_Version;
};