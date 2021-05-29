#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main()
{
	int glfwInitResult = glfwInit();
	if (glfwInitResult == GLFW_FALSE) throw std::runtime_error("Could not initialize glfw");

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	GLFWwindow* window = glfwCreateWindow(1280, 800, "OpenGL-Game", nullptr, nullptr);

	if (window == nullptr) throw std::runtime_error("Could not create a window");

	glfwMakeContextCurrent(window);

	int gladInitResult = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	if (gladInitResult == 0) throw std::runtime_error("Could not initialize glad");

	std::cout << "OpenGL Info:";
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				std::cout << "OpenGL Severity High: " << message << std::endl;
				break;
			
			case GL_DEBUG_SEVERITY_MEDIUM:
				std::cout << "OpenGL Severity Medium: " << message << std::endl;
				break;
			
			case GL_DEBUG_SEVERITY_LOW:
				std::cout << "OpenGL Severity Low: " << message << std::endl;
				break;

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				break;
		}

	}, nullptr);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}