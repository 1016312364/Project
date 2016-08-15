/***********************************************************************************
	File name:  Iterator.h
	Author:     common
	Created:    2016-7-3  15:13:51
	
	Purpose:    ������
	Addition:   
***********************************************************************************/
#pragma once


struct InputIteratorTag {};
struct OutputIteratorTag {};
struct ForwardIteratorTag : public InputIteratorTag {};
struct BidirectionalIteratorTag : public ForwardIteratorTag {};
struct RandomAccessIteratorTag : public BidirectionalIteratorTag {};

template<class T, class Distance>
struct InputIterator
{
	typedef InputIteratorTag IteratorCategory;
	typedef T                ValueType;
	typedef Distance         DifferenceType;
	typedef T*               Pointer;
	typedef T&               Reference;
};

//template<class T, class Distance>
struct OutputIterator
{
	typedef OutputIteratorTag IteratorCategory;
	typedef void              ValueType;
	typedef void              DifferenceType;
	typedef void              Pointer;
	typedef void              Reference;
};

template<class T, class Distance>
struct ForwardIterator
{
	typedef ForwardIteratorTag IteratorCategory;
	typedef T                  ValueType;
	typedef Distance           DifferenceType;
	typedef T*                 Pointer;
	typedef T&                 Reference;
};

template<class T, class Distance>
struct BidirectionalIterator
{
	typedef BidirectionalIteratorTag IteratorCategory;
	typedef T                        ValueType;
	typedef Distance                 DifferenceType;
	typedef T*                       Pointer;
	typedef T&                       Reference;
};

template<class T, class Distance>
struct RandomAccessIterator
{
	typedef RandomAccessIteratorTag IteratorCategory;
	typedef T                       ValueType;
	typedef Distance                DifferenceType;
	typedef T*                      Pointer;
	typedef T&                      Reference;
};

template <class Category, class T, class Distance = ptrdiff_t,
class Pointer = T*, class Reference = T&>
struct Iterator {
	typedef Category  IteratorCategory;
	typedef T         ValueType;
	typedef Distance  DifferenceType;
	typedef Pointer   Pointer;
	typedef Reference Reference;
};

template <class IteratorType>
struct IteratorTraits {
	typedef typename IteratorType::IteratorCategory IteratorCategory;
	typedef typename IteratorType::ValueType ValueType;
	typedef typename IteratorType::DifferenceType DifferenceType;
	typedef typename IteratorType::Pointer Pointer;
	typedef typename IteratorType::Reference Reference;
};





/////////////////////////////////////////////////////////////////////////////////
// ���������

template<class Iterator>
class ReverseIterator
{
protected:
	Iterator current;

public:
	typedef typename IteratorTraits<Iterator>::IteratorCategory IteratorCategory;
	typedef typename IteratorTraits<Iterator>::ValueType ValueType;
	typedef typename IteratorTraits<Iterator>::DifferenceType DifferenceType;
	typedef typename IteratorTraits<Iterator>::Reference Reference;
	typedef typename IteratorTraits<Iterator>::Pointer Pointer;

	typedef Iterator IteratorType;
	typedef ReverseIterator<Iterator> Self;

public:
	// * ���캯��
	ReverseIterator() {}
	explicit ReverseIterator(IteratorType x)
		:current(x)
	{}

	// * ��������
	ReverseIterator(const Self& x)
		:current(x.current)
	{}

	// * ���������ת����ͨ�ĵ�����
	IteratorType Base()
	{
		return current;
	}

	// * ����������Ľ����ã�Ҫ��ǰһ��Ԫ�ؽ�����
	Reference operator*()
	{
		IteratorType tmp = current;
		return *--tmp;                  // ����ʱ��������������ı䵱ǰ��current
	}

	// * ���ص��ǽڵ���ֵ�ĵ�ַ
	Pointer operator->()
	{
		return (&(operator*()));
	}

	// * ǰ��++��������Ϊ��ֵ����������
	Self& operator++()
	{
		--current;
		return *this;
	}

	// * ����++��������Ϊ��ֵ������ֵ
	Self operator++(int)
	{
		Self tmp = *this;
		--current;
		return tmp;
	}

	// * ǰ��--��������Ϊ��ֵ����������
	Self& operator--()
	{
		++current;
		return *this;
	}

	// * ����--��������Ϊ��ֵ������ֵ
	Self operator--(int)
	{
		Self tmp = *this;
		++current;
		return tmp;
	}

	// * ����Ľӿ���Ϊ������������

	Self operator+(DifferenceType n)
	{
		return Self(current - n);
	}

	Self& operator+=(DifferenceType n)
	{
		current -= n;
		return *this;
	}

	Self operator-(DifferenceType n)
	{
		return Self(current + n);
	}

	Self& operator-=(DifferenceType n)
	{
		current += n;
		return *this;
	}

