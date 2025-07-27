


/* Implementing: demoneye/gl.hpp: class GLShaderModule */
#include <demoneye/gl.hpp>



namespace demoneye
{
	GLShaderModule::GLShaderModule(const char *module_name, mod_type module_type)
	{
		modname = module_name;
		mod = glCreateShader(module_type);
	}
	GLShaderModule::GLShaderModule(const char *module_name, mod_type module_type, const char *src_path):
		GLShaderModule(module_name, module_type)
	{
		AddSource(src_path);
	}
	GLShaderModule::GLShaderModule(const char *module_name, mod_type module_type, const std::string &src_path):
		GLShaderModule(module_name, module_type)
	{
		AddSource(src_path);
	}

	GLShaderModule::~GLShaderModule()
	{
		glDeleteShader(mod);
	}


	void GLShaderModule::Compile() const
	{
		glCompileShader(mod);

		int statval = 0;
		glGetShaderiv(mod, GL_COMPILE_STATUS, &statval);

		constexpr size_t BUFSIZE = 512; // REFINE THIS APPROACH
		char buf[BUFSIZE+1] = {0};

		glGetShaderInfoLog(mod, BUFSIZE, NULL, buf);
		std::cout << "Shader module '" << modname << "' failed to compile:\n" << buf << std::endl;
	}

	void GLShaderModule::AddSource(const char *src_path, bool compile=true)
	{
		using std::fstream, std::ios, std::stringstream;

		constexpr size_t BUFSIZE = 512;

		char buf[BUFSIZE+1] = {0};

		const size_t l = filesize(src_path);

		fstream src(src_path, ios::in);
		if (!src.is_open())
			throw runtime_error("Failed to open the specified file.");

		stringstream ss;
		
		while (!src.eof()) {
			src.getline(buf, BUFSIZE);
			ss << buf << std::endl;
		}

		AddSourceRaw(ss.str(), compile);
		ss.str("");
	}
	void GLShaderModule::AddSource(const std::string &src_path, bool compile=true)
	{
		using std::fstream, std::ios, std::stringstream;

		constexpr size_t BUFSIZE = 512;

		char buf[BUFSIZE+1] = {0};

		const size_t l = filesize(src_path);

		fstream src(src_path, ios::in);
		if (!src.is_open())
			throw runtime_error("Failed to open the specified file.");

		stringstream ss;
		
		while (!src.eof()) {
			src.getline(buf, BUFSIZE);
			ss << buf << std::endl;
		}

		AddSourceRaw(ss.str(), compile);
		ss.str("");
	}
	void GLShaderModule::AddSourceRaw(const std::string &src, bool compile=true)
	{
		AddSourceRaw(src.length(), src.data(), compile);
	}
	void GLShaderModule::AddSourceRaw(size_t n, const char *src, bool compile=true)
	{
		glShaderSource(mod, 1, &src, NULL);
		if (compile)
			Compile();
	}
}


