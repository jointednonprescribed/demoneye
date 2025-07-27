



/* Implementing: demoneye/gl.hpp: class GLAD */
#include <demoneye/gl.hpp>



namespace demoneye
{
	bool GLAD::LoadGLFWLoader()
	{
		return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}
	bool GLAD::LoadGLLoader(GLADloadproc loader)
	{
		return gladLoadGLLoader(loader);
	}
}


