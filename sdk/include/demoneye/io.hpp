
#ifndef __DEMONEYE__IO_H
#define __DEMONEYE__IO_H 1

#include "threading.hpp"



namespace demoneye
{
	class Path
	{
		memref<char> pathname;
	public:

		Path();
		Path(const Path&);

		Path(const char *path);
		Path(const char *base_path, const char *path);
	};

	class FileHandle;
	class FileManagerThread: public thread
	{
	public:
		static constexpr exit_protocol DEFAULT_EXIT_PROTOCOL = WAIT;

		FileManagerThread();
		~FileManagerThread();

		static FileHandle GetStdin();
		static FileHandle GetStdout();
		static FileHandle GetStderr();
	};

	class FileHandle
	{
	public:
		/* Stream Flags enum
		.  - 'rw' (bits 0 & 1): The read/write flags for the file, determines
		.                       what mode the file is opened in, when both bits
		.                       are 1, bit 2 ('+') is ignored, both reading and
		.                       writing are enabled, and the read and write ptrs
		.                       are given indepence from each other, this is known
		.                       as "rw" mode.
		.  - '+' (bit 2): When this bit as well as bit 1 ('r') are 1, the file
		.                 is opened in "r+" mode, this means that reading and
		.                 writing are both enabled, but the write ptr will always
		.                 follow the read ptr. When this bit as well as bit 0 ('w')
		.                 are 1, the file is in "w+" mode, this means that, once
		.                 again, reading and writing are both enabled, but in this
		.                 configuration, the read ptr will always be following the
		.                 write ptr. In any configuration where both bits 0 and 1
		.                 are 1, this bit is ignored.
		.  - 'T' (bit 3): This bit is ignored when linking an already open file to
		.                 a FileHandle, but when opening a file that is not already
		.                 open, this bit orders a truncation of it's contents if such
		.                 a truncation is not prohibited by a No-Truncate request
		.                 placed on that file.
		.  - 'b' (bit 4): This bit indicates that the file is to be read in binary mode.
		.  - 'c' (bit 5): This bit indicates that the if the file does not already exist,
		.                 it should not be created even if the file is being opened in
		.                 "w", "rw", or "w+" mode.
		.  - 'x' (bit 6): This bit indicates the the file should not be opened if it
		.                 already exists (only open if it is a new file), this bit is
		.                 ignored if bit 0 ('w') is 0, however, if bit 0 is 1, this bit
		.                 overrides bit 5 ('c') if it is also 1.
		.*/
		enum StreamFlags
		{ //                      //            E  N  x c b T + rw
			NULL_STREAMFLAGS = 0, //     0000 0 00 00 0 0 0 0 0 00

			/* Read/Write permissions flags */
			READ         = 02, //        0000 0 00 00 0 0 0 0 0 10
			WRITE        = 01, //        0000 0 00 00 0 0 0 0 0 01
			PLUS         = 04, //        0000 0 00 00 0 0 0 0 1 00

			/* Stream in binary mode */
			BINARY       = 010, //       0000 0 00 00 0 0 0 1 0 00

			/* Request truncation of the file upon opening (not held in file
			.  handle, ignored if the file is already open). */
			TRUNC        = 020, //       0000 0 00 00 0 0 1 0 0 00

			/* Do not create a file if it does not exist. */
			NOCREATE     = 040, //       0000 0 00 00 0 1 0 0 0 00

			/* Do not open a file in write mode if it already exists. */
			NEWFILE      = 0100, //      0000 0 00 00 1 0 0 0 0 00

			/* Print numbers in decimal format (default). */
			DEC          = 0200, //      0000 0 00 01 0 0 0 0 0 00
			/* Print numbers in hex format. */
			HEX          = 0400, //      0000 0 00 10 0 0 0 0 0 00
			/* Print numbers in octal format. */
			OCT          = 0600, //      0000 0 00 11 0 0 0 0 0 00
			/* Bit mask (Number Format): Print numbers in binary format by clearing
			.  these bits to 0. */
			NUMFMT       = 0600, //      0000 0 00 11 0 0 0 0 0 00

			/* Print large floating-point numbers in exponential notation (default). */
			EXP_LARGEFP  = 01000,
			/* Print large numbers of any type in exponential notation. */
			EXP_LARGE    = 02000,
			/* Print all numbers in exponential notation, regardless of size. */
			EXP_ALL      = 03000,
			/* Bit mask (Exponential Notation): Disable exponential notaton by
			.  clearing these bits to 0. */
			EXP_NOTATION = 03000,

			/* Defaults */
			RW           = READ  | WRITE,
			RPLUS        = READ  | PLUS,
			WPLUS        = WRITE | PLUS,

			DEFAULT_PRINT_FLAGS = DEC | EXP_LARGEFP,

			/* Aliases */
			IN  = READ,
			OUT = WRITE,
		};
		typedef long long streamflags;

		/* File position pointer type */
		typedef signed long long streamptr;

	private:
		Path       *path;
		streamflags flags;
		streamptr   r, w;
		signed int  width, precision, fd;

	public:

		FileHandle(const char *__restrict__ pathname, mode_t flags = RW);
		FileHandle(const char *__restrict__ pathname, const char *__restrict__ mode);
		FileHandle(size_t n, const char *pathname, streamflags flags = RW);
		FileHandle(size_t n, const char *__restrict__ pathname, const char *__restrict__ mode);

		~FileHandle();

		const Path& GetPath() const;
		streamflags GetFlags() const;
		signed int  GetDescriptor() const;

		/* Get the precision of values entered into the stream. */
		signed int  GetPrecision() const;

