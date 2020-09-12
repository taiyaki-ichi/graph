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
		//�O���t�̃|�C���^
		static const adjacency_list<Args...>* m_adjacency_list_ptr;
		//�X�^�b�N
		static std::stack<unsigned int> m_stack;
		//�T���������ǂ���
		static std::unordered_map<unsigned int, bool> m_is_searched;

	public:
		static int init(const adjacency_list<Args...>* const g, unsigned int from)
		{
			//from���L���Ȓl�łȂ��Ƃ�
			if (g->get_adjacency_vertex_list().find(from) == g->get_adjacency_vertex_list().end())
				return search_iterator<DFS_iterator_type<IsPreorder,Args...>>::end();

			//�ȉ��A���ꂼ�ꏉ����

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
			//��s���̏ꍇ
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
				//���T���̒��_������ꍇ
				if (!m_is_searched.at(vertex))
				{
					m_is_searched.insert_or_assign(vertex, true);
					m_stack.push(vertex);

					pushFlag = true;
					break;
				}
			}

			//��s���̏ꍇ
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
			//��U���̏ꍇ
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


	//�R�s�[�s�A�͈�for���̈ꎞ�I�u�W�F�N�g�Ƃ��Ďg�p
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

	//�C���^�[�t�F�[�X
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