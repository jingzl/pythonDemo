
#pragma once

#include <assert.h>
#include "DataType.h"
#include "Libc.h"
/*	The support for explicit isn't that universal, and it isn't really
required - it is used to check that the CCPWString class isn't incorrectly
used. Be nice to old compilers and macro it here:
*/
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
#define CPCWSTR_EXPLICIT explicit
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
#define CPCWSTR_EXPLICIT explicit
#else
#define CPCWSTR_EXPLICIT
#endif

class CCPWString
{
public :
	// The size type used
	typedef ZUINT64 size_type;

	// Error value for find primitive
	static const size_type npos; // = -1;

	// CCPWString empty constructor
	CCPWString () : rep_(&nullrep_)
	{
	}

	// CCPWString copy constructor
	CCPWString ( const CCPWString & copy) : rep_(0)
	{
		init(copy.length());
		memcpy(start(), copy.data(), length() * sizeof(ZWCHAR));
	}

	// CCPWString constructor, based on a string
	CPCWSTR_EXPLICIT CCPWString ( const ZWCHAR * copy) : rep_(0)
	{
		init( static_cast<size_type>( wcslen(copy)));
		memcpy(start(), copy, length() * sizeof(ZWCHAR));
	}

	// CCPWString constructor, based on a string
	CPCWSTR_EXPLICIT CCPWString ( const ZWCHAR * str, size_type len) : rep_(0)
	{
		init(len);
		memcpy(start(), str, len * sizeof(ZWCHAR));
	}

	// CCPWString destructor
	~CCPWString ()
	{
		quit();
	}

	// = operator
	CCPWString& operator = (const ZWCHAR * copy)
	{
		return assign( copy, (size_type)wcslen(copy));
	}

	// = operator
	CCPWString& operator = (const CCPWString & copy)
	{
		return assign(copy.start(), copy.length());
	}


	// += operator. Maps to append
	CCPWString& operator += (const ZWCHAR * suffix)
	{
		return append(suffix, static_cast<size_type>( wcslen(suffix)));
	}

	// += operator. Maps to append
	CCPWString& operator += (ZWCHAR single)
	{
		return append(&single, 1);
	}

	// += operator. Maps to append
	CCPWString& operator += (const CCPWString & suffix)
	{
		return append(suffix.data(), suffix.length());
	}


	// Convert a CCPWString into a null-terminated char *
	const ZWCHAR * c_str () const { return rep_->str; }

	// Convert a CCPWString into a char * (need not be null terminated).
	const ZWCHAR * data () const { return rep_->str; }

	// Return the length of a CCPWString
	size_type length () const { return rep_->size; }

	// Alias for length()
	size_type size () const { return rep_->size; }

	// Checks if a CCPWString is empty
	ZBOOL empty () const { return rep_->size == 0; }

	// Return capacity of string
	size_type capacity () const { return rep_->capacity; }


	// single char extraction
	const ZWCHAR& at (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}

	// [] operator
	ZWCHAR& operator [] (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}

	// find a char in a string. Return CCPWString::npos if not found
	size_type find (ZWCHAR lookup) const
	{
		return find(lookup, 0);
	}

	// find a char in a string from an offset. Return CCPWString::npos if not found
	size_type find (ZWCHAR tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		for (const ZWCHAR* p = c_str() + offset; *p != '\0'; ++p)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}

	size_type find (const CCPWString strfind) const
	{
		return find(strfind, 0);
	}

	size_type find(const CCPWString strfind, size_type offset) const
	{
		if (offset >= length()) return npos;
		const ZWCHAR* p = wcsstr(c_str(), strfind.c_str());
		if (NULL == p)
			return npos;
		return p - c_str();
	}

	size_type refind (ZWCHAR lookup) const
	{
		return refind(lookup, 0);
	}

	size_type refind (ZWCHAR tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		size_type sizeoffset = length() - offset;
		for (const ZWCHAR* p = finish(); sizeoffset != npos; --p, --sizeoffset)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}

	size_type refind (const CCPWString strfind) const
	{
		return refind(strfind, 0);
	}

	size_type refind(const CCPWString strfind, size_type offset) const
	{
		if (offset >= length()) return npos;
		const ZWCHAR* ps = c_str() + offset;
		const ZWCHAR* p = wcsstr(ps, strfind.c_str());
		if (NULL == p) return npos;
		while (p)
		{
			p += strfind.size();
			ps = p;
			p = wcsstr(ps, strfind.c_str());
		}
		return ps - c_str() - strfind.size();
	}

