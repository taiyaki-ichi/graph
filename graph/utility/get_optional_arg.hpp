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
			//Head��Predicaate�𖞂����Ȃ� type=Head
			//�������Ȃ��Ȃ�ċA����Tail���`�F�b�N
			using type = typename std::conditional<
				Predicate<Head>::value,
				Head,
				typename find_if_impl<Predicate, Tail...>::type
			>::type;
		};

		//�ċA�̏I���
		template<template<typename>typename Predicate,typename Tail>
		struct find_if_impl<Predicate,Tail>
		{
			using type = typename std::conditional<
				Predicate<Tail>::value,
				Tail,
				not_found
			>::type;
		};

		//�C���^�[�t�F�[�X
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