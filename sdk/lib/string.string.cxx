


/* Implementing: demoneye/string.hpp: class string */
#include <demoneye/string.hpp>
#include <demoneye/setup.hpp>
#include <demoneye/memory.hpp>



namespace demoneye
{
	const std::type_info& string::type_info = typeid(string);

	string::string():
		data(), wcview()
	{}
	string::string(const string &rhv):
		data(rhv.data), wcview(rhv.data)
	{}

	string::string(const std::string &str):
		string(str.length(), str.data())
	{}
	string::string(const std::wstring &str):
		string(str.length(), str.data())
	{}

	string::string(size_t n, const char *str, int flags):
		string(n, _init_state)
	{
		flags &= flags_mask;
		flags &= (~wstr);

		char *addr = &(data.Address()[1]);

		for (size_t i = 0; i < n; i++, addr++, str++)
			*addr = *str;

		SetFlags(flags);
	}
	string::string(size_t n, const wchar_t *str, int flags):
		string(n, _init_state | wstr)
	{
		flags &= flags_mask;
		flags |= wstr;

		wchar_t *addr = wcview;

		for (size_t i = 0; i < n; i++)
			addr[i] = str[i];

		SetFlags(flags);
	}

	string::string(size_t n, int flags):
		data(n+1), wcview(data, 1)
	{
		flags &= flags_mask;

		*data = char(flags);
	}

	string::~string()
	{
		this->Drop();
	}

	int string::Drop()
	{
		wcview.Drop(); 
	}

	int string::Terminate()
	{
		return this->Drop();
	}

	const std::type_info& string::GetTypeInfo() const noexcept
	{
		return typeid(string);
	}

	charvalue_t* string::GetOrigin() noexcept
	{
		if (flags & wstr == 0) {
			it_origin = charvalue_t(data.GetIndex(1));
		} else {
			it_origin = charvalue_t(*wcview);
		}

		return &this->it_origin;
	}

	bool string::IterableStepFunction(iterator<charvalue_t> &iter)
	{
		const size_t limit = iter.GetMaxIterations();
		if (iter.HasIterationLimit() && iter.TypeInfo() == typeid(string)) {
			void *const istr_raw = iter.Iterable();
			string     *istr     = static_cast<string*>(istr_raw);

			iter.index++;
			if (iter.index == limit)
				return false;
			
			

			return true;
		} else
			return false;
	}

	bool string::HasLimit() const noexcept
	{
		return true;
	}
	size_t string::Limit() const noexcept
	{
		return Length();
	}

	int string::ConvertToHeapFlags(int str_flags)
	{
		int flags = SystemHeap::owner_read | SystemHeap::owner_auth;

		flags |= (str_flags & group_rw) >> (group_perms_bshft - SystemHeap::group_perms_bshft);
		flags |= (str_flags & public_rw) >> (public_perms_bshft - SystemHeap::public_perms_bshft);
		flags |= (SystemHeap::owner_write * ((str_flags & const_s) == 0));

		return flags;
	}

	memview<char> string::DataView()
	{
		return memview<char>(data, 1);
	}
	rememview<char, wchar_t> string::DataViewW()
	{
		return rememview<char, wchar_t>(data, 1);
	}
	memview<char> string::DataView() const
	{
		return memview<char>(data, 1);
	}
	const rememview<char, wchar_t>& string::DataViewW() const
	{
		return wcview;
	}
	const char* string::Address() const
	{
		return data.AddrIndex(1);
	}
	const wchar_t* string::AddressW() const
	{
		return wcview.Address();
	}
	int string::Flags() const
	{
		return int(flags);
	}

