#pragma once
#include"../adjacency_list.hpp"
#include"search_iterator.hpp"

namespace graph
{
	//‡•s“¯‚Å‚·‚×‚Ä’T‚é
	template<typename... Args>
	struct for_each_iterator_body
	{
		const adjacency_list<Args...>* m_adjacency_list;
		int m_index;

	public:
		template<typename... Args>
		int init(const adjacency_list<Args...>* g) {
			m_adjacency_list = g;
			m_index = 0;
			return m_adjacency_list->get_all_vertex_index()[m_index];
		}

		int increment(int) {
			m_index++;
			if (m_index >= static_cast<int>(m_adjacency_list->get_all_vertex_index().size()))
				return -1;
			else
				return m_adjacency_list->get_all_vertex_index()[m_index];
		}
	};

	template<typename... Args>
	struct for_each
	{
		using iter = search_iterator<for_each_iterator_body<Args...>>;
		const adjacency_list<Args...>* m_adjacency_list;

	public:
		for_each(const adjacency_list<Args...>& g)
			:m_adjacency_list{&g}
		{}

		iter begin(){
			return iter::begin(m_adjacency_list);
		}

		const iter& end() {
			return iter::end();
		}

		for_each& operator=(const for_each&) = delete;
		for_each& operator=(for_each&&) = delete;
		for_each(const for_each&) = delete;
		for_each(for_each&&) = delete;
	};

}
