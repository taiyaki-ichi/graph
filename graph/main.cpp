#include<iostream>
#include"graph.hpp"
#include"search_iterator.hpp"
#include"for_each_policy.hpp"

struct my_vertex_property{};
struct my_edge_property {};


int main()
{
	
	graph::graph<
		graph::set_vertex_property<my_vertex_property>,
		graph::set_edge_property<my_edge_property>
	> myGraph{};
	
	myGraph.add_vertex(0);
	myGraph.add_vertex(1);
	myGraph.add_vertex(2);
	myGraph.add_vertex(3);
	myGraph.add_vertex(4);

	using for_each_iter = graph::search_iterator<graph::for_each_policy>;

	for (auto iter = for_each_iter::begin(myGraph); iter != for_each_iter::end(); ++iter)
		std::cout << iter << " ";

	std::cout << "\n";

	for (auto iter = for_each_iter::begin(myGraph); iter != for_each_iter::end(); ++iter)
		std::cout << iter << " ";

	return 0;
}