	size_t string::Length() const
	{
		return len;
	}
	size_t string::LengthInBytes() const
	{
		const size_t char_size = ((flags & wstr) == 0)? 1 : sizeof(wchar_t);

		return len * char_size;
	}
	size_t string::Capacity() const
	{
		const size_t char_size = ((flags & wstr) == 0)? 1 : sizeof(wchar_t);

		return (data.GetBlockSize() - 1) / char_size;
	}
	size_t string::CapacityInBytes() const
	{
		return data.GetBlockSize() - 1;
	}
	size_t string::RemainingCapacity() const
	{
		return Capacity() - len;
	}
	size_t string::RemainingCapacityInBytes() const
	{
		const size_t char_size = ((flags & wstr) == 0)? 1 : sizeof(wchar_t),
			bytes_size = char_size * (data.GetBlockSize() - 1),
			bytes_size2 = len * char_size;

		return bytes_size - bytes_size2;
	}

	int  string::SetFlags(int flags)
	{
		flags &= flags_mask;

		try {
			*data = char(flags);
			flags = flags;
		} catch (exception e) {}
		catch(std::exception e) {
			logerr(e);
		}

		return flags;
	}

	bool string::IsConst() const
	{
		return flags & const_s;
	}
	bool string::IsCharWidth() const
	{
		return (flags & wstr) == 0;
	}
	bool string::IsWCharWidth() const
	{
		return flags & wstr;
	}

	size_t string::Expand(size_t amount)
	{
		if (data.IsNull()) {
			data.Alloc(amount);
			return data.GetBlockSize();
		}
	}

