#pragma once
#include <type_traits>

namespace meta
{
	template<class... Types>
	struct typelist;

	/// Represents situations where we did not find a specific type
	struct null_type {};

	/// Template is same allows us to determine whether a specific type is an
	/// instantiation of a specific template.
	///
	///     using is_vector = template_is_same<std::vector, std::vector<int>>;
	///     std::cout << is_vector::value << std::endl;
	///
	/// Prints "1", on the other hand:
	///
	///     using is_vector = template_is_same<std::vector, std::map<int,int>>;
	///     std::cout << is_vector::value << std::endl;
	///
	/// Prints "0"
	///
	template<template <class...> class T, class U>
	struct template_is_same : std::false_type
	{ };

	template<template <class...> class T, class... Arg>
	struct template_is_same<T, T<Arg...>> : std::true_type
	{ };

	/// @brief Conditionally appends types to a typelist.
	///
	/// The conditional_append meta function iterates though a variadic
	/// list of template arguments. For each type T in the list it uses the
	/// Predicate<T>::value to conditonally append the type ot the
	/// TypeList. The TypeList should be of type typelist or an otherwise
	/// compatible type.
	template
		<
		template <class> class Predicate,
		class TypeList,
		class...
		>
		struct conditional_append;

	/// Specialization for an empty list
	template
		<
		template <class> class Predicate,
		class TypeList
		>
		struct conditional_append<Predicate, TypeList>
	{
		using type = TypeList;
	};

	/// Specialization for an list with at least one element
	template
		<
		template <class> class Predicate,
		class TypeList,
		class T,
		class... List
		>
		struct conditional_append<Predicate, TypeList, T, List...>
	{
		using type = typename std::conditional<
			Predicate<T>::value,
			typename conditional_append<Predicate,
			typename TypeList::template append<T>, List...>::type,
			typename conditional_append<Predicate,
			TypeList, List...>::type>::type;
	};

	/// @brief Removes types for the variadic template list if the match
	///        the predicate.
	///
	/// To implement this feature we rely on the conditional_append meta
	/// function which will append types to a typelist if they match the
	/// Predicate. Removing elements can therefore be implemented by
	/// iterating the variadic types and only appending those that do not
	/// match the predicate. So a conditional_append with negated prefix
	/// will return a list of types without the types matching the
	/// Predicate.
	template
		<
		template <class> class Predicate,
		class... Args
		>
	struct remove
	{
		// Small helper negates the Predicate
		template<class U>
		struct negate_prediate : public std::conditional<
			Predicate<U>::value, std::false_type, std::true_type>::type
		{ };

		// Call conditional append
		using type = typename
			conditional_append<negate_prediate, typelist<>, Args...>::type;
	};

	/// @brief Concatenates two typelists
	template<class ListOne, class ListTwo>
	struct concatenate;

	// Specialization where we "extract" the types contained in the two lists
	template<class... TypesOne, class... TypesTwo>
	struct concatenate<typelist<TypesOne...>, typelist<TypesTwo...>>
	{
		using type = typelist<TypesOne..., TypesTwo...>;
	};

	/// @brief Will take variadic template list and for each element invoke
	///        the call operator of the Visitor object passed to the visit
	///        function.
	template<class... List>
	struct visitor
	{
		template<class Visitor>
		static void visit(Visitor&&) {}
	};

	// Specialization where there is at least one element in the list
	template<class T, class... List>
	struct visitor<T, List...>
	{
		template<class Visitor>
		static void visit(Visitor&& v)
		{
			// Invoke the call operator on the v object passing in a
			// default constructed object of type T.
			v(T{});

			// Recursive pass the remaining types to visitor, to visit the
			// next element in the list
			visitor<List...>::template visit(v);
		}
	};


	template <int, typename...>
	struct type_at;

	template <int I, typename T, typename ... List>
	struct type_at<I, T, List...>
	{
		using type = typename type_at<I - 1, List...>::type;
	};

	template <typename T, typename ... List>
	struct type_at<0, T, List...>
	{
		using type = T;
	};


