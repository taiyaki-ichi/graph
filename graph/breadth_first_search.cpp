#include"breadth_first_search.hpp"

namespace graph
{
	std::unordered_map<unsigned int, std::set<unsigned int>> breadth_first_search_preorder::m_adjacency_list;
	std::queue<unsigned int> breadth_first_search_preorder::m_queue;
	std::unordered_map<unsigned int, bool> breadth_first_search_preorder::m_is_searched;
}