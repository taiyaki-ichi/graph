#pragma once
#include"graph.hpp"
#include<queue>
#include"search_iterator.hpp"

namespace graph
{

	template<typename... Args>
	struct BFS_iterator_type
	{
	private:
		static std::unordered_map<unsigned int, std::set<unsigned int>> m_adjacency_list;
		static std::queue<unsigned int> m_queue;
		static std::unordered_map<unsigned int, bool> m_is_searched;

	public:
		static int init(const adjacency_list<Args...>& g, unsigned int from)
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
				//未探索の頂点がある場合
				if (!(m_is_searched.find(v)->second))
				{
					m_is_searched.insert_or_assign(v, true);
					m_queue.push(v);

					
					return v;
				}
			}

			m_queue.pop();

			if (m_queue.empty())
				return -1;
			else
				return num;
		}
	};

	template<typename... Args>
	std::unordered_map<unsigned int, std::set<unsigned int>> BFS_iterator_type<Args...>::m_adjacency_list;
	template<typename... Args>
	std::queue<unsigned int> BFS_iterator_type<Args...>::m_queue;
	template<typename... Args>
	std::unordered_map<unsigned int, bool> BFS_iterator_type<Args...>::m_is_searched;

	//コピー不可、範囲for文の一時オブジェクトとして使用
	template<typename... Args>
	struct BFS
	{
	private:
		using BFS_iter = search_iterator<BFS_iterator_type<Args...>>;

		const adjacency_list<Args...>* const m_graph;
		const unsigned int m_from;

	public:
		BFS(const adjacency_list<Args...>& g, unsigned int from)
			:m_graph{&g}
			, m_from{from}
		{}

		BFS_iter begin() {
			return BFS_iter::begin(*m_graph, m_from);
		}

		BFS_iter end() {
			return BFS_iter::end();
		}

		BFS& operator=(const BFS&) = delete;
		BFS& operator=(BFS&&) = delete;
		BFS(const BFS&) = delete;
		

	};


}