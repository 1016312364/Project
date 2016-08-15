/***********************************************************************************
	File name:  Alloc.h
	Author:     common
	Created:    2016-7-2  10:36:07
	
	Purpose:    �ռ���������ʵ��
	Addition:   
***********************************************************************************/
#pragma once

// * ���������__USE_MALLOC�ͽ�__malloc_alloc_template<0>��ΪĬ��������  
// * ����ʹ��__default_alloc_template��ΪĬ��������  

//#define __USE_MALLOC


typedef void (*FUNC_HANDLER)();                // * ����ָ����ض���

/* * func: һ���ռ�������(�������ڴ���������ͷţ�ֱ�������ϵͳ���н���) */
template<int inst>
class __MallocAllocTemplate
{
private:
	static FUNC_HANDLER MallocAllocOomHandler;    // * ��Ա����(Ĭ��Ϊ0��������������ָ�룬ִ����Ӧ�ĺ��������ܻ��ò���ϵͳ�ڳ�һЩ�ڴ�)

	static void *OomMalloc(size_t n);
	//static void *OomRealloc(void *p, size_t n);

public:
	static void *Allocate(size_t n)
	{
		void *result = malloc(n);    // * �����Լ�ȥ����mallocȥ���룬���벻���Ż�ȥ����OomMalloc(�������Լ����õĴ�������Ӧ�û����뵽)
		if (0 == result)
			result = OomMalloc(n);

		return result;
	}

	static void Deallocate(void *p, size_t n)
	{
		free(p);
	}

	//static void *Reallocate(void *p, size_t /* old_sz */, size_t new_sz);

	//static void(*set_malloc_handler(void(*f)()))()
	// * ��̬����ȥ��ʼ����̬����(һ������ָ��)
	static FUNC_HANDLER SetMallocHandler(FUNC_HANDLER f)
	{
		FUNC_HANDLER old = MallocAllocOomHandler;
		MallocAllocOomHandler = f;
		return old;
	}
};  /* end class __MallocAllocTemplate */


template<int inst>
FUNC_HANDLER __MallocAllocTemplate<inst>::MallocAllocOomHandler = 0;    // * ���þ�̬��Ա������Ĭ��Ϊ0


/* * func:  ���MallocAllocOomHandlerû�б�����(Ĭ����Ϊ�յ�),������������쳣��
			����������ˣ��ͻ�ȥ�������õĺ���ȥ�ò���ϵͳ�ͷ�һЩ�ڴ棬Ȼ���ٴ�malloc */
template<int inst>
void *__MallocAllocTemplate<inst>::OomMalloc(size_t n)
{
	FUNC_HANDLER MyMallocHandler;
	void *result;
	while (1)
	{
		// * ���õ�MallocAllocOomHandler����Ҫ�ܡ�������Ϊ������Ȼ���ڴ治��ʱ�����ڴ�������������ѭ����
		MyMallocHandler = MallocAllocOomHandler;
		if (0 == MyMallocHandler)
			throw bad_alloc(/*"�ڴ治�㴦����û������"*/);
		// * ����ֱ�����쳣������Ϊ֮ǰ�Ѿ�������malloc��û�з��䵽�ڴ�Ž��뵽������������û�����ô��������Ϳ���ֱ�����쳣��

		(*MyMallocHandler)();    // * ���ô�������ϣ������ϵͳ�ܿճ�һЩ�ڴ�

		result = malloc(n);
		if (result)
			return result;
	}
}

typedef __MallocAllocTemplate<0> MallocAlloc;


template<class T, class Alloc>
class SimpleAlloc
{
public:
	static T* Allocate()    // * ���뵥������Ŀռ�
	{
		return (T*)Alloc::Allocate(sizeof(T));
	}
	static T* Allocate(size_t n)    // * ����n������Ŀռ�
	{
		return 0 == n ? 0 : (T*)Alloc::Allocate(n*sizeof(T));
	}
	static void Deallocate(T* p)    // * �ͷŵ�������
	{
		Alloc::Deallocate(p, sizeof(T));
	}
	static void Deallocate(T* p, size_t n)    // * �ͷ�n������
	{
		Alloc::Deallocate(p, n*sizeof(T));
	}
};

#ifdef __USE_MALLOC