	Reference operator[](DifferenceType n) const
	{
		return (*(*this + n));
	}
};







///////////////////////////////////////////////////////////////////////////////////////////
//IteratorCategory()

// * ����ſ����д� ��inline RandomAccessIteratorTag IteratorCategory(const T*)��ͻ
//template <class Iterator>
//inline typename IteratorTraits<Iterator>::IteratorCategory
//IteratorCategory(const Iterator&)
//{
//	typedef typename IteratorTraits<Iterator>::IteratorCategory Category;
//	return Category();
//}

template <class T>
inline RandomAccessIteratorTag 
IteratorCategory(const T*)                // * for Vector
{
	return RandomAccessIteratorTag();
}


template <class T, class Distance>
inline InputIteratorTag
IteratorCategory(const InputIterator<T, Distance>&)
{
	return InputIteratorTag();
}

template <class T, class Distance>
inline ForwardIteratorTag
IteratorCategory(const ForwardIterator<T, Distance>&)
{
	return ForwardIteratorTag();
}

template <class T, class Distance>
inline BidirectionalIteratorTag
IteratorCategory(const BidirectionalIterator<T, Distance>&)
{
	return BidirectionalIteratorTag();
}

template <class T, class Distance>
inline RandomAccessIteratorTag
IteratorCategory(const RandomAccessIterator<T, Distance>&)
{
	return RandomAccessIteratorTag();
}



/////////////////////////////////////////////////////////////////////////////////////////////
// ValueType()

template <class T, class Distance>
inline T* ValueType(const InputIterator<T, Distance>&)
{
	return (T*)(0);
}

template <class T, class Distance>
inline T* ValueType(const ForwardIterator<T, Distance>&)
{
	return (T*)(0);
}

template <class T, class Distance>
inline T* ValueType(const BidirectionalIterator<T, Distance>&)
{
	return (T*)(0);
}

template <class T, class Distance>
inline T* ValueType(const RandomAccessIterator<T, Distance>&)
{
	return (T*)(0);
}


template <class T>
inline T* ValueType(const T*)     // ****** for Vector 
{
	return (T*)(0);
}


/////////////////////////////////////////////////////////////////////////////////////////
//DistanceType()

template<class T, class Distance>
inline Distance *
DistanceType(const BidirectionalIterator<T, Distance> &)
{
	return (Distance *)(0);
}

template<class T, class Distance>
inline Distance *
DistanceType(const RandomAccessIterator<T, Distance> &)
{
	return (Distance *)(0);
}


/////////////////////////////////////////////////////////////////////////////////////////
// Distance()

template <class InputIterator, class Dist>
inline void __Distance(InputIterator first, InputIterator last, Dist& n, InputIteratorTag)
{
	while (first != last) { ++first; ++n; }
}

template <class RandomAccessIterator, class Dist>
inline void __Distance(RandomAccessIterator first, RandomAccessIterator last, Dist& n, RandomAccessIteratorTag)
{
	n += last - first;
}


template <class InputIterator, class Dist>
inline void Distance(InputIterator first, InputIterator last, Dist& n)
{
	// * ģ��VS�� �������Vector���У���������ԭ��ָ�룬��������û��IteratorCategory������VS��Vector�ĵ�����Ҳʵ�ֳ�һ���࣬������

	//typename IteratorTraits<InputIterator>::IteratorCategory IteratorTag;
	//__Distance(first, last, n, IteratorTag);

	__Distance(first, last, n, IteratorCategory(first));
}


///////////////////////////////////////////////////////////////////////////////////////////
// Advance()

template<class BidirectionalIterator, class Dist>
void __Advance(BidirectionalIterator& i, Dist n, BidirectionalIteratorTag)
{
	if (n > 0)
		while (n--) ++i;
	else
		while (n++) --i;
}

template<class RandomAccessIterator, class Dist>
void __Advance(RandomAccessIterator& i, Dist n, RandomAccessIteratorTag)
{
	i += n;
}

template<class InputIterator, class Dist>
void Advance(InputIterator &i, Dist n)
{
	// * ģ��VS�� �������Vector���У���������ԭ��ָ�룬��������û��IteratorCategory������VS��Vector�ĵ�����Ҳʵ�ֳ�һ���࣬������

	//typename IteratorTraits<InputIterator>::IteratorCategory IteratorTag;
	//__Advance(i, n, IteratorTag);

	__Advance(i, n, IteratorCategory(i));
}

///////////////////////////////////////////////////////////////////////////////////////////

//template <class Iterator>
//inline bool operator==(ReverseIterator<Iterator>& x, ReverseIterator<Iterator>& y)
//{
//	return x.Base() == y.Base();
//}

//template <class Iterator>
//inline bool operator!=(ReverseIterator<Iterator>& x, ReverseIterator<Iterator>& y)
//{
//	return x.Base() != y.Base();
//}


