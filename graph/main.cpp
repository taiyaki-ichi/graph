#include<iostream>
#include"adjacency_list.hpp"
#include"search_iterator.hpp"
#include"for_each_policy.hpp"
#include"BFS.hpp"
#include"DFS.hpp"

#include"dijkstra_shortest_paths.hpp"

struct my_vertex_property{};
struct my_edge_property { int weight; };

int main()
{

	/*
	std::vector<unsigned int> list{};
	std::cout << "1 :";
	graph::dijkstra_shortest_paths_impl();
	std::cout << "\n";
	std::cout << "2 :";
	graph::dijkstra_shortest_paths_impl(graph::predecessor_map{ list });
	std::cout << "\n";
	std::cout << "3 :";
	for (auto n : list)
		std::cout << n << " ";
		*/


	/*
	std::vector<unsigned int> list{};

	graph::aaa(graph::predecessor_map{ list }, int{}, int{});

	for (auto n : list)
		std::cout << n << " ";
		*/
	
	using my_graph = graph::adjacency_list<
		graph::set_vertex_property<my_vertex_property>,
		graph::set_edge_property<my_edge_property>,
		graph::directed
	>;

	my_graph myGraph{};
	
	
	myGraph.add_vertex(0);
	myGraph.add_vertex(1);
	myGraph.add_vertex(2);
	myGraph.add_vertex(3);
	myGraph.add_vertex(4);
	myGraph.add_vertex(5);
	myGraph.add_vertex(6);
	myGraph.add_vertex(7);

	myGraph.add_edge({ 0,1 }, {5});
	myGraph.add_edge({ 0,2 }, {9});
	myGraph.add_edge({ 0,3 }, {2});
	myGraph.add_edge({ 0,4 }, {15});
	myGraph.add_edge({ 1,2 }, {7});
	myGraph.add_edge({ 2,5 }, {8});
	myGraph.add_edge({ 3,4 }, {1});
	myGraph.add_edge({ 3,6 }, {11});
	myGraph.add_edge({ 4,2 }, {7});
	myGraph.add_edge({ 4,5 }, {1});
	myGraph.add_edge({ 4,7 }, {8});
	myGraph.add_edge({ 5,7 }, {3});
	myGraph.add_edge({ 6,4 }, {9});
	myGraph.add_edge({ 6,7 }, {2});

	myGraph.print();
	
	
	std::unordered_map<unsigned int, unsigned int> parent{};
	std::unordered_map<unsigned int, std::optional<int>> distance{};
	
	unsigned int from = 0;
	unsigned int to = 7;
	
	graph::dijkstra_shortest_paths_impl(myGraph, from, parent, distance, &my_edge_property::weight);

	std::cout << "parent\n";
	for (int i = 0; i < parent.size(); i++)
		std::cout << i << ": " << parent[i] << "\n";
	std::cout << "\n";

	std::cout << "distance\n";
	for (int i = 0; i < distance.size(); i++)
		if (distance[i])
			std::cout << i << ": " << distance[i].value() << "\n";
	std::cout << "\n";
	

	return 0;
}