typedef MallocAlloc Alloc;

#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////

// * �ڶ��� �ռ�������
template<int inst>
class __DefaultAllocTemplate    // * ��������������ݼ��������Ǿ�̬�ģ���ֻ��һ����װ
{
public:
	static void *Allocate(size_t bytes)
	{
		Obj *result = NULL;

		// * ������ڴ�̫�󣬵���һ���ռ�������
		if (bytes > (size_t)_MAX_BYTES)
			result = (Obj*)MallocAlloc::Allocate(bytes);

		// * ���������ڴ�С�� _MAX_ALIGN���������������ȡ
		size_t index = FREELIST_INDEX(bytes);
		result = _free_list[index];

		if (0 == result)
		{
			void *r = _Refill(ROUND_UP(bytes));
			return r;
		}
		// ��result �� result->free_list_link�Ĳ�ֵ

		_free_list[index] = result->free_list_link;


		return result;
	}

	static void Deallocate(void *p, size_t bytes)
	{
		if (bytes > 128)
		{
			MallocAlloc::Deallocate(p, bytes);
			return;
		}

		size_t index = FREELIST_INDEX(bytes);
		(((Obj *)p)->free_list_link) = _free_list[index];
		_free_list[index] = (Obj*)p;
	}

public:
	static size_t ROUND_UP(size_t bytes)
	{
		return (((bytes)+_ALIGN - 1) & ~(_ALIGN - 1));    // * �Ѳ���_ALIGN��λ�����������ȡ����С��_ALIGN�ı���
	}

