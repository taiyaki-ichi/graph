#include<iostream>
#include"adjacency_list.hpp"
#include"iterator/search_iterator.hpp"
#include"iterator/for_each.hpp"
#include"iterator/BFS.hpp"
#include"iterator/DFS.hpp"

#include"algorithm/dijkstra_shortest_paths.hpp"
#include"algorithm/connected_components.hpp"

#include"sample_graph.hpp"

int main()
{
	
	auto myGraph = get_samplegraph2();

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


	auto myGraph2 = get_samplegraph1();

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


	std::vector<std::pair<unsigned int, unsigned int>> edge{
		{0,1},
		{1,2},
		{2,0},
		{2,3},
		{3,4},
		{4,3},
		{4,5},
		{4,6}
	};

	graph::adjacency_list<graph::directed> g3{ std::move(edge) };

	auto groups = graph::connected_components<graph::strong_connection_tag>(g3);

	for (const auto& g : groups)
	{
		for (const auto& v : g)
		{
			std::cout << v << " ";
		}
		std::cout << "\n";
	}


	return 0;
}