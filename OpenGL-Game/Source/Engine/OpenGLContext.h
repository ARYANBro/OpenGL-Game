#pragma once

class Window;

class OpenGLContext
{
public:
	OpenGLContext(const Window& window);

	const char* GetVendorName() const noexcept { return m_Vendor; }
	const char* GetRendererName() const noexcept { return m_Renderer; }
	const char* GetVersionName() const noexcept { return m_Version; }

private:
	const Window& m_Window;

	const char* m_Vendor;
	const char* m_Renderer;
	const char* m_Version;
};