
#ifndef __DEMONEYE__STRING_H
#define __DEMONEYE__STRING_H 1

#include "iter.hpp"



namespace demoneye
{
	/* String Comparison Result type */
	struct strcmp_t
	{
		int cmp_strlen, cmp_lastchr;

		strcmp_t();
		strcmp_t(int cmp_strlen, int cmp_lastchr);

		strcmp_t& operator =(const strcmp_t &rhv);

		int StringLength() const;
		int LastCharacter() const;

		bool LengthGreater() const;
		bool LengthLess() const;
		bool LengthGreaterEq() const;
		bool LengthLessEq() const;
		bool LengthEqual() const;
		bool LengthUnequal() const;

		bool CharGreater() const;
		bool CharLess() const;
		bool CharGreaterEq() const;
		bool CharLessEq() const;
		bool CharEqual() const;
		bool CharUnequal() const;

		bool Equal() const;
		bool Unequal() const;

		bool operator ==(const strcmp_t &rhv) const;
		bool operator !=(const strcmp_t &rhv) const;
	};

	/* String length operations */
	inline size_t (&strlen)(const char*) = ::strlen;
	inline size_t (&wstrlen)(const wchar_t*) = ::wcslen;

	/* Compare a two UTF-8 strings. */
	strcmp_t cmpstr(const char *s1, const char *s2);
	strcmp_t cmpstr(const char *s1, const char *s2, size_t n);
	/* Compare a two UTF-16 strings. */
	strcmp_t cmpstr(const wchar_t *s1, const wchar_t *s2);
	strcmp_t cmpstr(const wchar_t *s1, const wchar_t *s2, size_t n);

	/* Copy a UTF-8 string from one place in memory to another. */
	char*    cpystr(char *dest, size_t destsz, const char *src) noexcept(false);
	char*    cpystrn(char *dest, size_t destsz, const char *src, size_t cpysz) noexcept(false);
	/* Copy a UTF-16 string from one place in memory to another. */
	wchar_t* cpywstr(wchar_t *dest, size_t destsz, const wchar_t *src) noexcept(false);
	wchar_t* cpywstrn(wchar_t *dest, size_t destsz, const wchar_t *src, size_t cpysz) noexcept(false);

	/* Convert a UTF-8 (char) string to a UTF-16 (wchar_t) string. */
	wchar_t* strtowcs(
		wchar_t *__restrict__    conv_dest,
		size_t                   destsz,
		const char *__restrict__ conv_src
	) noexcept(false);
	wchar_t* strtowcsn(
		wchar_t *__restrict__    conv_dest,
		size_t                   destsz,
		const char *__restrict__ conv_src,
		size_t                   cpysz
	) noexcept(false);
	/* Convert a UTF-16 (wchar_t) string to a UTF-8 (char) string. */
	char*    wcstostr(
		char *__restrict__          conv_dest,
		size_t                      destsz,
		const wchar_t *__restrict__ conv_src
	) noexcept(false);
	char*    wcstostrn(
		char *__restrict__          conv_dest,
		size_t                      destsz,
		const wchar_t *__restrict__ conv_src,
		size_t                      cpysz
	) noexcept(false);

	/* Character Value structure */
	// used to contain character values regardless of character type
	class charvalue_t
	{
		wchar_t val;
		bool    wc;
	public:

		charvalue_t();
		charvalue_t(char c);
		charvalue_t(wchar_t wc);

		constexpr operator wchar_t() const
		{
			return wchar_t(val);
		}
		constexpr operator char() const
		{
			return (val > 255)? -1 : char(val);
		}
		constexpr bool IsWChar() const
		{
			return wc;
		}
		constexpr bool IsChar() const
		{
			return !wc;
		}

		charvalue_t& operator =(char c);
		charvalue_t& operator =(wchar_t c);
		charvalue_t& operator =(charvalue_t c);

		constexpr operator char() const
		{
			return ((char) val) * (!wc);
		}
		constexpr operator wchar_t() const
		{
			return ((wchar_t) val) * wc;
		}
	};

	class string: public iIterable<charvalue_t>
	{
		memref<char>             data;
		rememview<char, wchar_t> wcview;
		size_t                   len;
		charvalue_t              it_origin, it_current;

		char                     flags;


		static constexpr size_t STRBLK_DATA_OFFSET = 1;
		// Two-byte offset from the beginning of the memory block that the string
		// is allocated inside of these two bytes are used for storing string
		// flags.

