#pragma once
#include"graph.hpp"
#include<queue>

namespace graph
{
	struct breadth_first_search_preorder
	{

		static std::unordered_map<unsigned int, std::set<unsigned int>> m_adjacency_list;
		static std::queue<unsigned int> m_queue;
		static std::unordered_map<unsigned int, bool> m_is_searched;

		template<typename... Args>
		static int init(const graph<Args...>& g, unsigned int from)
		{
			m_is_searched.clear();
			auto vertex = g.get_all_vertex_index();
			for (auto num : vertex)
				m_is_searched.insert({ num, false });
			m_is_searched.insert_or_assign( from,true );

			while (!m_queue.empty())
				m_queue.pop();
			m_queue.push(from);

			m_adjacency_list = g.get_adjacency_vertex_list();

			return from;
		}

		static int increment(int num)
		{
			int next{};
			while(1)
			{
				next = action(num);
				if (next != num)
					return next;
			}
		}

	private:
		static int action(int num)
		{
			auto vertexList = m_adjacency_list.at(m_queue.front());
			for (auto v : vertexList)
			{
				//–¢’Tõ‚Ì’¸“_‚ª‚ ‚éê‡
				if (!(m_is_searched.find(v)->second))
				{
					m_is_searched.insert_or_assign(v, true);
					m_queue.push(v);

					return v;
				}
			}

			m_queue.pop();

			/*
			auto tmp = m_queue;
			while (!tmp.empty()) {
				std::cout << tmp.front() << " ";
				
			}
			*/

			if (m_queue.empty())
				return -1;
			else
				return num;
		}


	};
}