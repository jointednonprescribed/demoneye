


/* Implementing: demoneye/string.hpp */
#include <demoneye/string.hpp>

/* Source Requirement: demoneye/logger.hpp */
#include <demoneye/logger.hpp>



namespace demoneye
{
	const overflow_error STRFUNC_ERR_OVERFLOW_DEST("Destination buffer is too small for this string.");
	const overflow_error STRFUNC_ERR_OVERFLOW_COPY("Copy size is larger than the source string.");

	strcmp_t cmpstr(const char *s1, const char *s2)
	{
		strcmp_t cmp;

		char c1, c2;
		size_t i = 0;
		for (;; i++) {
			c1 = s1[i];
			c2 = s2[i];

			if (c1 != c2) {
				cmp.cmp_lastchr = (c1 > c2) - (c1 < c2);
				goto check_len;
			} else if (c1 == 0) {
				cmp.cmp_lastchr = 0;
				cmp.cmp_strlen  = 0;
				return cmp;
			}
		}
	check_len:
		if (c1 == 0) {
			cmp.cmp_strlen = -(c2 != 0);
			return cmp;
		} else if (c2 == 0) {
			cmp.cmp_strlen =  (c1 != 0);
			return cmp;
		}

		i++;
		c1 = s1[i];
		c2 = s2[i];
		goto check_len;
	}
	strcmp_t cmpstr(const char *s1, const char *s2, size_t n)
	{
		strcmp_t cmp;

		const size_t lim = n-1;

		char c1, c2;
		size_t i = 0;
		for (;; i++) {
			c1 = s1[i];
			c2 = s2[i];

			if (c1 != c2) {
				cmp.cmp_lastchr = (c1 > c2) - (c1 < c2);
				goto check_len;
			} else if (c1 == 0 || i == lim) {
				cmp.cmp_lastchr = 0;
				cmp.cmp_strlen  = 0;
				return cmp;
			}
		}
	check_len:
		if (i == lim) {
			cmp.cmp_strlen = 0;
			return cmp;
		} else if (c1 == 0) {
			cmp.cmp_strlen = -(c2 != 0);
			return cmp;
		} else if (c2 == 0) {
			cmp.cmp_strlen =  (c1 != 0);
			return cmp;
		}

		i++;
		c1 = s1[i];
		c2 = s2[i];
		goto check_len;
	}

	strcmp_t cmpstr(const wchar_t *s1, const wchar_t *s2)
	{
		strcmp_t cmp;

		wchar_t c1, c2;
		size_t i = 0;
		for (;; i++) {
			c1 = s1[i];
			c2 = s2[i];

			if (c1 != c2) {
				cmp.cmp_lastchr = (c1 > c2) - (c1 < c2);
				goto check_len;
			} else if (c1 == 0) {
				cmp.cmp_lastchr = 0;
				cmp.cmp_strlen  = 0;
				return cmp;
			}
		}
	check_len:
		if (c1 == 0) {
			cmp.cmp_strlen = -(c2 != 0);
			return cmp;
		} else if (c2 == 0) {
			cmp.cmp_strlen =  (c1 != 0);
			return cmp;
		}

		i++;
		c1 = s1[i];
		c2 = s2[i];
		goto check_len;
	}
	strcmp_t cmpstr(const wchar_t *s1, const wchar_t *s2, size_t n)
	{
		strcmp_t cmp;

		const size_t lim = n-1;

		wchar_t c1, c2;
		size_t i = 0;
		for (;; i++) {
			c1 = s1[i];
			c2 = s2[i];

			if (c1 != c2) {
				cmp.cmp_lastchr = (c1 > c2) - (c1 < c2);
				goto check_len;
			} else if (c1 == 0 || i == lim) {
				cmp.cmp_lastchr = 0;
				cmp.cmp_strlen  = 0;
				return cmp;
			}
		}
	check_len:
		if (i == lim) {
			cmp.cmp_strlen = 0;
			return cmp;
		} else if (c1 == 0) {
			cmp.cmp_strlen = -(c2 != 0);
			return cmp;
		} else if (c2 == 0) {
			cmp.cmp_strlen =  (c1 != 0);
			return cmp;
		}

		i++;
		c1 = s1[i];
		c2 = s2[i];
		goto check_len;
	}

	char* cpystr(char *dest, size_t destsz, const char *src) noexcept(false)
	{
		const size_t l = strlen(src);

		if (l >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		char c = src[0];
		for (size_t i = 0; i < destsz && c != 0; i++, c = src[i])
			dest[i] = c;

		return dest;
	}
	char* cpystrn(char *dest, size_t destsz, const char *src, size_t cpysz) noexcept(false)
	{
		const size_t l = strlen(src);

		if (l < cpysz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_COPY);
			return NULL;
		} else if (cpysz >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		char c = src[0];
		for (size_t i = 0; i < cpysz; i++, c = src[i])
			dest[i] = c;

		return dest;
	}

	wchar_t* cpywstr(wchar_t *dest, size_t destsz, const wchar_t *src) noexcept(false)
	{
		const size_t l = wstrlen(src);

		if (l >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		wchar_t c = src[0];
		for (size_t i = 0; i < destsz && c != 0; i++, c = src[i])
			dest[i] = c;

		return dest;
	}
	wchar_t* cpywstrn(wchar_t *dest, size_t destsz, const wchar_t *src, size_t cpysz) noexcept(false)
	{
		const size_t l = wstrlen(src);

		if (l < cpysz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_COPY);
			return NULL;
		} else if (cpysz >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		wchar_t c = src[0];
		for (size_t i = 0; i < cpysz; i++, c = src[i])
			dest[i] = c;

		return dest;
	}

	wchar_t* strtowcs(wchar_t *__restrict__ dest, size_t destsz, const char *__restrict__ src) noexcept(false)
	{
		const size_t l = strlen(src);

		if (l >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		// PERFORM CONVERSION

		return dest;
	}
	wchar_t* strtowcsn(wchar_t *__restrict__ dest, size_t destsz, const char *__restrict__ src, size_t cpysz)
		noexcept(false)
	{
		const size_t l = strlen(src);

		if (l < cpysz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_COPY);
			return NULL;
		} else if (cpysz >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		// PERFORM CONVERSION

		return dest;
	}
	char* wcstostr(char *__restrict__ dest, size_t destsz, const wchar_t *__restrict__ src) noexcept(false)
	{
		const size_t l = wstrlen(src);

		if (l >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		// PERFORM CONVERSION

		return dest;
	}
	char* wcstostrn(char *__restrict__ dest, size_t destsz, const wchar_t *__restrict__ src, size_t cpysz)
		noexcept(false)
	{
		const size_t l = wstrlen(src);

		if (l < cpysz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_COPY);
			return NULL;
		} else if (cpysz >= destsz) {
			throw logerr(STRFUNC_ERR_OVERFLOW_DEST);
			return NULL;
		}

		// PERFORM CONVERSION

		return dest;
	}
}


