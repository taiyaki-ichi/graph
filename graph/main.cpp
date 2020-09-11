#include<iostream>
#include"graph.hpp"

struct my_vertex_property{};
struct my_edge_property {};


int main()
{
	
	graph::graph<
		graph::set_vertex_property<my_vertex_property>,
		graph::set_edge_property<my_edge_property>
	> myGraph{};

	std::cout << typeid(decltype(myGraph)::vertex_property).name() << "\n";
	std::cout << typeid(decltype(myGraph)::edge_property).name() << "\n";

	
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