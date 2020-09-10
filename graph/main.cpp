#include<iostream>
#include"utility/get_optional_arg.hpp"

struct A {};
struct B {};
struct C {};


template<typename T>
struct is_A {
	static const bool value = false;
};
template<>
struct is_A<A> {
	static const bool value = true;
};

template<typename T>
struct is_A_ : std::conditional<
	std::is_same_v<A, T>,
	std::true_type, std::false_type>::type
{};

template<typename T>
struct boolean :T::type {};

int main()
{
	using namespace graph;

	
	if constexpr (utility::get_optional_arg<std::false_type, boolean, std::false_type,std::false_type,std::false_type>::type::value)
		std::cout << "find A!!\n";
	else
		std::cout << "not find A\n";

	if constexpr (utility::get_optional_arg<std::false_type, boolean, std::false_type, std::false_type, std::true_type>::type::value)
		std::cout << "find A!!\n";
	else
		std::cout << "not find A\n";
		




	return 0;
}