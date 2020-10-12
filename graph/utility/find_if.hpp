#pragma once
#include<type_traits>

namespace graph
{
	namespace utility
	{
		struct not_found {};

		template<template<typename>typename Predicate, typename Head, typename... Tail>
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
		template<template<typename>typename Predicate, typename Tail>
		struct find_if_impl<Predicate, Tail>
		{
			using type = typename std::conditional<
				Predicate<Tail>::value,
				Tail,
				not_found
			>::type;
		};


		//Listの中からPredicateをTrueにするような型の取得
		template<template<typename>typename Predicate, typename... List>
		struct find_if
		{
			//not_foundはListの大きさが0だった時用
			//別の方法で処理したい
			using type = typename find_if_impl<Predicate, not_found,List...>::type;
		};


		//Tを含んているかどうか
		template<typename T,typename... List>
		struct contain 
		{
			template<typename U>
			using check = typename std::is_same<T, U>::type;

			using type = typename std::conditional<
				std::is_same_v<typename find_if<check, not_found, List...>::type, not_found>,
				std::false_type,
				std::true_type
			>::type;
			
		};
	}
}