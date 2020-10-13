#pragma once
#include"adjacency_list.hpp"


//二分木っぽいの
inline graph::adjacency_list<graph::directed> get_samplegraph1()
{
	graph::adjacency_list<graph::directed> result{};

	result.add_vertex(0);
	result.add_vertex(1);
	result.add_vertex(2);
	result.add_vertex(3);
	result.add_vertex(4);
	result.add_vertex(5);
	result.add_vertex(6);
	result.add_edge({ 0,1 });
	result.add_edge({ 0,2 });
	result.add_edge({ 1,3 });
	result.add_edge({ 3,4 });
	result.add_edge({ 2,5 });
	result.add_edge({ 2,6 });

	return result;
}


struct my_vertex_property {};
struct my_edge_property { int weight; };

using path_graph = graph::adjacency_list<
	graph::set_vertex_property<my_vertex_property>,
	graph::set_edge_property<my_edge_property>,
	graph::directed
>;

//ダイクストラ法のために作ったやつ
inline path_graph get_samplegraph2()
{
	path_graph result{};

	result.add_vertex(0);
	result.add_vertex(1);
	result.add_vertex(2);
	result.add_vertex(3);
	result.add_vertex(4);
	result.add_vertex(5);
	result.add_vertex(6);
	result.add_vertex(7);

	result.add_edge({ 0,1 }, { 5 });
	result.add_edge({ 0,2 }, { 9 });
	result.add_edge({ 0,3 }, { 2 });
	result.add_edge({ 0,4 }, { 15 });
	result.add_edge({ 1,2 }, { 7 });
	result.add_edge({ 2,5 }, { 8 });
	result.add_edge({ 3,4 }, { 1 });
	result.add_edge({ 3,6 }, { 11 });
	result.add_edge({ 4,2 }, { 7 });
	result.add_edge({ 4,5 }, { 1 });
	result.add_edge({ 4,7 }, { 8 });
	result.add_edge({ 5,7 }, { 3 });
	result.add_edge({ 6,4 }, { 9 });
	result.add_edge({ 6,7 }, { 2 });

	return result;
}