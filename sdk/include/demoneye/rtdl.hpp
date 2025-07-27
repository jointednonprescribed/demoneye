
#ifndef __DEMONEYE__RTDL_H
#define __DEMONEYE__RTDL_H 1

#include "memory.hpp"



namespace demoneye
{
	class _LibraryPaths
	{
		std::vector<std::string> paths;

		_LibraryPaths& SetEqualTo(size_t npaths, const char **paths);
		_LibraryPaths& SetEqualTo(size_t npaths, std::string *paths);

	public:

		using iterator       = std::vector<const char*>::const_iterator;
		using const_iterator = std::vector<const char*>::iterator;

		_LibraryPaths(const char **paths, size_t npaths);
		_LibraryPaths(const std::string *paths, size_t npaths);
		_LibraryPaths(const std::vector<const char *> &paths);
		_LibraryPaths(const std::vector<const std::string> &paths);
		_LibraryPaths(const std::vector<const std::string&> &paths);
		template <size_t _NPaths>
		_LibraryPaths(const char *paths[_NPaths]):
			_LibraryPaths(paths, _NPaths)
		{}
		template <size_t _NPaths>
		_LibraryPaths(const std::string *paths[_NPaths]):
			_LibraryPaths(paths, _NPaths)
		{}
		template <size_t _NPaths>
		_LibraryPaths(const std::array<const char*, _NPaths> &paths):
			_LibraryPaths(paths.data(), _NPaths)
		{}
		template <size_t _NPaths>
		_LibraryPaths(const std::array<const std::string, _NPaths> &paths):
			_LibraryPaths(paths.data(), _NPaths)
		{}
		template <size_t _NPaths>
		_LibraryPaths(const std::array<const std::string&, _NPaths> &paths):
			_LibraryPaths(_NPaths)
		{
			for (size_t i = 0; i < _NPaths; i++)
				this->paths[i] = paths[i];
		}

		~_LibraryPaths();

		operator std::vector<std::string>() const;

		_LibraryPaths&     Expand(size_t expand_by);
		_LibraryPaths&     EnsureCapacity(size_t min_cap);
		size_t             Size() const;
		size_t             Capacity() const;

