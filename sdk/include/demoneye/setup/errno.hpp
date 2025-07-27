
#ifndef __DEMONEYE__setup_ERRNO_H
#define __DEMONEYE__setup_ERRNO_H 1

#include "types.hpp"



namespace demoneye
{
	enum STAT
	{
		OK = 0,
		TRUE = OK,
		FALSE,
		FAIL,

		NO_IMPL,

		NONEXISTENT,

		NOT_WRITABLE,
		NOT_READABLE,
		NOT_EXECUTABLE,

		NULL_ARG
	};

	class exception: public std::exception // std:set_terminate should be used in Demoneye Runtime Context
	{
		const char *errname, *errdesc;
		long long   errcode;
	public:

		exception();
		exception(long long error_code);
		exception(const char *desc, long long error_code = -1);

		virtual const char* Prompt() const;
		virtual void        Throw()  const noexcept(false);
		virtual const char* what()   const noexcept override; // inherited from std::exception

		virtual operator int() const;
		virtual operator long long() const;
		virtual operator const char*() const;
	};

	class divide_by_zero_error: public exception
	{
	public:
		divide_by_zero_error();
		divide_by_zero_error(long long error_code);
		divide_by_zero_error(const char *desc, long long error_code = -1);
	};

	class runtime_error: public exception
	{
	public:
		runtime_error();
		runtime_error(long long error_code);
		runtime_error(const char *desc, long long error_code = -1);
	};

	class overflow_error: public exception
	{
	public:
		overflow_error();
		overflow_error(long long error_code);
		overflow_error(const char *desc, long long error_code = -1);
	};

	class underflow_error: public exception
	{
	public:
		underflow_error();
		underflow_error(long long error_code);
		underflow_error(const char *desc, long long error_code = -1);
	};

	class nonnull_error: public exception
	{
	public:
		nonnull_error();
		nonnull_error(long long error_code);
		nonnull_error(const char *desc, long long error_code = -1);
	};

	class index_error: public exception
	{
	public:
		index_error();
		index_error(long long error_code);
		index_error(const char *desc, long long error_code = -1);
	};

	class argument_error: public exception
	{
	public:
		argument_error();
		argument_error(long long error_code);
		argument_error(const char *desc, long long error_code = -1);
	};

	class out_of_memory_error: public exception
	{
	public:
		out_of_memory_error();
		out_of_memory_error(long long error_code);
		out_of_memory_error(const char *desc, long long error_code = -1);
	};

	const std::exception& logerr(const std::exception& e);
	int        logerr(int error_code, const char *label);
	int        logerr(int error_code, std::string label);
	int        logerrf(int error_code, const char *fmt, ...);

	int        logwarn(const char *msg);
	int        logwarn(std::string msg);
	int        logwarnf(const char *fmt, ...);

	int        log(const char *msg) noexcept(false);
	int        log(std::string msg) noexcept(false);
	int        logf(const char *fmt, ...) noexcept(false);
}



#endif // __DEMONEYE__setup_ERRNO_H
