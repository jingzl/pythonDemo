
// smallobject_allocator.h: interface for the smallobject_allocator class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SMALLOBJECT_ALLOCATOR_H_
#define _SMALLOBJECT_ALLOCATOR_H_
#include <new>
#include "Platform.h"
class smallobject_allocator  
{
public:
	enum{
		chunk_limit=16384,	//一次分配内存的最大值 16K
		max_number=64,		//一次性分配内存单元个数
		align_size=4,		//内存对齐大小
		chunk_number = chunk_limit/align_size, //4K
	};
public:
	~smallobject_allocator();
	
	static smallobject_allocator& instance(); 
	ZVOID* allocate(size_t size);
	ZVOID deallocate(ZPVOID p, size_t size);
	
private:
	struct memory_list {
		memory_list* _next;
	};
	struct chunk_list{
		chunk_list* _next;
		memory_list* _data;
	};

	memory_list* _free_list[chunk_number];
	ZINT32 _guard[chunk_number];

	chunk_list* _chunk_list;
	ZINT32 _chunk_guard;

	static smallobject_allocator* _instance;
	static ZINT32 _singleton_guard;

	static ZBOOL _singleton_destroyed;
	static ZVOID create_instance();

	inline size_t chunk_index(size_t bytes);
	smallobject_allocator();
	memory_list* alloc_chunk(size_t idx);
};

//#ifdef __USE_SMALLOBJECT_ALLOC

#define SMALL_MAX_SIZE  128  //使用小内存分配器的分配内存最大限制

inline ZVOID* smalloc(ZUINT64 size)
{
	ZUINT32* prec;
	if(size>SMALL_MAX_SIZE)
		prec = (ZUINT32*)::malloc(4+size);
	else
		prec = reinterpret_cast<ZUINT32*>(smallobject_allocator::instance().allocate(4+size));
	*prec=size+4;
	return ++prec;
}

inline ZVOID sfree(ZPVOID p)
{
	ZUINT32* pun = reinterpret_cast<ZUINT32*>(p);
	if(*(--pun)>SMALL_MAX_SIZE)
		::free(pun);
	else
		smallobject_allocator::instance().deallocate(pun, (*pun));
}

//#endif

#endif