#include<iostream>
#include"graph.hpp"




int main()
{
	
	graph::graph<> myGraph{};

	myGraph.add_vertex(0);
	myGraph.add_vertex(1);
	myGraph.add_vertex(2);
	myGraph.add_vertex(3);

	std::cout << myGraph << "\n";

	myGraph.add_edge({ 0,1 });
	myGraph.add_edge({ 1,3 });
	myGraph.add_edge({ 2,3 });

	std::cout << myGraph << "\n";

	myGraph.remove_edge({ 2,3 });
	std::cout << myGraph << "\n";

	myGraph.remove_vertex(1);
	std::cout << myGraph << "\n";

	return 0;
}