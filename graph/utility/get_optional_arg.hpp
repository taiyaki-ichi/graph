#pragma once
#include<type_traits>

namespace graph
{
	namespace utility
	{

		struct not_found {};

		template<template<typename>typename Predicate,typename Head,typename... Tail>
		struct find_if_impl
		{
			//HeadがPredicaateを満たすなら type=Head
			//満たさないなら再帰してTailをチェック
			using type = typename std::conditional<
				Predicate<Head>::value,
				Head,
				typename find_if_impl<Predicate, Tail...>::type
			>::type;
		};

		//再帰の終わり
		template<template<typename>typename Predicate,typename Tail>
		struct find_if_impl<Predicate,Tail>
		{
			using type = typename std::conditional<
				Predicate<Tail>::value,
				Tail,
				not_found
			>::type;
		};

		//インターフェース
		template<template<typename>typename Predicate,typename... List>
		struct find_if
		{
			using type = typename find_if_impl<Predicate, List...>::type;
		};

		/*
		template<typename NullTag,template<typename>typename Predicate,typename... List>
		struct get_optional_arg
		{
		private:
			using result=
		};
		*/


	}
}