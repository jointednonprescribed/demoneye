
#ifndef __DEMONEYE__GL_H
#define __DEMONEYE__GL_H 1

#include "setup.hpp"
//#include "resmgr.hpp"
//#include "mathio.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
// SDL 3.2
#include <SDL3/SDL.h>
// GLFW 3.3 & GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>



namespace demoneye
{
	class GLAD
	{
	public:
		static bool LoadGLFWLoader();
		static bool LoadGLLoader(GLADloadproc loader);
	};

	typedef class GLShaderModule
	{
	private:
		unsigned int mod;
		const char  *modname;

	public:
		typedef unsigned int mod_type;

		GLShaderModule(const char *module_name, mod_type module_type);
		GLShaderModule(const char *module_name, mod_type module_type, const char *src_path);
		GLShaderModule(const char *module_name, mod_type module_type, const std::string &src_path);
		template <size_t _N>
		GLShaderModule(const char *module_name, mod_type module_type, const char *src_paths[_N]):
			GLShaderModule(module_name, module_type)
		{
			for (char char *path : src_paths)
				AddSource(path);
		}
		template <size_t _N>
		GLShaderModule(const char *module_name, mod_type module_type, const std::string src_paths[_N]):
			GLShaderModule(module_name, module_type)
		{
			for (char char *path : src_paths)
				AddSource(path);
		}

		~GLShaderModule();

		operator unsigned int() const;


		unsigned int Get() const;
		const char  *Name() const;

		void Compile() const noexcept(false);

		void AddSource(const char *src_path, bool compile=true) noexcept(false);
		void AddSource(const std::string &src_path, bool compile=true) noexcept(false);
		template <size_t _N>
		void AddSource(const char *src_paths[_N], bool compile=true) noexcept(false)
		{
			for (const char *path : src_paths)
				AddSource(path);
			if (compile)
				Compile();
		}
		template <size_t _N>
		void AddSource(const std::string src_paths[_N], bool compile=true) noexcept(false)
		{
			for (const std::string path : src_paths)
				AddSource(path);
			if (compile)
				Compile();
		}

		void AddSourceRaw(const std::string &src, bool compile=true) noexcept(false);
		void AddSourceRaw(size_t n, const char *src, bool compile=true) noexcept(false);
		template <size_t _N>
		void AddSourceRaw(const char src[_N], bool compile=true) noexcept(false)
		{
			AddSourceRaw(_N - 1, src);
			if (compile)
				Compile();
		}
	} GLShaderMod;

	typedef class GLShaderContext
	{
		unsigned int prog;
		bool         active;
	public:

		GLShaderContext();
		GLShaderContext(size_t n, const GLShaderModule *modules, bool link=true);

		~GLShaderContext();

		void AddShader(const GLShaderModule &module, bool link=false);
		void AddShaders(size_t n, const GLShaderModule *modules, bool link=false);
		void Link();
	} GLShaderCtxt;

	class GLFWContext: public GLShaderContext
	{
	private:
		bool running;

		static void WindowHintVersion(int major, int minor);
		static void WindowHintOpenGLProfile(int opengl_prof);

	public:
		GLFWContext();
		~GLFWContext();

		bool Initialize();
		void Terminate();

		bool IsInitialized() const;
	};
	bool operator !(const GLFWContext &rhv);

	class GLWindow
	{
	private:
		GLFWwindow  *handle;
		GLFWmonitor *monitor;
		GLFWwindow  *share;
		int          w, h;
		const char  *title;

	public:
		GLWindow();
		GLWindow(int width, int height, const char *title, GLFWmonitor *fs_monitor=NULL, GLFWwindow *share=NULL,
				bool start=true);
		~GLWindow();

		bool Initialize();
		bool Initialize(int width, int height);
		bool Initialize(int width, int height, const char *title);
		bool Initialize(
			int          width,
			int          height,
			const char  *title,
			GLFWmonitor *fs_monitor,
			GLFWwindow  *share
		);

		bool Reinitialize();
		bool Reinitialize(int width, int height);
		bool Reinitialize(int width, int height, const char *title);
		bool Reinitialize(
			int          width,
			int          height,
			const char  *title,
			GLFWmonitor *fs_monitor=NULL,
			GLFWwindow  *share=NULL
		);

		void Destroy();

		bool IsInitialized()   const;

		int          Width()   const;
		int          Height()  const;
		const char*  Title()   const;
		GLFWmonitor* Monitor() const;
		GLFWwindow*  SharingWith() const;
		GLFWwindow*  Get()     const;

		operator GLFWwindow*() const;

		void MakeViewport();
		void MakeViewport(int x, int y);

		GLWindow& operator =(const GLWindow &rhv);
		bool      operator ==(const GLWindow &rhv);
		bool      operator !=(const GLWindow &rhv);

		virtual int MainLoop();
		virtual int MainLoop(predicate inject_code);

		bool MakeCurrentContext() const;
	};
	bool operator !(const GLWindow &rhv);

	class VertexBuffer
	{

	};
	class VertexArray
	{
		
	};
}



#endif // __DEMONEYE__GL_H
