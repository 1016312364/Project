/***********************************************************************************
	File name:  List.h
	Author:     common
	Created:    2016-6-27  22:34:54
	
	Purpose:    ģ��STL�� list ��ʵ��
	Addition:   
***********************************************************************************/
#pragma once

#include <stddef.h>    // * ptrdiff_t

#include "Allocate.h"
#include "Construct.h"
#include "Iterator.h"

// * ����ڵ�
template<class T>
struct __ListNode
{ 
	T                     _data;     // * ֵ����
	struct __ListNode<T>* _next;     // * ָ����һ���ڵ�
	struct __ListNode<T>* _prev;     // * ָ��ǰһ���ڵ�

	__ListNode(const T& data, struct __ListNode<T>* next = NULL, struct __ListNode<T> *prev = NULL)
		:_data(data)
		, _next(next)
		, _prev(prev)
	{}
};


// * ����ĵ�����
template<class T, class Ref, class Ptr>
struct __ListIterator
{
	typedef __ListIterator<T, T&, T*> Iterator;
	typedef __ListIterator<T, const T&, const T*> ConstIterator;
	typedef __ListIterator<T, Ref, Ptr> Self;

	// * �������������ͱ�
	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptr Pointer;
	typedef Ref Reference;
	typedef ptrdiff_t DifferenceType;

	// * �ض���ָ������
	typedef __ListNode<T>* LinkType;

	typedef size_t SizeType;
	typedef ptrdiff_t Difference;

	// * �������캯��

	__ListIterator():_node(NULL) {}
	// * ��һ���ڵ��ָ�빹��
	__ListIterator(LinkType x):_node(x) {}
	// * ��������
	__ListIterator(const Iterator &x):_node(x._node) {}

	// * �������ļ�������
	bool operator==(const Self &x) { return _node == x._node; }
	bool operator!=(const Self &x) { return !operator==(x); }

	Reference operator*() { return (*_node)._data; }
	Pointer operator->() { return &(operator*()); }

	Self& operator++()    // * ǰ��++
	{
		_node = (*_node)._next;
		return *this;
	}
	Self operator++(int)
	{
		Self tmp = *this;
		++(*this);
		return tmp;
	}

	Self& operator--()    // * ǰ��--
	{
		_node = (*_node)._prev;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp = *this;
		(*this)--;
		return *this;
	}

	// * �����������ݳ�Ա(һ��ָ������ڵ��ָ��)
	LinkType _node;
};



template <class T, class Ref, class Ptr>
inline BidirectionalIteratorTag
IteratorCategory(const __ListIterator<T, Ref, Ptr>&)      // ****** only for List
{
	return BidirectionalIteratorTag();
}


template <class T, class alloc = Alloc>
class List
{
	typedef __ListNode<T> ListNode;

public:
	typedef T ValueType;

	typedef ValueType Pointer;
	typedef const ValueType ConstPointer;

	typedef ValueType&  Reference;
	typedef const ValueType& ConstReference;

	typedef __ListNode<T> ListNode;
	typedef ListNode* LinkType;

	typedef SimpleAlloc<ListNode, alloc> ListNodeAllocator;    // * list��ר���ռ�������������ڵ���

	typedef size_t SizeType;
	typedef ptrdiff_t DifferenceType;

	// * ������
	typedef typename __ListIterator<T, T&, T*>::Iterator Iterator;
	typedef typename __ListIterator<T, const T&, const T*>::ConstIterator ConstIterator;

	// * ���������
	typedef ReverseIterator<ConstIterator> ConstReverseIterator;
	typedef ReverseIterator<Iterator> ReverseIterator;


protected:
	LinkType _node;    // ****** ����ľ����һ��ͷ�ڵ��ָ��

public:
	// * ������������(���죬�������������죬��ֵ�����)

	List() { EmptyInitialize(); }
	List(SizeType n, const T& value) { FillInitialize(Begin(), n, value); }
	// * ��ֻ��һ�����ݳ�Ա���࣬�������Ĺ��캯�����׷�����ʿת����ǰ���explicit���Է�ֹ�������
	explicit List(SizeType n) { FillInitialize(Begin(), n, T()); }