	int string::SetTo(const string &set_to)
	{
		SetFlags(set_to.flags);

		EnsureCapacity(set_to.Capacity());

		const size_t l = set_to.Length();

		if ((set_to.flags & wstr) != 0) {
			wchar_t       *dest = wcview;
			const wchar_t *src  = set_to.wcview;

			for (size_t i = 0; i < l; i++)
				dest[i] = src[i];
		} else {
			
		}
	}
	int string::SetTo(const std::string &set_to)
	{
		return SetTo(set_to.length(), set_to.data());
	}
	int string::SetTo(const std::wstring &set_to)
	{
		return SetTo(set_to.length(), set_to.data());
	}
	int string::SetToSubstr(ssize_t beg, size_t len, const string &substr_src)
	{
		if (flags & wstr == 0) {
			const char *cdata = substr_src.data.AddrIndex(1);
			return SetTo(len, &(cdata[modules::iter::absolute_index(beg, substr_src.Length())]));
		} else {
			const wchar_t *wcdata = substr_src.wcview;
			return SetTo(len, &(wcdata[modules::iter::absolute_index(beg, substr_src.Length())]));
		}
	}
	int string::SetToSubstr(ssize_t beg, size_t len, const std::string &substr_src)
	{
		return SetTo(len, &(substr_src.data()[modules::iter::absolute_index(beg, substr_src.length())]));
	}
	int string::SetToSubstr(ssize_t beg, size_t len, const std::wstring &substr_src)
	{
		return SetTo(len, &(substr_src.data()[modules::iter::absolute_index(beg, substr_src.length())]));
	}
	int string::SetTo(const iIterable<charvalue_t> &iter)
	{
		if (SetFlags(flags | wstr) != 0) {
			return logerr(-1, "Failed to set string flags.");
		}

		iterator<charvalue_t> it(
			static_cast<const iIterable<charvalue_t>&>(*this),
			iterator<charvalue_t>::step_function_t::FromLambda(
				[](iterator<charvalue_t> &iter)
				{
					return IterableStepFunction(iter);
				}
			)
		);


	}
	int string::SetTo(const iIterable<char>&);
	int string::SetTo(const iIterable<wchar_t>&);
	int string::SetTo(size_t max_iterations, const iIterable<charvalue_t>&);
	int string::SetTo(size_t max_iterations, const iIterable<char>&);
	int string::SetTo(size_t max_iterations, const iIterable<wchar_t>&);
	int string::SetTo(const char *str)
	{
		return SetTo(strlen(str), str);
	}
	int string::SetTo(const wchar_t *wstr)
	{
		return SetTo(wcslen(wstr), wstr);
	}
	int string::SetTo(size_t n, const char *str)
	{
		if ((!data.IsNull() && (ClearAll() != 0)) || (SetFlags(flags | string::wstr) != 0) || (EnsureCapacity(n) != n))
			return logerr(-1, "Failed to get ready for string data overwrite (failed to clear current data, reset string flags, or make space for new data).");

		char *addr = data.AddrIndex(1);

		for (size_t i = 0; i < n; i++)
			addr[i] = str[i];
		
		return 0;
	}
	int string::SetTo(size_t n, const wchar_t *wstr)
	{
		if ((!data.IsNull() && (ClearAll() != 0)) || (SetFlags(flags | string::wstr) != 0) || (EnsureCapacity(n) != n))
			return logerr(-1, "Failed to get ready for string data overwrite (failed to clear current data, reset string flags, or make space for new data).");

		wchar_t *addr = wcview;

		for (size_t i = 0; i < n; i++)
			addr[i] = wstr[i];
		
		return 0;
	}
	int string::SetAt(ssize_t set_at, const string &set_to)
	{
		if (set_to.flags & wstr == 0)
			return SetAt(set_at, set_to.Length(), set_to.data.AddrIndex(1));
		else
			return SetAt(set_at, set_to.Length(), static_cast<const wchar_t*>(set_to.wcview));
	}
	int string::SetAt(ssize_t set_at, const std::string &set_to)
	{
		return SetAt(set_at, set_to.length(), set_to.data());
	}
	int string::SetAt(ssize_t set_at, const std::wstring &set_to)
	{
		return SetAt(set_at, set_to.length(), set_to.data());
	}
	int string::SetToSubstrAt(ssize_t set_at, ssize_t substr_beg, size_t substr_len, const string &set_to)
	{
		if (set_to.flags & wstr == 0)
			return SetAt(
				set_at,
				substr_len,
				static_cast<const char*> (
					set_to.data.AddrIndex(modules::iter::absolute_index(substr_beg, set_to.Length()) + 1)
				)
			);
		else
			return SetAt(
				set_at,
				substr_len,
				static_cast<const wchar_t*> (
					set_to.wcview.AddrIndex(modules::iter::absolute_index(substr_beg, set_to.Length()))
				)
			);
	}
	int string::SetToSubstrAt(ssize_t set_at, ssize_t substr_beg, size_t substr_len, const std::string &set_to)
	{
		return SetAt(set_at, substr_len, &(set_to.data()[modules::iter::absolute_index(substr_beg, set_to.length())]));
	}
	int string::SetToSubstrAt(ssize_t set_at, ssize_t substr_beg, size_t substr_len, const std::wstring &set_to)
	{
		return SetAt(set_at, substr_len, &(set_to.data()[modules::iter::absolute_index(substr_beg, set_to.length())]));
	}
	int string::SetAt(ssize_t set_at, const iIterable<charvalue_t>&);
	int string::SetAt(ssize_t set_at, const iIterable<char>&);
	int string::SetAt(ssize_t set_at, const iIterable<wchar_t>&);
	int string::SetAt(ssize_t set_at, const char*);
	int string::SetAt(ssize_t set_at, const wchar_t*);
	int string::SetAt(ssize_t set_at, size_t n, const char *str);
	int string::SetAt(ssize_t set_at, size_t n, const wchar_t *wstr);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const string&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const std::string&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const std::wstring&);
	int string::SetPortionToSubstr(ssize_t portion_beg, ssize_t substr_beg, size_t set_len, const string&);
	int string::SetPortionToSubstr(ssize_t portion_beg, ssize_t substr_beg, size_t set_len, const std::string&);
	int string::SetPortionToSubstr(ssize_t portion_beg, ssize_t substr_beg, size_t set_len, const std::wstring&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const iIterable<charvalue_t>&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const iIterable<char>&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const iIterable<wchar_t>&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t max_iterations, const iIterable<charvalue_t>&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t max_iterations, const iIterable<char>&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t max_iterations, const iIterable<wchar_t>&);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const char*);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, const wchar_t*);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t n, const char *str);
	int string::SetPortionTo(ssize_t portion_beg, size_t portion_len, size_t n, const wchar_t *wstr);

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
}


