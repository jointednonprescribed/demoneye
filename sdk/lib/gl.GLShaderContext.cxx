


/* Implementing: demoneye/gl.hpp: class GLShaderModule */
#include <demoneye/gl.hpp>



namespace demoneye
{
	GLShaderContext::GLShaderContext()
	{
		prog   = glCreateProgram();
		active = true;
	}

	GLShaderContext::GLShaderContext(size_t n, const GLShaderModule *modules, bool link=true):
		GLShaderContext()
	{
		AddShaders(n, modules, link);
	}

	GLShaderContext::~GLShaderContext()
	{
		glDeleteProgram(prog);
		prog   = 0;
		active = false;
	}

	void GLShaderContext::AddShader(const GLShaderModule &mod, bool link=false)
	{
		if (!active)
			return;

		glAttachShader(prog, mod);

		if (link)
			Link();
	}
	void GLShaderContext::AddShaders(size_t n, const GLShaderModule *modules, bool link=false)
	{
		if (!active)
			return;

		for (size_t i = 0; i < n; i++)
			glAttachShader(prog, modules[i]);

		if (link)
			Link();
	}

	void GLShaderContext::Link()
	{
		if (!active)
			return;

		glLinkProgram(prog);

		int statval = 0;
		glGetProgramiv(prog, GL_LINK_STATUS, &statval);

		constexpr size_t BUFSIZE = 512; // REFINE THIS APPROACH
		char buf[BUFSIZE+1] = {0};

		glGetProgramInfoLog(prog, BUFSIZE, NULL, buf);
		std::cout << "Shader context failed to link:\n" << buf << std::endl;
	}
}


