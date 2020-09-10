#include<iostream>
#include"utility/get_optional_arg.hpp"

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