	static size_t FREELIST_INDEX(size_t bytes)
	{
		return ((bytes)+_ALIGN - 1) / (_ALIGN) - 1;       // * 
	}

private:
/* * func: ���ڴ����ȡ����(20)���ڴ�飬��һ�鷵�ظ��û�������ҵ���Ӧ������������ */
	static void *_Refill(size_t bytes)
	{
		size_t nobjs = 20;    // * Ĭ��һ��ȡ20������Ŀռ�
		char *chunk = _ChunkAlloc(bytes, nobjs);
		Obj *result;
		Obj **myFreeList;

		if (1 == nobjs)    // * 
			return chunk;

		//myFreeList = _free_list + FREELIST_INDEX(bytes);
		size_t index = FREELIST_INDEX(bytes);
		myFreeList = _free_list + index;

		result = (Obj *)chunk;
		Obj *cur_obj = NULL;
		Obj *next_obj = (Obj *)(chunk + bytes);      // * chunk��������char *, ����bytes��ָ����һ���ڵ����ʼ��ַ
		*myFreeList = next_obj;
		for (size_t i = 1; i < nobjs; ++i)      // * �Ѵ��ڴ���ж�ȡ���ڴ�ҵ���Ӧ������������
		{
			cur_obj = next_obj;
			next_obj = (Obj *)((char *)next_obj + bytes);   //next_obj = next_obj + 1;

			cur_obj->free_list_link = next_obj;
		}
		cur_obj->free_list_link = 0;          // * �����һ���ڵ�� _freeListLink �ÿ�

		return result;
	}

/* * func: ��ô�õ��㹻����ڴ潻���������� */
	static char *_ChunkAlloc(size_t size, size_t &nobjs)
	{
		char *result = NULL;
		size_t total_size = size * nobjs;
		size_t bytes_left_memory_poll = _end_free - _start_free;    // * char *���͵�ָ��ֱ��������õ�ʣ���ڴ���ֽ���

		// * 1 �ڴ���е�ʣ���ڴ��㹻����nobjs������Ŀռ�
		if (bytes_left_memory_poll >= total_size)
		{
			result = _start_free;
			_start_free += total_size;
			return result;
		}
		// * 2 �ڴ����ʣ���ڴ治������nobjs������Ŀռ䣬���������ܷ���һ����СΪsize�Ŀռ�
		else if (bytes_left_memory_poll > size)
		{
			nobjs = bytes_left_memory_poll / size; // * �޸��ܷ���Ķ���ռ����
			total_size = size * nobjs;             // * �����޸��ܵ��ֽ���

			result = _start_free;                  // * ����ֵ
			_start_free += total_size;             // * �޸��ڴ����ʼ��ַ
			return result;
		}
		// * 3 �ڴ���е�ʣ���ڴ���һ������Ŀռ䶼�ֲ�������
		else
		{
			// * �ڴ���п��ܻ���һЩ��ͷ������Щ�ڴ���䵽��Ӧ������������
			if (bytes_left_memory_poll > 0)
			{
				size_t index = FREELIST_INDEX(bytes_left_memory_poll);

				// * ����_free_list�����ڴ����ʣ����ڴ�ҵ�����������
				((Obj *)_start_free)->free_list_link = _free_list[index];
				_free_list[index] = (Obj *)_start_free;
			}

			// * �ڴ���ڴ治��Ľ������ ......
			// * ������� 1 �� �Ҷ�(����ϵͳ)Ҫ
			size_t bytes_to_get = 2 * total_size + ROUND_UP(_heap_size >> 4);    // * ����������
			_start_free = (char *)malloc(bytes_to_get);
			if (0 == _start_free)
			{// * �ѿռ��ڴ治�㣬 mallocʧ��

			 // * ������� 2 ��Ҫ�����������м����ϴ�����飬
				size_t index = 0;
				for (size_t i = size; i < _MAX_BYTES; i += _ALIGN)
				{
					index = FREELIST_INDEX(i);
					Obj *tmp = _free_list[index];
					if (tmp)    // * index�µ�����������
					{
						Obj *first_obj = tmp;
						_free_list[index] = first_obj->free_list_link;    // * �ѵ�һ�����յ���������ĵ�һ���ڵ���tmpȡ���������������ڵ���������

						_start_free = (char*)tmp;
						_end_free = _start_free + i;          // * ��Ϊ_start_free��char*���͵�ָ��

						return _ChunkAlloc(size, nobjs);       // * ���ʱ��nobjs��Ȼû�иı䣬�ݹ��ȥ��ʱ��(�����Ҫ)��ı��
					}
				}  /* for(; i < _MAX_BYTES; ) */

				// * ������������û���ҵ��㹻��Ŀ�(��������)���ͻ��ߵ�����
				// * ������� 3 ������һ���ռ��������ܲ��ܳ�����(�Լ��ռ����������ָ�����ڴ治��ʱ�Ĵ����������п��ܿճ��ڴ�)

				// * ��������һ���ռ�������������쳣������Ҫ֮ǰ����_end_free����������ã��´ν���_ChunkAllocʱ��_start_free == 0, _end_freeû�ģ��ڴ�ػ�ܴ���ʵ������
				_end_free = 0;
				_start_free = (char*)MallocAlloc::Allocate(bytes_to_get);

			}  /* if 0 == _start_free */

			_heap_size += bytes_to_get;       // * ����������
			_end_free = _start_free + bytes_to_get;
			
			return _ChunkAlloc(size, nobjs);
		}  /* else */
	}  /* end _ChunkAlloc */

private:
	enum { _ALIGN = 8 };                            // * free_list����ڵ����С��λ
	enum { _MAX_BYTES = 128 };                      // * free_list����ڵ�����ֵ
	enum { _N_FREE_LIST = _MAX_BYTES / _ALIGN };    // * ��������ĸ���

	typedef union Obj
	{
		union Obj* free_list_link;   // * ָ����������ڵ��ָ��
		char client_data[1];         // * �ͻ�����
	}Obj;

	// * ��������
	static Obj* _free_list[_N_FREE_LIST];

	// * �ڴ��
	static char * _start_free;
	static char * _end_free;
	static size_t _heap_size;    // * ����������
};

// * ��ʼ��__DefaultAllocTemplate�еľ�̬��Ա
template<int inst>
typename __DefaultAllocTemplate<inst>::Obj * __DefaultAllocTemplate<inst>::_free_list[__DefaultAllocTemplate<inst>::_N_FREE_LIST];
template<int inst>
char * __DefaultAllocTemplate<inst>::_start_free = 0;
template<int inst>
char * __DefaultAllocTemplate<inst>::_end_free = 0;
template<int inst>
size_t __DefaultAllocTemplate<inst>::_heap_size = 0;


#ifndef __USE_MALLOC

typedef __DefaultAllocTemplate<0> Alloc;

#endif
