/***********************************************************************************
	File name:  Vector.h
	Author:     common
	Created:    2016-8-7  20:50:49
	
	Purpose:    ģ��STL�� vector ��ʵ��
	Addition:   
***********************************************************************************/
#pragma once

#include "Allocate.h"
#include "Iterator.h"
#include "Uninitialized.h"
#include "Construct.h"

template<class T, class alloc = Alloc>
class Vector
{
public:
	// * ֵ����
	typedef T ValueType;

	// * ָ������
	typedef ValueType* Pointer;
	typedef const ValueType* ConstPointer;

	// * ��������
	typedef ValueType& Reference;
	typedef const ValueType& ConstReference;

	// * ����������
	typedef ValueType* Iterator;
	typedef const ValueType* ConstIterator;

	// * �������������
	typedef ReverseIterator<ConstIterator> ConstReverseIterator;
	typedef ReverseIterator<Iterator> ReverseIterator;

	typedef size_t SizeType;
	typedef ptrdiff_t DifferenceType;

	// * ������ר���������� ÿ�η�������ͷ�һ��ValueType���͵Ŀռ䣬ʹ��alloc����
	typedef SimpleAlloc<ValueType, alloc> DataAllocator;

protected:
	/* ****** �����Ĺ��� 
	startָ��ʼ��
	finishָ�����һ�����ݵ���һ��λ��
	endOfStorageָ�����һ���ռ����һ��λ��
	*/

	Iterator start;
	Iterator finish;
	Iterator endOfStorage;

public:
	// * ������������(���죬�������������죬��ֵ�����)

	// * ���캯��
	Vector() : start(0), finish(0), endOfStorage(0) {}

	// * ����һ����������n��ֵΪvalue��Ԫ��
	Vector(SizeType n, const ValueType& value) {	Fill_Initialize(n, value); }

	// * ��ֻ��һ�����ݳ�Ա���࣬�������Ĺ��캯�����׷�����ʽת����ǰ���explicit���Է�ֹ������������Ƕ���Vecotr���������ݳ�Ա���࣬ǰ���explicit�ɼӿɲ���
	explicit Vector(SizeType n) {	Fill_Initialize(n, T()); }

	// * ��һ�����乹��һ������
	Vector(ConstIterator first, ConstIterator last)
	{
		SizeType n = 0;
		Distance(first, last, n);
		start = AllocateAndCopy(n, first, last);
		finish = start + n;
		endOfStorage = finish;
	}

	// * ��������
	Vector(const Vector<T, alloc>& x)
	{
		start = AllocateAndCopy(x.Size(), x.Begin(), x.End());
		finish = start + x.Size();
		endOfStorage = finish;
	}

	// * ��������
	~Vector()
	{
		// * ��������Ԫ��
		Destroy(start, finish);
		// * �ͷ����пռ�
		Deallocate();
	}

	/*  ��ֵ������Ĵ�ͳд�� */
	//Vector<T, alloc> & operator=(const Vector<T, alloc>& x)
	//{
	//	if (&x != this)
	//	{
	//		SizeType sizeOfX = x.Size();
	//		if (sizeOfX > Capacity())    // * x��size���ҵ�ǰ������������Ҫ����ռ�
	//		{
	//			Iterator tmp = AllocateAndCopy(x.End() - x.Begin(), x.Begin(), x.End());
	//			
	//			// * �ͷ�ԭ�пռ�
	//			Destroy(start, finish);
	//			Deallocate();
	//			// * �޸ķ�Χ
	//			start = tmp;
	//			endOfStorage = start + sizeOfX;
	//		}
	//		else if (Size() > x.Size())    // * x��size�Ƚ�С��ֻ��x.Size()��Ԫ�أ���ǰ������֮���Ԫ��������
	//		{
	//			Iterator end = copy(x.Begin(), x.End(), start);
	//			Destroy(end, finish);
	//		}
	//		else                           // * x��size�Ƚϴ�ǰ��ο����������ȳ�ʼ���ڿ���(�����POD���ͣ��ú������ǵ��õ�copy)
	//		{
	//			copy(x.Begin(), x.End(), start);
	//			UninitializedCopy(x.Begin() + Size(), x.End(), finish);
	//		}
	//		finish = start + sizeOfX;
	//	}
	//	return *this;
	//}

