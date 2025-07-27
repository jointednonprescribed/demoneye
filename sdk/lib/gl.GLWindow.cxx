


/* Implementing: demoneye/gl.hpp: class GLWindow */
#include <demoneye/gl.hpp>



namespace demoneye
{
	GLWindow::GLWindow()
	{
		handle  = NULL;
		w = h   = 0;
		title   = NULL;
	}
	GLWindow::GLWindow(int width, int height, const char *title, GLFWmonitor *fs_mon, GLFWwindow *share, bool start)
	{
		handle = NULL;
		if (start)
			Initialize(width, height, title, fs_mon, share);
		else {
			this->w       = width;
			this->h       = height;
			this->title   = title;
			this->monitor = fs_mon;
			this->share   = share;
		}
	}
	GLWindow::~GLWindow()
	{
		Destroy();
	}

	bool GLWindow::Initialize()
	{
		if (handle) {
			std::cout << "GLWindow at " << handle << " is already initialized." << std::endl;
			return true;
		} else {
			handle = glfwCreateWindow(w, h, title, monitor, share);
			if (handle) {
				std::cout << "GLWindow initialized at " << handle << '.' << std::endl;
				return true;
			} else {
				std::cout << "GLWindow did not initialize." << std::endl;
				return false;
			}
		}
	}
	bool GLWindow::Initialize(int width, int height)
	{
		if (IsInitialized())
			return true;
		else {
			this->w       = width;
			this->h       = height;
			return Initialize();
		}
	}
	bool GLWindow::Initialize(int width, int height, const char *title)
	{
		if (IsInitialized())
			return true;
		else {
			this->w       = width;
			this->h       = height;
			this->title   = title;
			return Initialize();
		}
	}
	bool GLWindow::Initialize(int width, int height, const char *title, GLFWmonitor *fs_mon, GLFWwindow *share)
	{
		if (IsInitialized())
			return true;
		else {
			this->w       = width;
			this->h       = height;
			this->title   = title;
			this->monitor = fs_mon;
			this->share   = share;
			return Initialize();
		}
	}

	bool GLWindow::Reinitialize()
	{
		if (handle) {
			glfwDestroyWindow(handle);
		}
		handle = glfwCreateWindow(w, h, title, monitor, share);
		return handle != NULL;
	}
	bool GLWindow::Reinitialize(int width, int height)
	{
		this->w = width;
		this->h = height;
		return Reinitialize();
	}
	bool GLWindow::Reinitialize(int width, int height, const char *title)
	{
		this->w     = width;
		this->h     = height;
		this->title = title;
		return Reinitialize();
	}
	bool GLWindow::Reinitialize(
		int          width,
		int          height,
		const char  *title,
		GLFWmonitor *fs_monitor,
		GLFWwindow  *share
	){
		this->w       = width;
		this->h       = height;
		this->title   = title;
		this->monitor = fs_monitor;
		this->share   = share;
		return Reinitialize();
	}

	void GLWindow::Destroy()
	{
		if (handle) {
			glfwDestroyWindow(handle);
			handle  = NULL;
		}
		title = NULL;
		w = h = 0;
	}

	bool GLWindow::IsInitialized() const
	{
		return handle != NULL;
	}

	int GLWindow::Width()  const
	{
		return w;
	}
	int GLWindow::Height() const
	{
		return h;
	}
	const char* GLWindow::Title() const
	{
		return title;
	}
	GLFWmonitor* GLWindow::Monitor() const
	{
		return monitor;
	}
	GLFWwindow* GLWindow::SharingWith() const
	{
		return share;
	}
	GLFWwindow* GLWindow::Get() const
	{
		return handle;
	}

	GLWindow::operator GLFWwindow*() const
	{
		return handle;
	}

	void GLWindow::MakeViewport()
	{
		MakeViewport(0, 0);
	}
	void GLWindow::MakeViewport(int x, int y)
	{
		glViewport(x, y, w, h);
	}

	GLWindow& GLWindow::operator =(const GLWindow &rhv)
	{
		Destroy();
		w       = rhv.w;
		h       = rhv.h;
		title   = rhv.title;
		monitor = rhv.monitor;
		share   = rhv.share;
		handle  = rhv.handle;
		return *this;
	}
	bool GLWindow::operator ==(const GLWindow &rhv)
	{
		return handle == rhv.handle;
	}
	bool GLWindow::operator !=(const GLWindow &rhv)
	{
		return handle != rhv.handle;
	}

	int GLWindow::MainLoop()
	{
		if (handle) {
			while (!glfwWindowShouldClose(handle)) {
				glfwSwapBuffers(handle);
				glfwPollEvents();
			}
			return 0;
		} else
			return 1;
	}
	int GLWindow::MainLoop(predicate run_each)
	{
		if (handle) {
			bool nobreak = true;
			while (!glfwWindowShouldClose(handle) && nobreak) {
				glfwSwapBuffers(handle);
				glfwPollEvents();
				nobreak = run_each();
			}
			return 0;
		} else
			return 1;
	}

	bool GLWindow::MakeCurrentContext() const
	{
		if (handle) {
			glfwMakeContextCurrent(handle);
			return true;
		} else
			return false;
	}
}


