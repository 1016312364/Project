/***********************************************************************************
	File name:  Construct.h
	Author:     common
	Created:    2016-7-2  15:45:50
	
	Purpose:    ����������
	Addition:   
***********************************************************************************/
#pragma once
#include "TypeTraits.h"


/* * func: ��ָ��λ�ù������ */
template<class T1, class T2>
inline void Construct(T1 *p, const T2 &value)
{
	new (p) T1(value);
}

/* * func: �������������������ͷſռ� */
template<class T>
inline void Destroy(T *pointer)
{
	pointer->~T();
}



template<class ForwardIterator>
inline void __Destroy_Aux(ForwardIterator, ForwardIterator, __TrueType)
{}

template<class ForwardIterator>
inline void __Destroy_Aux(ForwardIterator first, ForwardIterator last, __FalseType)
{
	for (; first != last; ++first)
		Destroy(&*first);
}

template<class ForwardIterator, class T>
inline void __Destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __TypeTraits<T>::HasTrivialDestructor TrivialDestructor;
	__Destroy_Aux(first, last, TrivialDestructor());
}

/* * func: ����[first, last)�����Ԫ�أ����ǲ����ͷſռ� */
template<class ForwardIterator>
inline void Destroy(ForwardIterator first, ForwardIterator last)
{
	__Destroy(first, last, ValueType(first));
}