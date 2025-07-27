
#ifndef __DEMONEYE__LOGGER_H
#define __DEMONEYE__LOGGER_H 1

#include "io.hpp"



namespace demoneye
{
	class LoggerFMThread: public FileManagerThread
	{
	public:

		LoggerFMThread();
		LoggerFMThread(const LoggerFMThread&);

		LoggerFMThread(const FileHandle &logfile);
		LoggerFMThread(const FileHandle &outpipe, const FileHandle &errpipe, const FileHandle &logfile);

		LoggerFMThread(const supplier<FileHandle> &logfile_supplier);
		LoggerFMThread(
			const supplier<FileHandle> &outpipe_supplier,
			const supplier<FileHandle> &errpipe_supplier,
			const supplier<FileHandle> &logfile_supplier
		);

		LoggerFMThread(const char *logfile_path);
		LoggerFMThread(const char *outpipe_path, const char *errpipe_path, const char *logfile_path);

		LoggerFMThread(const std::string &logfile_path);
		LoggerFMThread(
			const std::string &outpipe_path,
			const std::string &errpipe_path,
			const std::string &logfile_path
		);

		~LoggerFMThread();

		/* Assign using another LoggerFMThread object */
		LoggerFMThread& operator =(const LoggerFMThread&);

		/* Get log output pipe. */
		const FileHandle& GetOutputPipe() const;
		/* Get log error pipe. */
		const FileHandle& GetErrorPipe() const;
		/* Get the log file. */
		const FileHandle& GetLogFile() const;

		/* Get log output pipe. */
		bool SetOutputPipe(const FileHandle&);
		bool SetOutputPipe(const supplier<FileHandle>&);
		bool SetOutputPipe(const char*);
		bool SetOutputPipe(const std::string&);
		/* Get log error pipe. */
		bool SetErrorPipe(const FileHandle&);
		bool SetErrorPipe(const supplier<FileHandle>&);
		bool SetErrorPipe(const char*);
		bool SetErrorPipe(const std::string&);
		/* Get the log file. */
		bool SetLogFile(const FileHandle&);
		bool SetLogFile(const supplier<FileHandle>&);
		bool SetLogFile(const char*);
		bool SetLogFile(const std::string&);
	};
}



#endif // __DEMONEYE__LOGGER_H
