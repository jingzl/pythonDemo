
// hugeobject_allocator.h: interface for the hugeobject_allocator class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HUGEOBJECT_ALLOCATOR_H_
#define _HUGEOBJECT_ALLOCATOR_H_

#include "Platform.h"

#define	MAXLEN		(64*1024*1024)
#define	ALIGN_SIZE	(8)

class hugeobject_allocator  
{
public:
	~hugeobject_allocator();

	static hugeobject_allocator& instance();

	ZVOID* allocate(ZUINT64 size);
	ZVOID deallocate(const ZPVOID p);

private:
	struct chunk_list{
		chunk_list*	 _next;
		CCPMapFile*	 _handle;
		ZPVOID       _pbaseptr;
	};

	chunk_list* _chunk_list;
	chunk_list* _chunk_last;

	ZINT32 _chunk_guard;	
	static hugeobject_allocator* _instance;	
	static ZVOID create_instance();
	hugeobject_allocator();

};

#define HUGE_MIN_SIZE  4096  //使用大内存分配器的分配内存最小限制

inline ZVOID* hmalloc(ZUINT64 size)
{
	ZUINT32* prec;
	if(size<HUGE_MIN_SIZE)
		prec = (ZUINT32*)::malloc(4+size);
	else
		prec = reinterpret_cast<ZUINT32*>(hugeobject_allocator::instance().allocate(4+size));

	if(prec == NULL)
	{
		ZLOGE(TAG_UTIL," prec == NULL ");
		return NULL;
	}
	*prec=(ZUINT32)size;
	return ++prec;
}
inline ZVOID hfree(ZPVOID p)
{
	ZUINT32* pun = reinterpret_cast<ZUINT32*>(p);
	if (NULL == pun)
	{
		return ;
	}
	if(*(--pun)<HUGE_MIN_SIZE)
		::free(pun);
	else
		hugeobject_allocator::instance().deallocate(pun);
}

#endif