	public:
		/* This enum is used to store flag bits for string permissions and
		.* initialization options:
		.*
		.*  - 'T' (bit 0): Flags the creation of a null sentinel character for
		.*                 (const char*) compatibility.
		.*  - 'c' (bit 1): Flags the string as a 'const' string, all write operations
		.*                 after this flag is applied are unpermitted (except for
		.*                 modifications to string flags & permissions by the string
		.*                 author(s)). Initialize the string itself BEFORE applying
		.*                 this flag.
		.*  - 'W' (bit 2): Flags the string as a wide string utilizing UTF-16 encoded
		.*                 'wchar_t' characters, without this flag, the string is a
		.*                 'cstring' made up of UTF-8 encoded 'char' characters.
		.*  - 'rw' (bits 3 & 4): These bits are flags pertaining to the read/write
		.*                       permissions of the other members of this string's
		.*                       owner's group.
		.*  - 'RW' (bits 5 & 6): These bits are flags pertaining to the read/write
		.*                       permissions of the public, anyone other than the
		.*                       owner and the other members of their group.
		.*/
		enum flags_t: char
		{ //                             RW rw W c T
			null_term    = 01, //      0 00 00 0 0 0
			const_s      = 02, //      0 00 00 0 1 0
			ro           = const_s, // alias for 'const_s'
			wstr         = 04, //      0 00 00 1 0 0
			null_term_bshft = 0,
			const_s_bshft = 1,
			wstr_bshft = 2,

			group_write  = 010, //     0 00 01 0 0 0
			group_read   = 020, //     0 00 10 0 0 0
			group_rw     = 030, //     0 00 11 0 0 0
			group_perms_bshft = 3,

			public_write = 040, //     0 00 01 0 0 0
			public_read  = 0100, //    0 00 10 0 0 0
			public_rw    = 0140, //    0 11 00 0 0 0
			public_perms_bshft = 5,

			wstr_default = wstr,
			cstr_default = 0,

			flags_mask   = 0x7,
			_init_state  = 0,
		};

		string();
		string(const string&);

		string(const std::string&);
		string(const std::wstring&);

		string(size_t n, const char *str, int flags=cstr_default);
		string(size_t n, const wchar_t *str, int flags=wstr_default);

		template <size_t _N>
		string(const char str[_N], int flags=cstr_default):
			string(_N, str, flags)
		{}
		template <size_t _N>
		string(const wchar_t wstr[_N], int flags=wstr_default):
			string(_N, wstr, flags)
		{}

		string(size_t preallocate_capacity, int flags=cstr_default);

		~string();

		static const std::type_info& type_info;

		int         Drop()      noexcept;
		virtual int Terminate() noexcept override;

		virtual const std::type_info& GetTypeInfo() const noexcept override;

		virtual charvalue_t* GetOrigin()      noexcept override;

		static bool IterableStepFunction(iterator<charvalue_t>&);

		virtual bool   HasLimit() const noexcept override;
		virtual size_t Limit()    const noexcept override;

		static int ConvertToHeapFlags(int str_flags);

		memview<char>                   DataView();
		rememview<char, wchar_t>        DataViewW();
		memview<char>                   DataView()  const;
		const rememview<char, wchar_t>& DataViewW() const;
		const char*      Address()   const;
		const wchar_t*   AddressW()  const;
		int              Flags()     const;

		size_t Length() const;
		size_t LengthInBytes() const;
		size_t Capacity() const;
		size_t CapacityInBytes() const;
		size_t RemainingCapacity() const;
		size_t RemainingCapacityInBytes() const;

		int  SetFlags(int flags);

		bool IsConst()      const;
		bool IsCharWidth()  const;
		bool IsWCharWidth() const;

		/* Expand the strings capacity by a certain amount of characters, returns
		.  the new capacity of the string, throws an exception if operation fails. */
		size_t Expand(size_t expand_by_amount) noexcept(false);
		/* Resize the string's memory bank to a given size, 0 will drop the
		.  memory bank, throws an exception if the operation does not succeeded. */
		void   Resize(size_t new_size) noexcept(false);
		/* Ensure that the string has at least the given capacity, resize if the string
		.  has less than the given capacity, do nothing otherwise, returns 0 if the
		.  operation suceeded, this function assumes character type based on string
		.  flags at the time of execution. */
		int    EnsureCapacity(size_t minimum) noexcept;
		/* Ensure that the string has an exact, given capacity, resize if the string
		.  has less or more than the given capacity, do nothing otherwise, returns 0
		.  if the operation suceeded, this function assumes character type based on
		.  string flags at the time of execution. */
		int    EnsureExactCapacity(size_t minimum) noexcept;