	// * ���������������һ�����乹��һ��������
	List(ConstIterator first, ConstIterator last) { RangeInitialize(first, last); };
	// * �����������һ�����乹��һ��������
	List(const T* first, const T* last) { RangeInitialize(first, last); }
	// * ���������͵��������乹��һ��������
	template<class InputIterator>
	List(InputIterator first, InputIterator last) { RangeInitialize(first, last); }

	// * ��������
	List(const List<T> &x)
	{
		EmptyInitialize();
		Insert(Begin(), x.Begin(), x.End());
	}

	~List()
	{
		Clear();

		//delete _node;
		Destroy(_node);
		_node = NULL;
	}

/* * func: ��ֵ��������� (��ͳд��) */
//	List<T>& operator=(const List<T> &x)
//	{
//		if (this != &x)
//		{
//			Iterator first1 = Begin(), last1 = End();
//			ConstIterator first2 = x.Begin(), last2 = x.End();
//
//			while (first1 != last1 && first2 != last2)
//			{
//				*first1++ = *first2++;
//			}
//
//			if (first1 == last1)    // * ǰ����������ߵ�ͷ����Ҫ��x��ʣ�µĽڵ㿽������ǰ��������
//				Insert(last1, first2, last2);
//			else
//				Erase(first1, last1);
//		}
//
//		return *this;
//	}

/* * func: ��ֵ��������� (���д��) ���ֱ�ۣ����ǲ��ܱ����Ը�ֵ */
	List<T>& operator=(List<T> x)
	{
		Swap(x);
		return *this;
	}

public:
	// * List�Ķ���ӿ�

	Iterator Begin() { return Iterator((*_node)._next); }
	ConstIterator Begin() const { return Iterator((*_node)._next); }

	Iterator End() { return Iterator(_node); }
	ConstIterator End() const { return Iterator(_node); }

	ReverseIterator RBegin() { return ReverseIterator(End()); }
	ConstReverseIterator RBegin()const { return ConstReverseIterator(End()); }

	ReverseIterator REnd() { return ReverseIterator(Begin()); }
	ConstReverseIterator REnd() const { return ConstReverseIterator(Begin()); }

	bool Empty() { return _node->_next == _node; }

	Reference Front() { return *Begin(); }
	ConstReference Front() const { return *Begin(); }

	Reference Back() { return *End(); }
	ConstReference Back() const { return *End(); }

	void Swap(List<T> &lt) { std::swap(_node, lt._node); }
	 

	// * List ����������ʵ��

/* * func: ��positionλ��ǰ����ֵΪx�Ľڵ� */
	Iterator Insert(Iterator &position, const T& x)
	{
		LinkType tmp = CreateNode(x);
		
		// * �ýڵ���ǰ��ڵ�Ӻ�
		tmp->_prev = position._node->_prev;
		tmp->_next = position._node;

		position._node->_prev = tmp;
		tmp->_prev->_next = tmp;

		// * �����µĽڵ�ĵ�����
		return Iterator(tmp);
	}

/* * func: ��positionǰ�����n��ֵΪx��Ԫ�� */
	void Insert(Iterator &position, SizeType n, const T& x)
	{
		for (; n > 0; --n)
			Insert(position, x);
	}

