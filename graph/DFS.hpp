#pragma once
#include"graph.hpp"
#include"search_iterator.hpp"
#include<stack>

namespace graph
{

	template<bool IsPreorder,typename... Args>
	struct DFS_iterator_type
	{
	private:
		//グラフのポインタ
		static const adjacency_list<Args...>* m_adjacency_list_ptr;
		//スタック
		static std::stack<unsigned int> m_stack;
		//探索したかどうか
		static std::unordered_map<unsigned int, bool> m_is_searched;

	public:
		static int init(const adjacency_list<Args...>* const g, unsigned int from)
		{
			//fromが有効な値でないとき
			if (g->get_adjacency_vertex_list().find(from) == g->get_adjacency_vertex_list().end())
				return search_iterator<DFS_iterator_type<IsPreorder,Args...>>::end();

			//以下、それぞれ初期化

			m_is_searched.clear();
			auto vertex = g->get_all_vertex_index();
			for (auto num : vertex)
				m_is_searched.insert({ num, false });
			m_is_searched.insert_or_assign(from, true);

			while (!m_stack.empty())
				m_stack.pop();
			m_stack.push(from);

			m_adjacency_list_ptr = g;

			int num = from;
			//後行順の場合
			if constexpr (!IsPreorder)
			{
				while (1)
				{
					num = action(num);
					if (num != from)
						break;
				}
			}

			return num;
		}

		static int increment(int num)
		{
			int next = num;
			while (1)
			{
				next = action(next);
				if (next != num)
					return next;
			}
		}

	private:
		static int action(int num)
		{
			if (m_stack.empty())
				return search_iterator<DFS_iterator_type<IsPreorder,Args...>>::end();

			auto vertexList = m_adjacency_list_ptr->get_adjacency_vertex(m_stack.top());
			bool pushFlag = false;
			for (auto vertex : vertexList)
			{
				//未探査の頂点がある場合
				if (!m_is_searched.at(vertex))
				{
					m_is_searched.insert_or_assign(vertex, true);
					m_stack.push(vertex);

					pushFlag = true;
					break;
				}
			}

			//先行順の場合
			if constexpr (IsPreorder)
			{
				if (pushFlag)
				{
					return m_stack.top();
				}
				else
				{
					m_stack.pop();
					return num;
				}
			}
			//後攻順の場合
			else
			{
				if (pushFlag)
				{
					return num;
				}
				else
				{
					auto nextNum = m_stack.top();
					m_stack.pop();
					return nextNum;
				}
			}
			
		}
	};

	template<bool IsPreorder, typename... Args>
	const adjacency_list<Args...>* DFS_iterator_type<IsPreorder, Args...>::m_adjacency_list_ptr = nullptr;
	template<bool IsPreorder, typename... Args>
	std::stack<unsigned int> DFS_iterator_type<IsPreorder, Args...>::m_stack{};
	template<bool IsPreorder, typename... Args>
	std::unordered_map<unsigned int, bool> DFS_iterator_type<IsPreorder, Args...>::m_is_searched{};


	//コピー不可、範囲for文の一時オブジェクトとして使用
	template<bool IsPreorder, typename... Args>
	struct DFS
	{
	private:
		using DFS_iter = search_iterator<DFS_iterator_type<IsPreorder, Args...>>;

		const adjacency_list<Args...>* const m_graph;
		const unsigned int m_from;

	public:
		DFS(const adjacency_list<Args...>& g, unsigned int from)
			:m_graph{ &g }
			, m_from{ from }
		{}

		DFS_iter begin() {
			return DFS_iter::begin(m_graph, m_from);
		}

		DFS_iter end() {
			return DFS_iter::end();
		}

		DFS& operator=(const DFS&) = delete;
		DFS& operator=(DFS&&) = delete;
		DFS(const DFS&) = delete;
	};

	//インターフェース
	template<typename... Args>
	struct DFS_preorder : DFS<true, Args...> {
		DFS_preorder(const adjacency_list<Args...>& g, unsigned int from)
			:DFS<true, Args...>{ g,from }
		{}
	};

	template<typename... Args>
	struct DFS_postorder : DFS<false, Args...> {
		DFS_postorder(const adjacency_list<Args...>& g, unsigned int from)
			: DFS<false, Args...>{ g,from }
		{}
	};


}