	CCPWString substr(size_type offset, size_type num = npos) const
	{
		if (offset >= length()) return CCPWString(L"");
		if (num == npos || num > length()) num = length() - offset;
		CCPWString tmp;
		tmp.assign(c_str() + offset, num);
		return tmp;
	}

	ZVOID clear ()
	{
		//Lee:
		//The original was just too strange, though correct:
		//	CCPWString().swap(*this);
		//Instead use the quit & re-init:
		quit();
		init(0,0);
	}

	/*	Function to reserve a big amount of data when we know we'll need it. Be aware that this
	function DOES NOT clear the content of the CCPWString if any exists.
	*/
	ZVOID reserve (size_type cap);

	CCPWString& assign (const ZWCHAR* str, size_type len);

	CCPWString& append (const ZWCHAR* str, size_type len);

	ZVOID swap (CCPWString& other)
	{
		Rep* r = rep_;
		rep_ = other.rep_;
		other.rep_ = r;
	}

private:

	ZVOID init(size_type sz) { init(sz, sz); }
	ZVOID set_size(size_type sz) { rep_->str[ rep_->size = sz ] = '\0'; }
	ZWCHAR* start() const { return rep_->str; }
	ZWCHAR* finish() const { return rep_->str + rep_->size; }

	struct Rep
	{
		size_type size, capacity;
		ZWCHAR str[1];
	};

	ZVOID init(size_type sz, size_type cap)
	{
		if (cap)
		{
			// Lee: the original form:
			//	rep_ = static_cast<Rep*>(operator new(sizeof(Rep) + cap));
			// doesn't work in some cases of new being overloaded. Switching
			// to the normal allocation, although use an 'int' for systems
			// that are overly picky about structure alignment.
			const size_type bytesNeeded = sizeof(Rep) + cap * sizeof(ZWCHAR);
			const size_type intsNeeded = ( bytesNeeded + sizeof(ZINT32) - 1 ) / sizeof( ZINT32 ); 
			//rep_ = reinterpret_cast<Rep*>( new ZINT32[ intsNeeded ] );
			rep_ = new Rep[intsNeeded];

			rep_->str[ rep_->size = sz ] = '\0';
			rep_->capacity = cap;
		}
		else
		{
			rep_ = &nullrep_;
		}
	}

	ZVOID quit()
	{
		if (rep_ && rep_ != &nullrep_ && rep_->size > 0)
		{
			// The rep_ is really an array of ints. (see the allocator, above).
			// Cast it back before delete, so the compiler won't incorrectly call destructors.
			delete [] ( reinterpret_cast<ZINT32*>( rep_ ) );
		}
	}

	Rep * rep_;
	static Rep nullrep_;

} ;


inline ZBOOL operator == (const CCPWString & a, const CCPWString & b)
{
	return ( a.length() == b.length() )				// optimization on some platforms
		&& ( wcscmp(a.c_str(), b.c_str()) == 0 );	// actual compare
}
inline ZBOOL operator < (const CCPWString & a, const CCPWString & b)
{
	return wcscmp(a.c_str(), b.c_str()) < 0;
}

inline ZBOOL operator != (const CCPWString & a, const CCPWString & b) { return !(a == b); }
inline ZBOOL operator >  (const CCPWString & a, const CCPWString & b) { return b < a; }
inline ZBOOL operator <= (const CCPWString & a, const CCPWString & b) { return !(b < a); }
inline ZBOOL operator >= (const CCPWString & a, const CCPWString & b) { return !(a < b); }

inline ZBOOL operator == (const CCPWString & a, const ZWCHAR* b) { return wcscmp(a.c_str(), b) == 0; }
inline ZBOOL operator == (const ZWCHAR* a, const CCPWString & b) { return b == a; }
inline ZBOOL operator != (const CCPWString & a, const ZWCHAR* b) { return !(a == b); }
inline ZBOOL operator != (const ZWCHAR* a, const CCPWString & b) { return !(b == a); }

CCPWString operator + (const CCPWString & a, const CCPWString & b);
CCPWString operator + (const CCPWString & a, const ZWCHAR* b);
CCPWString operator + (const ZWCHAR* a, const CCPWString & b);