	// * �������а�nǿת��SizeType(size_t)���͵�ԭ�������ǿת����ݹ�����Լ�����Ϊ�����˳����Ӷ�����ջ���
	void Insert(Iterator &position, int n, const T& x) { Insert(position, (SizeType)n, x); }
	void Insert(Iterator &position, long n, const T& x) { Insert(position, (SizeType)n, x); }
	

/* * func: Ϊ�����������һ������ȥ��ʼ���������ƣ� ��positionǰ���� [first, last) ֮���Ԫ�� */
	void Insert(Iterator position, ConstIterator first, ConstIterator last)
	{
		for (; first != last; ++first)
			Insert(position, *first);
	}

/* * func: Ϊ�������ʼ���������� */
	void Insert(Iterator position, const T* first, const T* last)
	{
		for (; first != last; ++first)
			Insert(position, *first);
	}

/* * func: Ϊ���������͵�һ������ȥ��ʼ���������� */
	template<class InputIterator>
	void Insert(Iterator position, InputIterator first, InputIterator last)
	{
		for (; first != last; ++first)
			Insert(position, *first);
	}
	

/* * func: ɾ��ָ��Ϊλ��position����Ԫ�� */
	Iterator Erase(Iterator &position)
	{
		LinkType NextNode = position._node->_next;
		LinkType PrevNode = position._node->_prev;

		// * �Ӻ�ǰ��ڵ�
		NextNode->_prev = PrevNode;
		PrevNode->_next = NextNode;

		// * ɾ���ýڵ�
		//delete position._node;
		ListNodeAllocator::Deallocate(position._node);

		// * ���ظýڵ��һ���ڵ�ĵ�����
		return Iterator(NextNode);
	}

/* * func: ɾ��ָ������ [first, last) ֮���Ԫ�� */
	Iterator Erase(Iterator &first, Iterator &last)
	{
		while (first != last)
			Erase(first++);

		return last;
	}

/* * func: ���¸�������size��newSize�����newSize �� size�󣬺��油x*/
	void Resize(SizeType newSize, const ValueType& x)
	{
		SizeType len = 0;
		Iterator it = Begin();
		for (; it != End() && len < newSize; ++it, ++len)
			;
		if (len == newSize)
			Erase(it, End());
		else
			Insert(End(), newSize - len, x);
	}

/* * func: ���¸�������size��newSize */
	void Resize(SizeType newSize) { Resize(newSize, T()); }

/* * func: ��list������[first, last)��������滻�� */
	template<class _Iter>
	void Assign(_Iter first, _Iter last)
	{
		Iterator it = Begin();

		// * Ϊ�˸��ýڵ㣬���Ȱ�һЩֵ����list��ʼ��һ��Ԫ��
		for (; it != End() && first != last; ++it, ++first)
			*it = *first;

		Erase(it, End());

		for (; first != last; ++first)
			Insert(it++, *first);
	}


/* * func: �����������Ԫ�أ����ǲ��ݻ����� */
	void Clear()
	{
		LinkType cur = _node->_next;
		LinkType del = NULL;
		while (cur != _node)    // * ����ɾ����ͷ�ڵ���������нڵ�
		{
			del = cur;
			cur = cur->_next;
			//delete del;
			ListNodeAllocator::Deallocate(del);
		}
		_node->_next = _node;
		_node->_prev = _node;
	}

/* * func: ɾ��ָ��Ԫ��ֵ�Ľڵ� */
	void Remove(const T& value)
	{
		Iterator cur = Begin();
		Iterator end = End();
		Iterator del;    // * ��ʱ��������¼Ҫɾ���Ľڵ�

		while (cur != end)
		{
			if (*cur == value)    // * ע�������ʧЧ������
			{
				del = cur;
				++cur;
				Erase(del);
			}
			else
				++cur;
		}
	}

	void Unique()
	{
		Iterator first = Begin();
		Iterator end = End();
		if (first == end) return;

		Iterator next = first;
		while (++next != end)
		{
			if (*first != *next)
				first = next;
			else
			{
				Erase(next);
				next = first;
			}
		}
	}

/* * func: ������x�ϲ�����ǰ������ (���ᴴ���½ڵ�)����ǰ������x�ϲ�֮ǰ���������� */
	void Merge(List<T> &x)
	{
		Iterator first1 = Begin(), last1 = End();
		Iterator first2 = x.Begin(), last2 = x.End();
		Iterator next;
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1)    // * first2�����ݽ�С����Ҫ�ϲ�
			{
				next = first2;
				Transfer(first1, first2, ++next);
				first2 = next;    // * �ϲ�֮����Ҫ��first2ָ��x�е���һ���ڵ�(��ʱ����ʹ��first2++����Ϊfirst2��ָ�ڵ��Ѿ��ӵ���ǰ��������)
			}
			else
				++first1;
		}  /* while */

		// * ���������������֣�first1��û�ߵ�ͷ��first2��ͷ�� �� first1��first2ͬʱ�ߵ�ͷ�� first1�ߵ�ͷ�˵�first2��û�ߵ�ͷ
		// * ���ڵ�һ��������޹ؽ�Ҫ
		// * ���ڵڶ��������Ҫ��x��ʣ��Ľڵ�ӵ���ǰ����������
		if (first2 != last2)
			Transfer(last1, first2, last2);
	}

	void Reverse()
	{
		Iterator first = Begin();        // * ��һ���ڵ�
		if (first._node == _node || first._node->_next == _node)  // * û�нڵ����ֻ��һ���ڵ㣬ֱ�ӷ���
			return;

		Iterator prev;        // * ��ʱ��������¼��ǰת�ӵĵ�һ���ڵ�
		Iterator end = End(); // * ѭ���Ľ�������

		++first;
		while (first != end)    // * first �ӵڶ������ݽڵ㿪ʼ
		{
			prev = first;
			Transfer(Begin(), prev, ++first);
		}
	}

