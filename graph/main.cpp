#include<iostream>
#include"graph.hpp"
#include"search_iterator.hpp"
#include"for_each_policy.hpp"
#include"BFS.hpp"

struct my_vertex_property{};
struct my_edge_property {};

int main()
{

	
	

	
	graph::adjacency_list<
		graph::set_vertex_property<my_vertex_property>,
		graph::set_edge_property<my_edge_property>,
		graph::directed
	> myGraph{};
	
	
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
	

	for (auto iter : graph::BFS{ myGraph,0 })
		std::cout << iter << " ";



	/*
	using for_each_iter = graph::search_iterator<graph::for_each_policy>;

	for (auto iter = for_each_iter::begin(myGraph); iter != for_each_iter::end(); ++iter)
		std::cout << iter << " ";

	std::cout << "\n";

	using BFS_iter = graph::search_iterator<graph::breadth_first_preorder>;
	for (auto iter = BFS_iter::begin(myGraph, 0); iter != BFS_iter::end(); ++iter)
		std::cout << iter << " ";
		*/
	
	return 0;
}