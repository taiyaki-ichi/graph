#pragma once
#include"graph.hpp"

namespace graph
{
	//���s���ł��ׂĒT��
	struct for_each_policy
	{
		static std::vector<unsigned int> m_all_graph_vertex_index;
		static int m_index;

		template<typename... Args>
		static int init(const graph<Args...>& g) {
			m_all_graph_vertex_index = g.get_all_vertex_index();
			m_index = 0;
			return m_all_graph_vertex_index[m_index];
		}

		static int increment(int) {
			m_index++;
			if (m_index >= static_cast<int>(m_all_graph_vertex_index.size()))
				return -1;
			else
				return m_all_graph_vertex_index[m_index];
		}
	};

	std::vector<unsigned int> for_each_policy::m_all_graph_vertex_index{};
	int for_each_policy::m_index = 0;

	
}