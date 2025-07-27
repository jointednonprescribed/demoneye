


/* Implementing: demoneye/gl.hpp */
#include <demoneye/gl.hpp>



namespace demoneye
{
	bool operator !(const GLFWContext &rhv)
	{
		return !rhv.IsInitialized();
	}
	bool operator !(const GLWindow &rhv)
	{
		return rhv.Get() == NULL;
	}
}