		/* Set the string's data equal to a given input string or substring in any form. */
		int SetTo(const string&);
		int SetTo(const std::string&);
		int SetTo(const std::wstring&);
		int SetToSubstr(ssize_t beg, size_t len, const string&);
		int SetToSubstr(ssize_t beg, size_t len, const std::string&);
		int SetToSubstr(ssize_t beg, size_t len, const std::wstring&);
		int SetTo(const iIterable<charvalue_t>&);
		int SetTo(const iIterable<char>&);
		int SetTo(const iIterable<wchar_t>&);
		int SetTo(size_t max_iterations, const iIterable<charvalue_t>&);
		int SetTo(size_t max_iterations, const iIterable<char>&);
		int SetTo(size_t max_iterations, const iIterable<wchar_t>&);
		int SetTo(const char*);
		int SetTo(const wchar_t*);
		int SetTo(size_t n, const char *str);
		int SetTo(size_t n, const wchar_t *wstr);
		/* Set data after a certain index of the string to a given input string. */
		int SetAt(ssize_t set_at, const string&);
		int SetAt(ssize_t set_at, const std::string&);
		int SetAt(ssize_t set_at, const std::wstring&);
		int SetToSubstrAt(ssize_t set_at, ssize_t substr_beg, size_t substr_len, const string&);
		int SetToSubstrAt(ssize_t set_at, ssize_t substr_beg, size_t substr_len, const std::string&);
		int SetToSubstrAt(ssize_t set_at, ssize_t substr_beg, size_t substr_len, const std::wstring&);
		int SetAt(ssize_t set_at, const iIterable<charvalue_t>&);
		int SetAt(ssize_t set_at, const iIterable<char>&);
		int SetAt(ssize_t set_at, const iIterable<wchar_t>&);
		// int SetAt(ssize_t set_at, size_t max_iterations, const iIterable<charvalue_t>&);
		// int SetAt(ssize_t set_at, size_t max_iterations, const iIterable<char>&);
		// int SetAt(ssize_t set_at, size_t max_iterations, const iIterable<wchar_t>&); // MAY NOT BE NECESSARY WITH ITERATOR
		int SetAt(ssize_t set_at, const char*);
		int SetAt(ssize_t set_at, const wchar_t*);
		int SetAt(ssize_t set_at, size_t n, const char *str);
		int SetAt(ssize_t set_at, size_t n, const wchar_t *wstr);
		/* Set a certain portion of the string to a given input string. */
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const string&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const std::string&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const std::wstring&);
		int SetPortionToSubstr(ssize_t portion_beg, ssize_t substr_beg, size_t set_len, const string&);
		int SetPortionToSubstr(ssize_t portion_beg, ssize_t substr_beg, size_t set_len, const std::string&);
		int SetPortionToSubstr(ssize_t portion_beg, ssize_t substr_beg, size_t set_len, const std::wstring&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const iIterable<charvalue_t>&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const iIterable<char>&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const iIterable<wchar_t>&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t max_iterations, const iIterable<charvalue_t>&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t max_iterations, const iIterable<char>&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t max_iterations, const iIterable<wchar_t>&);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const char*);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, const wchar_t*);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t n, const char *str);
		int SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t n, const wchar_t *wstr);

		string& operator =(const string&);
		string& operator =(const std::string&);
		string& operator =(const std::wstring&);
		string& operator =(const char*);
		string& operator =(const wchar_t*);
		string& operator =(const memref<char>&);
		string& operator =(const memref<wchar_t>&);
		string& operator =(const memview<char>&);
		string& operator =(const memview<wchar_t>&);
		string& operator =(const rememview<wchar_t, char>&);
		string& operator =(const rememview<char, wchar_t>&);
		template <size_t _N>
		string& operator =(const char str[_N])
		{
			SetTo(_N, str);
			return *this;
		}
		template <size_t _N>
		string& operator =(const wchar_t wstr[_N])
		{
			SetTo(_N, wstr);
			return *this;
		}

		/* Append some other string to the end of this string. */
		int Append(const string&);
		int Append(const std::string&);
		int Append(const std::wstring&);
		int AppendSubstring(ssize_t substr_beg, size_t substr_len,
			const string &substr_src);
		int AppendSubstring(ssize_t substr_beg, size_t substr_len,
			const std::string &substr_src);
		int AppendSubstring(ssize_t substr_beg, size_t substr_len,
			const std::wstring &substr_src);
		int Append(const iIterable<charvalue_t>&);
		int Append(const iIterable<char>&);
		int Append(const iIterable<wchar_t>&);
		int Append(const char*);
		int Append(const wchar_t*);
		int Append(size_t n, const char *str);
		int Append(size_t n, const wchar_t *wstr);
		/* Append some other string to the beginning of this string. */
		int AppendBeg(const string&);
		int AppendBeg(const std::string&);
		int AppendBeg(const std::wstring&);
		int AppendSubstringBeg(ssize_t substr_beg, size_t substr_len,
			const string &substr_src);
		int AppendSubstringBeg(ssize_t substr_beg, size_t substr_len,
			const std::string &substr_src);
		int AppendSubstringBeg(ssize_t substr_beg, size_t substr_len,
			const std::wstring &substr_src);
		int AppendBeg(const iIterable<charvalue_t>&);
		int AppendBeg(const iIterable<char>&);
		int AppendBeg(const iIterable<wchar_t>&);
		int AppendBeg(const char*);
		int AppendBeg(const wchar_t*);
		int AppendBeg(size_t n, const char *str);
		int AppendBeg(size_t n, const wchar_t *wstr);
		/* Append some other string to a given point in this string. */
		int AppendAt(ssize_t append_at, const string&);
		int AppendAt(ssize_t append_at, const std::string&);
		int AppendAt(ssize_t append_at, const std::wstring&);
		int AppendSubstringAt(ssize_t append_at, ssize_t substr_beg,
				size_t substr_len, const string &substr_src);
		int AppendSubstringAt(ssize_t append_at, ssize_t substr_beg,
				size_t substr_len, const std::string &substr_src);
		int AppendSubstringAt(ssize_t append_at, ssize_t substr_beg,
				size_t substr_len, const std::wstring &substr_src);
		int AppendAt(ssize_t append_at, const iIterable<charvalue_t>&);
		int AppendAt(ssize_t append_at, const iIterable<char>&);
		int AppendAt(ssize_t append_at, const iIterable<wchar_t>&);
		int AppendAt(ssize_t append_at, const char*);
		int AppendAt(ssize_t append_at, const wchar_t*);
		int AppendAt(ssize_t append_at, size_t n, const char *str);
		int AppendAt(ssize_t append_at, size_t n, const wchar_t *wstr);

		/* Trim the string's unused data. (Trim() functions delete memory to
		.  resize the string data buffer and return the new size of the buffer). */
		size_t TrimToSize();
		size_t TrimBuffer(size_t trim_size); /* trim from the end including
		//                                      uninitialized spaces. */
		size_t TrimBufferBeg(size_t trim_size); // trim from beginning instead.
		size_t TrimBufferAt(ssize_t trim_at, size_t trim_size); /* trim at a
		//                                                   a given position
		//                                                   by a given amount. */
		size_t Trim(size_t trim_size /* clear the ending of the string
		.                                               starting at a given index. */, bool tim_to_size=true); /* trim initialized
		//                                            string data only, trim_to_size
		//                                            invokes concurrent deletion of
		//                                            unused memory space. */
		size_t TrimBeg(size_t trim_size, bool trim_to_size=true); /* trim from
		//                                                           beginning. */
		size_t TrimAt(ssize_t trim_at, size_t trim_size, bool trim_to_size=true);

		/* Clear initialized string data from the memory buffer without deleting
		.  or reallocating memory, return the new length of the string. */
		size_t ClearAll() noexcept; // clear all initalized characters
		size_t ClearEnd(size_t clear_amount) noexcept; /* clear characters from the
		.                                                 end. */
		size_t ClearBeg(size_t clear_amount) noexcept; /* clear characters from the
		.                                                 beginning. */
		size_t ClearEnd(ssize_t clear_idx) noexcept; /* clear the ending of the string
		.                                               starting at a given index. */
		size_t ClearBeg(ssize_t clear_idx) noexcept; /* clear the ending of the string
		.                                               starting at a given index. */
		size_t ClearPortion(ssize_t portion_beg, size_t portion_size) noexcept;


	};
}



#endif // __DEMONEYE__STRING_H