	/*  ��ֵ������ļ��д�� */
	Vector<T, alloc> & operator=(Vector<T, alloc> x)
	{
		Swap(x);
		return *this;
	}
public:
	// * �����Ķ���ӿ�

	Iterator Begin() { return start; };
	ConstIterator Begin()const { return start; };

	Iterator End() { return finish; };
	ConstIterator End()const { return finish; };

	/*
	RBegin()
	RBegin()
	REnd()
	REnd()
	*/

/* * func: ���ص�������Ԫ�ظ��� */
	SizeType Size()const { return End() - Begin(); }

	SizeType MaxSize() { return SizeType(SizeType(-1) / sizeof(T)); }

/* * func: �޸�����������Ԫ�ظ�����newSize���������Ԫ�ظ���newSize����ɾ������һ���֣� ��֮��β�˲���һ���� x */
	void Resize(SizeType newSize, ValueType& x)
	{
		if (newSize < Size())
			Erase(Begin() + newSize, finish);
		else
			Insert(End(), newSize - Size(), x);
	}

/* * func: ���ص�����������������������Ŀռ��С */
	SizeType Capacity()const { return endOfStorage - start; };

	bool Empty()const { return End() == Begin(); };

	Reference operator[](SizeType n) { return *(Begin() + n); };
	ConstReference operator[](SizeType n)const { return *(Begin() + n); }

	Reference Front() { return *Begin(); }
	ConstReference Front() const { return *Begin(); }

	Reference Back() { return *(End() - 1); }
	ConstReference Back() const { return *(End() - 1); }


/* * func: �������������� ���� ���� ��n��ֻ�е������ĵ�ǰ����С��nʱ���ú����Ż������� */
	void Reserve(SizeType n)
	{
		if (Capacity() < n)
		{
			// * �������ݵ�һ���µĵ�ַ���ÿռ䳤��Ϊ n
			SizeType oldSize = Size();
			Iterator tmp = AllocateAndCopy(n, start, finish);

			// * ����ԭ�пռ�
			Destroy(start, finish);
			Deallocate();

			// * �����޸ķ�Χ
			start = tmp;
			finish = start + oldSize;
			endOfStorage = start + n;
		}
	}

/* * func: ������β���������� x */
	void PushBack(const T& x)
	{
		if (finish != endOfStorage)
			Construct(finish++, x);
		else
			Insert_Aux(End(), x);
	}

/* * func: β��ɾ��һ��Ԫ�� */
	void PopBack()
	{
		--finish;
		Destroy(finish);
	}

/* * func: ɾ�������е�����Ԫ�أ����ǲ��������� */
	void Clear() { Erase(Begin(), End()); }

/* * func: ������������ */
	void Swap(Vector<T, Alloc> &x)
	{
		std::swap(start, x.start);
		std::swap(finish, x.finish);
		std::swap(endOfStorage, x.endOfStorage);
	}

/* * func: ��position������Ԫ�� x */
	Iterator Insert(Iterator position, const ValueType& x)
	{
		// * �п��пռ䣬���Ҳ������������棬ֱ�ӹ���һ������
		if (finish != endOfStorage && position == End())
		{
			Construct(position, x);
			++finish;
		}
		// * ���û�п��пռ� | position�������м�ĳһ��λ�ã���Ҫ����ռ� | �����ƶ�Ԫ��
		else
			Insert_Aux(position, x);
		
		return position;
	}

