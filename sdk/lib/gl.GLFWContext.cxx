


/* Implementing: demoneye/gl.hpp: class GLFWContext */
#include <demoneye/gl.hpp>



namespace demoneye
{
	GLFWContext::GLFWContext()
	{
		running = glfwInit();
		WindowHintVersion(DE_OPENGL_VERSION_MAJOR, DE_OPENGL_VERSION_MINOR);
	}
	GLFWContext::~GLFWContext()
	{
		Terminate();
	}

	bool GLFWContext::Initialize()
	{
		if (running)
			return true;
		else {
			running = glfwInit();
			return running;
		}
	}
	void GLFWContext::Terminate()
	{
		if (running) {
			glfwTerminate();
			running = false;
		}
	}

	bool GLFWContext::IsInitialized() const
	{
		return running;
	}


	void GLFWContext::WindowHintVersion(int major, int minor)
	{
		glfwWindowHint(GLFW_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_VERSION_MINOR, minor);
	}
	void GLFWContext::WindowHintOpenGLProfile(int profile)
	{
		glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	}
}


