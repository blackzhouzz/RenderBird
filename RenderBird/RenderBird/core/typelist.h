#pragma once

struct NullType {};

template <typename T, typename... S>
struct TypeList
{
	typedef T Head;
	typedef TypeList<S...> Tail;
};

template <typename T>
struct TypeList<T>
{
	typedef T Head;
	typedef NullType Tail;
};

template <typename T, typename TL>
struct type_id
{
	enum { value = 1 + type_id<T, typename TL::Tail>::value };
};

template <typename T, typename... S>
struct type_id<T, TypeList<T, S...>>
{
	enum { value = 0 };
};

template <int I, typename TL>
struct type_at
{
	typedef typename type_at<I - 1, typename TL::Tail>::type type;
};

template <typename Head, typename... S>
struct type_at<0, TypeList<Head, S...>>
{
	typedef Head type;
};

template <typename TL>
struct length
{
	enum { value =  1 + length<TL::Tail>::value };
};

template <>
struct length<NullType>
{
	enum { value = 0 };
};