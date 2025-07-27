


/* Implementing: demoneye/resmgr.hpp */
#include <demoneye/resmgr.hpp>



namespace demoneye
{
	size_t filesize(const char *pathname)
	{
		using std::fstream, std::ios;

		fstream file(pathname, ios::in | ios::binary | ios::ate);

		size_t fsize = static_cast<size_t>(file.tellg());
		file.close();

		return fsize;
	}
	size_t filesize(std::string pathname)
	{
		using std::fstream, std::ios;

		fstream file(pathname, ios::in | ios::binary | ios::ate);

		size_t fsize = static_cast<size_t>(file.tellg());
		file.close();

		return fsize;
	}
}