/* * func: ������x�Ӻϵ���ǰ��positionλ��ǰ */
	void Splice(Iterator &position, List<T> &x)
	{
		if (!x.Empty())
			Transfer(position, x.Begin(), x.End());
	}

/* * func: ��i��ָ�ڵ�ӵ�positionǰ���λ�� */
	void Splice(Iterator &position, Iterator i)
	{
		Iterator j = i;
		++j;

		// * �Լ��ӵ��Լ�ǰ�棬����ǰһ���ڵ�ӵ���ǰ�ڵ��ǰ�棬ֱ�ӷ���
		if (position == i || position == j)  
			return;

		Transfer(position, i, j);
	}

/* * func: �� [first, last) ��ָ����ӵ�positionǰ�� */
	void Splice(Iterator &position, Iterator first, Iterator last)
	{
		if (first != last)
			Transfer(position, first, last);
	}

	void PushFront(const T& x) { Insert(Begin(), x); }

	void PushBack(const T& x) { Insert(End(), x); }

	void PopFront() { Erase(Begin()); }

	void PopBack() { Erase(--End()); }

protected:
	// * List�ļ����ڲ��ӿ�

/* * func: ����һ���ڵ���ڴ棬�����ýڵ�Ĺ��캯������󷵻ظýڵ��ָ�� */
	LinkType CreateNode(const T& value)
	{
		LinkType tmp = GetNode();
		Construct(tmp, value);

		return tmp;
	}

/* * func: ����һ������ڵ�Ŀռ�(ֻ����ռ䲢������) */
	LinkType GetNode()
	{
		return ListNodeAllocator::Allocate();
	}

/* * func: ��ʼ���յ����� (��ʵ���ǹ���һ��ͷ�ڵ�) */
	void EmptyInitialize()
	{
		_node = CreateNode(T());
		_node->_next = _node;
		_node->_prev = _node;
	}

/* * func: ���캯���� */
	void FillInitialize(SizeType n, const T& value)
	{
		EmptyInitialize();
		Insert(Begin(), n, value);
	}

/* * func: ��[first, last)�ڵ�����Ԫ���ƶ���position֮ǰ */
	// *ps: ���䲻���ص�����position����λ��[first, last)�����ڲ�
	void Transfer(Iterator position, Iterator first, Iterator last)
	{
		if (position != last)
		{
			// * �ܹ�������ָ��(�ܹ�6��)��Ҫ�޸�
			// * �������ҵķ���
			last._node->_prev->_next = position._node;
			position._node->_prev->_next = first._node;
			first._node->_prev->_next = last._node;

			// * ��������ķ���
			position._node->_prev = last._node->_prev;
			last._node->_prev = first._node->_prev;
			first._node->_prev = position._node->_prev;
		}
	}

	void RangeInitialize(const T* first, const T* last)
	{
		EmptyInitialize();
		Insert(Begin(), first, last);
	}

	template<class InputIterator>
	void RangeInitialize(InputIterator first, InputIterator last)
	{
		EmptyInitialize();
		Insert(Begin(), first, last);
	}
};


// * ����ȫ�ֺ���
template<class T>
inline bool operator==(const List<T> &x, const List<T> &y)
{
	typedef typename List<T>::LinkType LinkType;
	LinkType end1 = x._node;
	LinkType end2 = y._node;
	LinkType f1 = end1->_next;
	LinkType f2 = end2->_next;

	for (; f1 != end1 && f2 != end2; f1 = f1->_next, f2 = f2->_next)
	{
		if (f1->_data != f2->_data)
			return false;
	}

	// ****** ѭ�������󻹲���ֱ�ӷ����棬��ʱҪ�����������ǲ���һ����
	return f1 == end1 && f2 == end2;
}

template<class T>
inline bool operator<(const List<T> &x, const List<T> &y)
{
	return true;
}

template<class T>
inline void Swap(const List<T> &x, const List<T> &y)
{
	x.Swap(y);
}