		/* Write to the file. */
		size_t  Write(const char *str)                           noexcept(false);
		size_t  Write(size_t n, const char *str)                 noexcept(false);
		size_t  WriteW(const wchar_t *str)                       noexcept(false);
		size_t  WriteW(size_t n, const wchar_t *str)             noexcept(false);
		size_t  Write(size_t n, size_t nmemb, const void *array) noexcept(false);
		size_t  Write(char c)                                    noexcept(false);
		size_t  Write(char c, size_t ntimes)                     noexcept(false);
		size_t  WriteF(const char *fmt, ...)                     noexcept(false);
		size_t  WriteWF(const wchar_t *fmt, ...)                 noexcept(false);
		/* Print to the file (to differ from Write() -- flushes the buffer when
		.  done writing, not just when the buffer is full). */
		size_t  Print(const char *str)                           noexcept(false);
		size_t  Print(size_t n, const char *str)                 noexcept(false);
		size_t  PrintW(const wchar_t *str)                       noexcept(false);
		size_t  PrintW(size_t n, const wchar_t *str)             noexcept(false);
		size_t  Print(size_t n, size_t nmemb, const void *array) noexcept(false);
		size_t  Print(char c)                                    noexcept(false);
		size_t  Print(char c, size_t ntimes)                     noexcept(false);
		size_t  PrintF(const char *fmt, ...)                     noexcept(false);
		size_t  PrintWF(const wchar_t *fmt, ...)                 noexcept(false);

		/* Read from the file. */
		size_t  Read()                                           noexcept(false);
		size_t  Read(size_t max_chars)                           noexcept(false);
		size_t  Read(char *dest, size_t max_chars)               noexcept(false);
		size_t  ReadW()                                          noexcept(false);
		size_t  ReadW(size_t max_chars)                          noexcept(false);
		size_t  ReadW(char *dest, size_t max_chars)              noexcept(false);
		size_t  Read(size_t n, size_t nmemb, void *array)        noexcept(false);
		size_t  ReadCh()                                         noexcept(false);
		size_t  ReadCh(char &c)                                  noexcept(false);
		size_t  ScanF(const char *fmt, ...)                      noexcept(false);
		size_t  ScanWF(const wchar_t *fmt, ...)                  noexcept(false);
		/* Skip characters in the file. */
		// - Ignore next char
		void    Ignore();
		// - Ignore all chars until a certain one is found, inclusive=true will
		//   keep the ptrs on the given char if/when it is found, while inclusive=false
		//   will skip it as well and settle on the next char.
		void    Ignore(char c, bool inclusive=true);
		void    Ignore(wchar_t wc, bool inclusive=true);
		// - Ignore n chars
		void    Ignore(size_t n);
		// - Ignore n chars or until a delimiter is reached
		void    Ignore(size_t n, char c, bool inclusive=true);
		void    Ignore(size_t n, wchar_t c, bool inclusive=true);
		// - Ignore whitespace until next line ending or non-whitespace character
		void    IgnoreSpaces();
		// - Ignore the rest of the line
		void    IgnoreLine();
		// - Ignore all whitespace, including line endings
		void    IgnoreWhitespace();

		/* Get the line endings in the file, at least 4 character capacity is
		.  recommended for destination array. */
		size_t  GetLineEndings(size_t max_chars, char *dest) noexcept(false);
		/* Get the length of the line endings in the file. */
		size_t  LineEndLength() noexcept(false);

		/* Read from buffer. */
		// - Output the start of the buffer, return the number of chars in the
		//   buffer.
		size_t  BuffRd(const char *&ptr_output)              const;
		// - Output the contents of the buffer to dest until either max_chars
		//   characters have been moved to the destination, or the end of the
		//   buffer has been reached.
		size_t  BuffRd(char *dest, size_t max_chars);
		// - Read the buffer in wchar mode if possible.
		size_t  BuffRdW(const wchar_t *&ptr_output)          const noexcept(false);
		// - Read the buffer contents into a destination string in wchar mode.
		size_t  BuffRdW(wchar_t *dest, size_t max_chars)           noexcept(false);
		// - Read the buffer contents into an array in binary mode.
		size_t  BuffRd(size_t n, size_t nmemb, void *array)        noexcept(false);
		// - Read a character from the buffer, output null character if the
		//   buffer is empty or in a different encoding mode.
		char    BuffRdCh();
		bool    BuffRdCh(char &c);
		// - wchar mode
		wchar_t BuffRdWc();
		bool    BuffRdWc(wchar_t &wc);
		/* Peek at chars in the buffer. */
		// (Peek, instead of Read, means the buffer data is unaffected)
		char    BuffPkCh()                                   const;
		bool    BuffPkCh(char &c)                            const;
		// wchar mode
		wchar_t BuffPkWc()                                   const;
		bool    BuffPkWc(wchar_t &wc)                        const;
		/* Skip characters in the buffer */
		// - Ignore all chars in the buffer
		void    BuffClr();
		// - Ignore next char
		void    BuffClrNext();
		// - Ignore all chars in the buffer until a certain one is found,
		//   inclusive=true will keep the given char if/when it is found
		//   in the buffer, while inclusive=false will clear it as well
		//   and settle on the next char.
		void    BuffClr(char c, bool inclusive=true);
		void    BuffClr(wchar_t wc, bool inclusive=true);
		// - Ignore n chars
		void    BuffClr(size_t n);
		// - Ignore n chars in the buffer or until a delimiter is reached
		void    BuffClr(size_t n, char c, bool inclusive=true);
		void    BuffClr(size_t n, wchar_t c, bool inclusive=true);
		// - Ignore whitespace in the buffer until next line ending or
		//   non-whitespace character
		void    BuffClrSpaces();
		// - Ignore the rest of the line in the buffer
		void    BuffClrLine();
		// - Ignore all whitespace in the buffer, including line endings
		void    BuffClrWhitespace();
	};
}



#endif // __DEMONEYE__IO_H
