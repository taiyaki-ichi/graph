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
			//Head��Predicaate�𖞂����Ȃ� type=Head
			//�������Ȃ��Ȃ�ċA����Tail���`�F�b�N
			using type = typename std::conditional<
				Predicate<Head>::value,
				Head,
				typename find_if_impl<Predicate, Tail...>::type
			>::type;
		};

		//�ċA�̏I���
		template<template<typename>typename Predicate, typename Tail>
		struct find_if_impl<Predicate, Tail>
		{
			using type = typename std::conditional<
				Predicate<Tail>::value,
				Tail,
				not_found
			>::type;
		};

		//�C���^�[�t�F�[�X
		template<template<typename>typename Predicate, typename... List>
		struct find_if
		{
			//not_found��List�̑傫����0���������p
			//�ʂ̕��@�ŏ���������
			using type = typename find_if_impl<Predicate, not_found,List...>::type;
		};



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