	/// @brief Finds a type based on the specific feature tag
	///
	/// The find meta function accepts a variable list of template
	/// arguments, it then instantiates the Predicate template with each
	/// type and checks the Predicate<T>::value. If the value evaluates
	/// true find<Predicate, Default, List>::type will equal T. If
	/// Predicate<T>::value is false for all types in List then
	/// find<Predicate, Default, List>::type will be set to Default.
	template
		<
		template <class> class Predicate,
		class NotFound,
		class... List
		>
		struct find;

	/// Specialization for when the list is empty, we just set type = NotFound
	template
		<
		template <class> class Predicate,
		class NotFound
		>
	struct find<Predicate, NotFound>
	{
		using type = NotFound;
	};

	/// Specialization when there is at lets one element in the list
	template
		<
		template <class> class Predicate,
		class NotFound,
		class T,
		class... List
		>
	struct find<Predicate, NotFound, T, List...>
	{
		// If Predicate<T>::value is true then type = T
		// Else we call find for the next type in the list
		using type = typename std::conditional<
			Predicate<T>::value,
			T, typename find<Predicate, NotFound, List...>::type>::type;
	};

	/// @brief Typelist list which basically is a "container" for a
	///        variable number of template arguments.
	///
	/// Using the "meta" methods define here we can manipulate the content
	/// of the type list.
	template<class... Types>
	struct typelist
	{
		/// Appends a type to the list.
		///
		///     using result = typelist<>::template
		///         append<std::vector<int>>::template
		///         append<double>;
		///
		/// The results is typelist<std::vector<int>,double>
		///
		template<class... AppendTypes>
		using append = typelist<Types..., AppendTypes...>;

		/// The meta method extend appends the contents of input typelist
		/// the current typelist.
		///
		///     using result = typelist<int, double>::
		///         extend<meta::typelist<float, bool>>;
		///
		/// The result is typelist<int,double,float,bool>
		///
		template<class List>
		using extend = typename concatenate<typelist<Types...>, List>::type;

		/// Finds a type in the list.
		///
		///     using types =
		///         typelist<std::vector<int>, int, double>;
		///
		///     using result = types::template
		///         find<std::is_integral>;
		///
		/// The results is int
		///
		template
			<
			template <class> class Predicate,
			class NotFound = null_type
			>
			using find = typename find<Predicate, NotFound, Types...>::type;

		/// Visit each type in the list.
		///
		///    using types = typelist<my_type_one, my_type_two>;
		///
		///    struct print
		///    {
		///        template<class T>
		///        void operator()(T)
		///        {
		///            std::cout << T::to_string() << std::endl;
		///        }
		///    };
		///
		///    types::template visit(print());
		///
		/// To use visit the types contained in the list must be default
		/// constructed. Furthermore in the specific example show above we
		/// use a static member of the types called to_string() this would
		/// need to be adapted to the specific types in the type list.
		///
		template<class Visitor>
		static void visit(Visitor&& v)
		{
			visitor<Types...>::template visit(v);
		}

		/// Removes an item from the list if present
		///
		///    using types =
		///         typelist<std::vector<int>, int, double>;
		///
		///    using result = types::template
		///        remove<std::is_integral>;
		///
		/// The results is typelist<std::vector<int>, double>
		///
		template<template <class> class Predicate>
		using remove = typename remove<Predicate, Types...>::type;

		static const int length = sizeof...(Types);

		template <int I>
		using type_at = typename type_at<I, Types...>::type;

		template<typename, typename>
		struct index_of {};

		// index_of base case: found the type we're looking for.
		template <typename T, typename... List>
		struct index_of<T, typelist<T, List...>>
			: std::integral_constant<std::size_t, 0>
		{
		};

		// index_of recursive case: 1 + index_of the rest of the types.
		template <typename T, typename TOther, typename... List>
		struct index_of<T, typelist<TOther, List...>>
			: std::integral_constant<std::size_t,
			1 + index_of<T, typelist<List...>>::value>
		{
		};

		template<typename T>
		using type_index = typename index_of<T, typelist<Types...>>;
	};
}