	/* * func: ��position������Ĭ��Ԫ�� */
	Iterator Insert(Iterator position) { return Insert(position, T()); }

/* * func: ��positionλ�ò���һ������ [first, last) */
	void Insert(Iterator position, ConstIterator first, ConstIterator last)
	{
		// * ֻ�е�[first, last)����ʱ��ȥ��һЩ��
		if (first != last)
		{
			SizeType n = 0;
			Distance(first, last, n);

			if (endOfStorage - finish > n)  // ʣ��ռ乻��
			{
				SizeType elemsAfter = finish - position;
				Iterator oldFinish = finish;
				if (elemsAfter > n)
				{
					// * ����finish֮ǰ��n�����ݵ�finish
					UninitializedCopy(finish - n, finish, finish);
					finish += n;
					// * ʣ���elemsAfter-n��Ԫ�����󿽱�
					copy_backward(position, oldFinish - n, oldFinish);
					// * ��������ݵ�ָ��λ��
					copy(first, last, position);
				}
				else    // elemsAfter <= n
				{
					// * �Ȱ�[first, last)����ĺ��漸��Ԫ�ؿ�����ԭ����β��
					UninitializedCopy(first + elemsAfter, last, finish);
					finish += n - elemsAfter;
					// * ��elemsAfter��ʣ��Ԫ�ؿ�����β��
					UninitializedCopy(position, oldFinish, finish);
					finish += elemsAfter;
					// * ��elemsAfter����Ԫ�ؿ�����position
					copy(first, first + elemsAfter, position);
				}
			}  /* if endOfStorage - finish > n */
			else  // * endOfStorage - finish <= n ˵���ռ䲻����
			{
				// * ����ռ�
				SizeType oldSize = Size();
				SizeType len = oldSize + max(oldSize, n);
				Iterator newStart = DataAllocator::Allocate(len);
				Iterator newFinish = newStart;
				
				// * ����position֮ǰ��Ԫ�ص��¿ռ䣬 ����[start, position)��newStart
				newFinish = UninitializedCopy(start, position, newStart);
				// * ׷�������ݵ��¿ռ��β��������[first, last)��newFinish
				newFinish = UninitializedCopy(first, last, newFinish);
				// * ׷��position֮���Ԫ�ص��¿ռ��β��������[position, finish)��newFinish
				newFinish = UninitializedCopy(position, finish, newFinish);

				// * �ͷ�ԭ�ռ�
				Destroy(start, finish);
				Deallocate();

				// * ���ķ�Χ
				start = newStart;
				finish = newFinish;
				endOfStorage = start + len;
			}  /* else */
		}  /* if first != last */
	}

/* * func: ��positionλ�ò���n��ֵΪx��Ԫ�� */
	void Insert(Iterator position, SizeType n, ValueType& x)
	{
		if (n != 0)
		{
			if (endOfStorage - finish >= n)  // ʣ��ռ乻��
			{
				ValueType xCopy = x;
				SizeType elemsAfter = finish - position;
				Iterator oldFinish = finish;
				if (elemsAfter > n)
				{
					// * ����finish֮ǰ��n�����ݵ�finish
					UninitializedCopy(finish - n, finish, finish);
					finish += n;
					// * ʣ���elemsAfter-n��Ԫ�����󿽱�
					copy_backward(position, oldFinish - n, oldFinish);
					// * ��������ݵ�ָ��λ��
					fill(position, position + n, xCopy);
				}
				else    // elemsAfter <= n
				{
					// * �Ȱ�[first, last)����ĺ��漸��Ԫ�ؿ�����ԭ����β��
					UninitializedFill_n(finish, n - elemsAfter, xCopy);
					finish += n - elemsAfter;
					// * ��elemsAfter��ʣ��Ԫ�ؿ�����β��
					UninitializedCopy(position, oldFinish, finish);
					finish += elemsAfter;
					// * ��elemsAfter����Ԫ�ؿ�����position
					fill(position, oldFinish, xCopy);
				}
			}  /* if endOfStorage - finish > n */
			else  // * endOfStorage - finish <= n ˵���ռ䲻����
			{
				// * ����ռ�
				SizeType oldSize = Size();
				SizeType len = oldSize + max(oldSize, n);
				Iterator newStart = DataAllocator::Allocate(len);
				Iterator newFinish = newStart;

				// * ����position֮ǰ��Ԫ�ص��¿ռ䣬 ����[start, position)��newStart
				newFinish = UninitializedCopy(start, position, newStart);
				// * ׷�������ݵ��¿ռ��β��������[first, last)��newFinish
				newFinish = UninitializedFill_n(newFinish, n, x);
				// * ׷��position֮���Ԫ�ص��¿ռ��β��������[position, finish)��newFinish
				newFinish = UninitializedCopy(position, finish, newFinish);

				// * �ͷ�ԭ�ռ�
				Destroy(start, finish);
				Deallocate();

				// * ���ķ�Χ
				start = newStart;
				finish = newFinish;
				endOfStorage = start + len;
			}  /* else */
		}  /* if n != 0 */
	}

/* * func: ɾ��positionλ�õ�Ԫ�أ�����positionλ�ã����ڸ�λ�õ�ֵ���ɾ��ǰposition����һ��Ԫ�� */
	Iterator Erase(Iterator position)
	{
		if (position + 1 != finish)
			copy(position + 1, finish, position);
		--finish;
		Destroy(finish);
		return position;
	}

/* * func: ɾ�������е�[first, last)�����Ԫ�أ�����ԭ����firstλ�� */
	Iterator Erase(Iterator first, Iterator last)
	{
		// * ������[last, finish)�����Ԫ�ظ���[first, last)�� newFinish����last����һ��λ��
		Iterator newFinish = copy(last, finish, first);
		// * ɾ��������ظ�Ԫ��
		Destroy(newFinish, finish);
		// * �޸�������
		finish = newFinish;
		return first;
	}

/* * func: ��[first, last)�����Ԫ�ظ�ֵ�����������[first, last)�����Ԫ�ػ�û�е�ǰ��Size()�󣬾�ɾ���������������Ԫ�� */
	void Assign(ConstIterator first, ConstIterator last)
	{
		Iterator it = Begin();

		// * Ϊ�˸��ýڵ㣬���Ȱ�һЩֵ����������ʼ��һ��Ԫ��
		for (; it != End() && first != last; ++it, ++first)
			*it = *first;

		Erase(it, End());

		for (; first != last; ++first)
			Insert(it++, *first);
	}

protected:
	// һЩ�ڲ��ӿ�

/* * func: ��position������Ԫ�� x */
	template<class T>
	void Insert_Aux(Iterator position, const T& x)
	{
		if (finish != endOfStorage)
		{
			Construct(finish, *(finish - 1));
			++finish;
			T x_copy = x;

			// * ��[position, finish-2)��Ԫ�ؿ������� finish-1) Ϊĩ��ַ�Ŀռ䡾�Ӻ���ǰ������
			copy_backward(position, finish - 2, finish - 1);
			//���¶��壺 
			//copy_backward(first, last, result)  {hile (first != last) *--result = *--last;}

			*position = x_copy;
		}
		else    // * finish == endOfStorage�����пռ䲻��
		{
			const SizeType old_size = Size();
			const SizeType len = old_size != 0 ? 2 * old_size : 1;
			Iterator new_start = DataAllocator::Allocate(len);
			Iterator new_finish = new_start;

			// * �ȿ���������ǰһ����
			//new_finish = UninitializedCopy(start, position, new_start);
			new_finish = uninitialized_copy(start, position, new_start);

			// * ��position���뵱ǰ��ֵ
			Construct(new_finish, x);
			++new_finish;

			// * ���������ĺ�һ����
			//new_finish = UninitializedCopy(position, finish, new_finish);
			new_finish = uninitialized_copy(position, finish, new_finish);

			// * ����֮ǰ�Ŀռ�
			Destroy(Begin(), End());
			//DataAllocator::Deallocate();
			Deallocate();
			
			// * ������ǰ�ķ�Χ
			start = new_start;
			finish = new_finish;
			endOfStorage = new_start + len;
		}  /* else */
	}  /* Insert_Aux */

/* * func: ������ʼ���ã�����n���ռ䣬����ʼ����value */
	void Fill_Initialize(SizeType n, const ValueType& value)
	{
		start = AllocateAndFill(n, value);
		finish = start + n;
		endOfStorage = finish;
	}

/* * func: ����n���ռ䣬���ڸշ���Ŀռ������n��ֵΪvalue��Ԫ�� */
	Iterator AllocateAndFill(SizeType n, const ValueType& value)
	{
		Iterator result = DataAllocator::Allocate(n);

		UninitializedFill_n(result, n, value);

		return result;
	}

/* * func: ����n���ռ䣬����[first, last)�����Ԫ�ؿ������շ���Ŀռ��� */
	template<class ForwardIterator>
	Iterator AllocateAndCopy(SizeType n, ForwardIterator first, ForwardIterator last)
	{
		Iterator result = DataAllocator::Allocate(n);

		// [first, last) ��������result��ʼ�ĵط�
		UninitializedCopy(first, last, result);

		return result;
	}

/* * func: ������������������ͷ����пռ� */
	void Deallocate()
	{
		if (start)
			DataAllocator::Deallocate(start, endOfStorage - start);
	}
};  /* Vector */