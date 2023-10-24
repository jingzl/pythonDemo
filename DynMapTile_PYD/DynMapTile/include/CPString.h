
#pragma once

#include <assert.h>
#include <string.h>
#include "DataType.h"
#include "Libc.h"
/*	The support for explicit isn't that universal, and it isn't really
required - it is used to check that the CCPString class isn't incorrectly
used. Be nice to old compilers and macro it here:
*/
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
#define CPCSTR_EXPLICIT explicit
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
#define CPCSTR_EXPLICIT explicit
#else
#define CPCSTR_EXPLICIT
#endif

class CCPString
{
public :
	// The size type used
	typedef ZUINT64 size_type;

	// Error value for find primitive
	static const size_type npos; // = -1;

	// CCPString empty constructor
	CCPString () : rep_(&nullrep_)
	{
	}

	// CCPString copy constructor
	CCPString ( const CCPString & copy) : rep_(0)
	{
		init(copy.length());
		memcpy(start(), copy.data(), length());
	}

	// CCPString constructor, based on a string
	CPCSTR_EXPLICIT CCPString ( const ZCHAR * copy) : rep_(0)
	{
		init( static_cast<size_type>( strlen(copy) ));
		memcpy(start(), copy, length());
	}

	// CCPString constructor, based on a string
	CPCSTR_EXPLICIT CCPString ( const ZCHAR * str, size_type len) : rep_(0)
	{
		init(len);
		memcpy(start(), str, len);
	}

	// CCPString destructor
	~CCPString ()
	{
		quit();
	}

	// = operator
	CCPString& operator = (const ZCHAR * copy)
	{
		return assign( copy, (size_type)strlen(copy));
	}

	// = operator
	CCPString& operator = (const CCPString & copy)
	{
		return assign(copy.start(), copy.length());
	}


	// += operator. Maps to append
	CCPString& operator += (const ZCHAR * suffix)
	{
		return append(suffix, static_cast<size_type>( strlen(suffix) ));
	}

	// += operator. Maps to append
	CCPString& operator += (ZCHAR single)
	{
		return append(&single, 1);
	}

	// += operator. Maps to append
	CCPString& operator += (const CCPString & suffix)
	{
		return append(suffix.data(), suffix.length());
	}


	// Convert a CCPString into a null-terminated char *
	const ZCHAR * c_str () const { return rep_->str; }

	// Convert a CCPString into a char * (need not be null terminated).
	const ZCHAR * data () const { return rep_->str; }

	// Return the length of a CCPString
	size_type length () const { return rep_->size; }

	// Alias for length()
	size_type size () const { return rep_->size; }

	// Checks if a CCPString is empty
	ZBOOL empty () const { return rep_->size == 0; }

	// Return capacity of string
	size_type capacity () const { return rep_->capacity; }


	// single char extraction
	const ZCHAR& at (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}

	// [] operator
	ZCHAR& operator [] (size_type index) const
	{
		assert( index < length() );
		return rep_->str[ index ];
	}

	// find a char in a string. Return CCPString::npos if not found
	size_type find (ZCHAR lookup) const
	{
		return find(lookup, 0);
	}

	// find a char in a string from an offset. Return CCPString::npos if not found
	size_type find (ZCHAR tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		for (const ZCHAR* p = c_str() + offset; *p != '\0'; ++p)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}

	size_type find (const CCPString strfind) const
	{
		return find(strfind, 0);
	}

	size_type find(const CCPString strfind, size_type offset) const
	{
		if (offset >= length()) return npos;
		const ZCHAR* p = strstr(c_str(), strfind.c_str());
		if (NULL == p)
			return npos;
        return p - c_str();
	}

	size_type refind (ZCHAR lookup) const
	{
		return refind(lookup, 0);
	}

	size_type refind (ZCHAR tofind, size_type offset) const
	{
		if (offset >= length()) return npos;

		size_type sizeoffset = length() - offset;
		for (const ZCHAR* p = finish(); sizeoffset != npos; --p, --sizeoffset)
		{
			if (*p == tofind) return static_cast< size_type >( p - c_str() );
		}
		return npos;
	}

	size_type refind (const CCPString strfind) const
	{
		return refind(strfind, 0);
	}

	size_type refind(const CCPString strfind, size_type offset) const
	{
        if (offset >= length()) return npos;
		const ZCHAR* ps = c_str() + offset;
        const ZCHAR* p = strstr(ps, strfind.c_str());
		if (NULL == p) return npos;
		while (p)
		{
			p += strfind.size();
			ps = p;
			p = strstr(ps, strfind.c_str());
		}
		return ps - c_str() - strfind.size();
	}

	CCPString substr(size_type offset, size_type num = npos) const
	{
        if (offset >= length()) return CCPString("");
        if (num == npos || num > length()) num = length() - offset;
        CCPString tmp;
		tmp.assign(c_str() + offset, num);
		return tmp;
	}

	ZVOID clear ()
	{
		//Lee:
		//The original was just too strange, though correct:
		//	CCPString().swap(*this);
		//Instead use the quit & re-init:
		quit();
		init(0,0);
	}

	/*	Function to reserve a big amount of data when we know we'll need it. Be aware that this
	function DOES NOT clear the content of the CCPString if any exists.
	*/
	ZVOID reserve (size_type cap);

	CCPString& assign (const ZCHAR* str, size_type len);

	CCPString& append (const ZCHAR* str, size_type len);

	ZVOID swap (CCPString& other)
	{
		Rep* r = rep_;
		rep_ = other.rep_;
		other.rep_ = r;
	}

private:

	ZVOID init(size_type sz) { init(sz, sz); }
	ZVOID set_size(size_type sz) { rep_->str[ rep_->size = sz ] = '\0'; }
	ZCHAR* start() const { return rep_->str; }
	ZCHAR* finish() const { return rep_->str + rep_->size; }

	struct Rep
	{
	    size_type size, capacity;
	    ZCHAR str[1];
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
			const size_type bytesNeeded = sizeof(Rep) + cap;
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


inline ZBOOL operator == (const CCPString & a, const CCPString & b)
{
	return ( a.length() == b.length() )				// optimization on some platforms
		&& ( strcmp(a.c_str(), b.c_str()) == 0 );	// actual compare
}
inline ZBOOL operator < (const CCPString & a, const CCPString & b)
{
	return strcmp(a.c_str(), b.c_str()) < 0;
}

inline ZBOOL operator != (const CCPString & a, const CCPString & b) { return !(a == b); }
inline ZBOOL operator >  (const CCPString & a, const CCPString & b) { return b < a; }
inline ZBOOL operator <= (const CCPString & a, const CCPString & b) { return !(b < a); }
inline ZBOOL operator >= (const CCPString & a, const CCPString & b) { return !(a < b); }

inline ZBOOL operator == (const CCPString & a, const ZCHAR* b) { return strcmp(a.c_str(), b) == 0; }
inline ZBOOL operator == (const ZCHAR* a, const CCPString & b) { return b == a; }
inline ZBOOL operator != (const CCPString & a, const ZCHAR* b) { return !(a == b); }
inline ZBOOL operator != (const ZCHAR* a, const CCPString & b) { return !(b == a); }

CCPString operator + (const CCPString & a, const CCPString & b);
CCPString operator + (const CCPString & a, const ZCHAR* b);
CCPString operator + (const ZCHAR* a, const CCPString & b);
