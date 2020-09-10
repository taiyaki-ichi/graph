#include<iostream>
#include"utility/get_optional_arg.hpp"

struct A {};
struct B {};
struct C {};

/*
template<typename T>
struct is_A :std::conditional<
	std::is_same_v<A,T>,
	std::true_type,std::false_type>
{};
*/

template<typename T>
struct is_A {
	static const bool value = false;
};
template<>
struct is_A<A> {
	static const bool value = true;
};


int main()
{
	using namespace graph;

	//if constexpr()

	is_A<A> a;
	is_A<B> b;

	/*
	if constexpr (is_A<A>::type::value)
		std::cout << "a";

		*/
	
	if constexpr (std::is_same_v<utility::find_if<is_A, C, B,A>::type, A>)
		std::cout << "find A!!\n";
	else
		std::cout << "not find A\n";

	if constexpr (std::is_same_v<utility::find_if<is_A, C, B>::type, A>)
		std::cout << "find A!!\n";
	else
		std::cout << "not find A\n";
		




	return 0;
}