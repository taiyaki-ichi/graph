#include<iostream>
#include"adjacency_list.hpp"
#include"iterator/search_iterator.hpp"
#include"iterator/for_each.hpp"
#include"iterator/BFS.hpp"
#include"iterator/DFS.hpp"

#include"dijkstra_shortest_paths.hpp"

struct my_vertex_property{};
struct my_edge_property { int weight; };

int main()
{
	
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
	std::cout << "\n";
	
	std::unordered_map<unsigned int, unsigned int> parent{};
	std::unordered_map<unsigned int, std::optional<int>> distance{};
	
	unsigned int from = 0;
	unsigned int to = 7;
	
	graph::dijkstra_shortest_paths(myGraph, from, &my_edge_property::weight, &parent, &distance);

	std::cout << "parent\n";
	for (size_t i = 0; i < parent.size(); i++)
		std::cout << i << ": " << parent[i] << "\n";
	std::cout << "\n";

	std::cout << "distance\n";
	for (size_t i = 0; i < distance.size(); i++)
		if (distance[i])
			std::cout << i << ": " << distance[i].value() << "\n";
	std::cout << "\n";


	graph::adjacency_list<graph::directed> myGraph2{};

	myGraph2.add_vertex(0);
	myGraph2.add_vertex(1);
	myGraph2.add_vertex(2);
	myGraph2.add_vertex(3);
	myGraph2.add_vertex(4);
	myGraph2.add_vertex(5);
	myGraph2.add_vertex(6);
	myGraph2.add_edge({ 0,1 });
	myGraph2.add_edge({ 0,2 });
	myGraph2.add_edge({ 1,3 });
	myGraph2.add_edge({ 3,4 });
	myGraph2.add_edge({ 2,5 });
	myGraph2.add_edge({ 2,6 });

	for (auto iter : graph::BFS{ myGraph2,0 })
		std::cout << iter << "\n";
	std::cout << "\n";

	for (auto iter : graph::DFS_postorder{ myGraph2,0 })
		std::cout << iter << "\n";
	std::cout << "\n";

	for (auto iter : graph::DFS_preorder{ myGraph2,0 })
		std::cout << iter << "\n";
	std::cout << "\n";

	for (auto iter : graph::for_each{ myGraph2 })
		std::cout << iter << "\n";
	std::cout << "\n";

	return 0;
}