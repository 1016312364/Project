/***********************************************************************************
	File name:  TypeTraits.h
	Author:     common
	Created:    2016-7-2  18:03:40
	
	Purpose:    ������ȡ
	Addition:   �ڵڶ����ռ�����������Ҫ�õ����������Ⱥ�������Щ������ʵ�ֻ�����������������Ԫ�����ͣ�
				POD���ͣ�������������ͣ����������޹�ʹ���Ĺ��졢�������졢��ֵ��������������������������;���POD����
***********************************************************************************/
#pragma once

//#define __STL_CLASS_PARTIAL_SPECIALIZATION

struct __TrueType
{};

struct __FalseType
{};

template<class T>            // * T��ֵ����
struct __TypeTraits
{
	typedef __TrueType this_dummy_member_must_be_first;
	                                                        // * Trivial ����Ҫ�ģ��޹�ʹ����
	typedef __FalseType HasTrivialDefaultConstructor;       // * �Ƿ����޹�ʹ����Ĭ�Ϲ��캯��
	typedef __FalseType HasTrivialCopyConstructor;          // * �Ƿ����޹�ʹ����Ĭ�Ͽ������캯��
	typedef __FalseType HasTrivialAssignmentOperator;       // * �Ƿ����޹�ʹ���ĸ�ֵ�����
	typedef __FalseType HasTrivialDestructor;               // * �Ƿ����޹�ʹ������������

	typedef __FalseType IsPODType;    // * ��������Ĭ�ϲ���POD���ͣ�ȫ�����Զ������ͣ�
};



// * �ػ�
// * ������Щ�ػ����ͣ������������ͣ�Ĭ�϶��� POD ���͡�
template<>
struct __TypeTraits<char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<signed char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned char>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType    IsPODType;
};

template<>
struct __TypeTraits<short>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned short>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<int>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned int>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned long>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<float>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<double>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<long double>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};


// * �����Ƕ�ָ�����͵��ػ�
//#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class T>
struct __TypeTraits<T*>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

//#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template<>
struct __TypeTraits<char*>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<signed char*>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};

template<>
struct __TypeTraits<unsigned char*>
{
	typedef __TrueType HasTrivialDefaultConstructor;
	typedef __TrueType HasTrivialCopyConstructor;
	typedef __TrueType HasTrivialAssignmentOperator;
	typedef __TrueType HasTrivialDestructor;
	typedef __TrueType IsPODType;
};