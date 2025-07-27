


/* Implementing: demoneye/string.hpp: struct strcmp_t */
#include <demoneye/string.hpp>



namespace demoneye
{
	strcmp_t::strcmp_t()
	{
		cmp_strlen  = 0;
		cmp_lastchr = 0;
	}
	strcmp_t::strcmp_t(int len, int chr)
	{
		cmp_strlen  = len;
		cmp_lastchr = chr;
	}

	strcmp_t& strcmp_t::operator =(const strcmp_t &rhv)
	{
		cmp_strlen  = rhv.cmp_strlen;
		cmp_lastchr = rhv.cmp_lastchr;
	}

	int strcmp_t::StringLength() const
	{
		return cmp_strlen;
	}
	int strcmp_t::LastCharacter() const
	{
		return cmp_lastchr;
	}

	bool strcmp_t::LengthGreater() const
	{
		return cmp_strlen > 0;
	}
	bool strcmp_t::LengthLess() const
	{
		return cmp_strlen < 0;
	}
	bool strcmp_t::LengthGreaterEq() const
	{
		return cmp_strlen >= 0;
	}
	bool strcmp_t::LengthLessEq() const
	{
		return cmp_strlen <= 0;
	}
	bool strcmp_t::LengthEqual() const
	{
		return cmp_strlen == 0;
	}
	bool strcmp_t::LengthUnequal() const
	{
		return cmp_strlen != 0;
	}

	bool strcmp_t::CharGreater() const
	{
		return cmp_lastchr > 0;
	}
	bool strcmp_t::CharLess() const
	{
		return cmp_lastchr < 0;
	}
	bool strcmp_t::CharGreaterEq() const
	{
		return cmp_lastchr >= 0;
	}
	bool strcmp_t::CharLessEq() const
	{
		return cmp_lastchr <= 0;
	}
	bool strcmp_t::CharEqual() const
	{
		return cmp_lastchr == 0;
	}
	bool strcmp_t::CharUnequal() const
	{
		return cmp_lastchr != 0;
	}

	bool strcmp_t::Equal() const
	{
		return cmp_strlen == 0 && cmp_lastchr == 0;
	}
	bool strcmp_t::Unequal() const
	{
		return cmp_strlen != 0 || cmp_lastchr != 0;
	}

	bool strcmp_t::operator ==(const strcmp_t &rhv) const
	{
		return rhv.cmp_strlen == cmp_strlen && rhv.cmp_lastchr == cmp_lastchr;
	}
	bool strcmp_t::operator !=(const strcmp_t &rhv) const
	{
		return rhv.cmp_strlen != cmp_strlen || rhv.cmp_lastchr != cmp_lastchr;
	}
}


