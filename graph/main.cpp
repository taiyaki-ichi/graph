#include<iostream>
#include"adjacency_list.hpp"
#include"search_iterator.hpp"
#include"for_each_policy.hpp"
#include"BFS.hpp"
#include"DFS.hpp"


struct my_vertex_property{};
struct my_edge_property {};

int main()
{
	std::vector<std::pair<unsigned int, unsigned int>> list =
	{
		{0,1},
		{0,2},
		{1,3},
		{3,4},
		{2,5},
		{2,6}
	};

	graph::adjacency_list<
		graph::set_vertex_property<my_vertex_property>,
		graph::set_edge_property<my_edge_property>,
		graph::directed
	> myGraph{ std::move(list) };
	
	
	/*
	myGraph.add_vertex(0);
	myGraph.add_vertex(1);
	myGraph.add_vertex(2);
	myGraph.add_vertex(3);
	myGraph.add_vertex(4);
	myGraph.add_vertex(5);
	myGraph.add_vertex(6);

	myGraph.add_edge({ 0,1 });
	myGraph.add_edge({ 0,2 });
	myGraph.add_edge({ 1,3 });
	myGraph.add_edge({ 3,4 });
	myGraph.add_edge({ 2,5 });
	myGraph.add_edge({ 2,6 });
	*/
	
	myGraph.print();

	for (auto iter : graph::DFS_preorder{myGraph,0})
		std::cout << iter << " ";
	std::cout << "\n";
	for (auto iter : graph::DFS_postorder{ myGraph,0 })
		std::cout << iter << " ";
	std::cout << "\n";
	for (auto iter : graph::BFS{myGraph,0})
		std::cout << iter << " ";
	std::cout << "\n";

	return 0;
}