		// - Set the capacity with operator =(size_t)
		_LibraryPaths& operator =(size_t rhv);
		_LibraryPaths& operator =(const _LibraryPaths &rhv);
		_LibraryPaths& operator =(const std::vector<const char*> &rhv);
		_LibraryPaths& operator =(const std::vector<const std::string> &rhv);
		_LibraryPaths& operator =(const std::vector<const std::string&> &rhv);
		template <size_t _NPaths>
		_LibraryPaths& operator =(const std::array<const char*, _NPaths> &rhv)
		{
			SetEqualTo(rhv.data(), _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths& operator =(const std::array<const std::string, _NPaths> &rhv)
		{
			SetEqualTo(rhv.data(), _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths& operator =(const std::array<const std::string&, _NPaths> &rhv)
		{
			this->paths.clear();

			EnsureCapacity(_NPaths);

			for (size_t i = 0; i < _NPaths; i++)
				this->paths[i] = rhv[i];
		}
		template <size_t _NPaths>
		_LibraryPaths& operator =(const char *rhv[_NPaths])
		{
			SetEqualTo(rhv, _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths& operator =(const std::string rhv[_NPaths])
		{
			SetEqualTo(rhv, _NPaths);
		}

		const std::vector<const std::string>& Get() const;
		iterator           Begin();
		iterator           End();
		const_iterator     BeginConst()   const;
		const_iterator     EndConst()     const;
		const std::string& At(size_t idx) const;
		void               SetAt(size_t idx, const char *path);
		void               RemoveAt(size_t idx);
		const std::string& operator [](size_t idx);
		// operator -=(size_t) deletes a path at that index
		_LibraryPaths&     operator -=(size_t idx);

		_LibraryPaths&     Append(const char *append);
		_LibraryPaths&     Append(const std::string &append);
		_LibraryPaths&     Append(const _LibraryPaths &append);
		_LibraryPaths&     Append(size_t npaths, const char **append);
		_LibraryPaths&     Append(size_t npaths, const std::string *append);
		_LibraryPaths&     Append(const std::vector<const char*> &append);
		_LibraryPaths&     Append(const std::vector<const std::string> &append);
		_LibraryPaths&     Append(const std::vector<const std::string&> &append);
		template <size_t _NPaths>
		_LibraryPaths&     Append(const std::array<const char*, _NPaths> &rhv)
		{
			return Append(rhv.data(), _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths&     Append(const std::array<const std::string, _NPaths> &rhv)
		{
			return Append(rhv.data(), _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths&     Append(const std::array<const std::string&, _NPaths> &rhv)
		{
			const size_t sz = paths.size();
			EnsureCapacity(_NPaths + sz);

			for (size_t i = 0; i < _NPaths; i++)
				paths[i+sz] = rhv[i];
		}
		template <size_t _NPaths>
		_LibraryPaths&     Append(const char *rhv[_NPaths])
		{
			const size_t sz = paths.size();
			EnsureCapacity(_NPaths + sz);

			for (size_t i = 0; i < _NPaths; i++)
				paths[i+sz] = rhv[i];
		}
		template <size_t _NPaths>
		_LibraryPaths&     Append(const std::string rhv[_NPaths])
		{
			const size_t sz = paths.size();
			EnsureCapacity(_NPaths + sz);

			for (size_t i = 0; i < _NPaths; i++)
				paths[i+sz] = rhv[i];
		}
		// - Expand capacity with operator +=(size_t)
		_LibraryPaths& operator +=(size_t rhv);
		_LibraryPaths& operator +=(const char *rhv);
		_LibraryPaths& operator +=(const std::string &rhv);
		_LibraryPaths& operator +=(const _LibraryPaths &rhv);
		_LibraryPaths& operator +=(const std::vector<const char*> &rhv);
		_LibraryPaths& operator +=(const std::vector<const std::string> &rhv);
		_LibraryPaths& operator +=(const std::vector<const std::string&> &rhv);
		template <size_t _NPaths>
		_LibraryPaths& operator +=(const std::array<const char*, _NPaths> &rhv)
		{
			return Append(rhv.data(), _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths& operator +=(const std::array<const std::string, _NPaths> &rhv)
		{
			return Append(rhv.data(), _NPaths);
		}
		template <size_t _NPaths>
		_LibraryPaths& operator +=(const std::array<const std::string&, _NPaths> &rhv)
		{
			const size_t sz = paths.size();
			EnsureCapacity(_NPaths + sz);

			for (size_t i = 0; i < _NPaths; i++)
				paths[i+sz] = rhv[i];
		}
		template <size_t _NPaths>
		_LibraryPaths& operator +=(const char *rhv[_NPaths])
		{
			const size_t sz = paths.size();
			EnsureCapacity(_NPaths + sz);

			for (size_t i = 0; i < _NPaths; i++)
				paths[i+sz] = rhv[i];
		}
		template <size_t _NPaths>
		_LibraryPaths& operator +=(const std::string rhv[_NPaths])
		{
			const size_t sz = paths.size();
			EnsureCapacity(_NPaths + sz);

			for (size_t i = 0; i < _NPaths; i++)
				paths[i+sz] = rhv[i];
		}
	};
	extern _LibraryPaths RTLIB_PATHS;

	typedef class RuntimeLibrary
	{
		enum Bits
		{
			OK   = 0,
			FAIL = 01,
			OPEN = 02,

			ERRBITS = FAIL,
		};

		const char *lib_path, *file_path;
		int bits;

	public:

		/* If exact_dir is false, the path supplied will be searched for inside
		.  of the global library paths (LIB_PATHS), if it is true, the path
		.  supplied will be treated as an exact path to the intended library. */
		RuntimeLibrary(const char *full_path, bool exact_dir=false);
		RuntimeLibrary(const char *lib_path, const char *file_path);
		RuntimeLibrary(const _LibraryPaths &paths, const char *lib);

		~RuntimeLibrary();

		RuntimeLibrary& operator =(const RuntimeLibrary &rhv);
		/* These operator = methods set the library path as if given an exact
		.  path to some library */
		RuntimeLibrary& operator =(const char *rhv);
		RuntimeLibrary& operator =(const std::string &rhv);

		RuntimeLibrary& Open();
		void            Close();

		const char* GetLibraryPath() const;
		const char* GetLibraryDir()  const;
		const char* GetLibraryFilePath() const;
		const char* GetLibraryFileName() const;

		/* Clears the failure bit as well as any dedicated error bits. */
		bool Clear();
		/* Check if the failure bit is set. */
		bool Fail() const;
		/* Check if the library has been opened. */
		bool IsOpen() const;
	} RuntimeDL;
}



#endif // __DEMONEYE__RTDL_H
