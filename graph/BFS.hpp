#pragma once
#include"adjacency_list.hpp"
#include<queue>
#include"search_iterator.hpp"

namespace graph
{

	template<typename... Args>
	struct BFS_iterator_type
	{
	private:
		//�O���t�̃|�C���^
		static const adjacency_list<Args...>* m_adjacency_list_ptr;
		//�L���[
		static std::queue<unsigned int> m_queue;
		//�T���ς݂��ǂ���
		static std::unordered_map<unsigned int, bool> m_is_searched;

	public:
		static int init(const adjacency_list<Args...>* const g, unsigned int from)
		{
			//from���L���Ȓl�łȂ��Ƃ�
			if (g->get_adjacency_vertex_list().find(from) == g->get_adjacency_vertex_list().end())
				return search_iterator<BFS_iterator_type<Args...>>::end();

			//�ȉ��A���ꂼ�ꏉ����

			m_is_searched.clear();
			auto vertex = g->get_all_vertex_index();
			for (auto num : vertex)
				m_is_searched.insert({ num, false });
			m_is_searched.insert_or_assign( from,true );

			while (!m_queue.empty())
				m_queue.pop();
			m_queue.push(from);

			m_adjacency_list_ptr = g;

			return from;
		}

		static int increment(int num)
		{
			int next{};
			while(1)
			{
				next = action(num);
				//����ɂ���ēY�������i�߂�ꂽ�ꍇ
				if (next != num)
					return next;
			}
		}

	private:
		//�A���S���Y����1�X�e�b�v�i�߂�
		static int action(int num)
		{
			auto vertexList = m_adjacency_list_ptr->get_adjacency_vertex(m_queue.front());
			for (auto v : vertexList)
			{
				//���T���̒��_������ꍇ
				if (!m_is_searched.at(v))
				{
					m_is_searched.insert_or_assign(v, true);
					m_queue.push(v);

					return v;
				}
			}
			
			m_queue.pop();

			if (m_queue.empty())
				return search_iterator<BFS_iterator_type<Args...>>::end();
			else
				return num;
		}
	};

	template<typename... Args>
	const adjacency_list<Args...>* BFS_iterator_type<Args...>::m_adjacency_list_ptr = nullptr;
	template<typename... Args>
	std::queue<unsigned int> BFS_iterator_type<Args...>::m_queue{};
	template<typename... Args>
	std::unordered_map<unsigned int, bool> BFS_iterator_type<Args...>::m_is_searched{};


	//�R�s�[�s�A�͈�for���̈ꎞ�I�u�W�F�N�g�Ƃ��Ďg�p
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
			return BFS_iter::begin(m_graph, m_from);
		}

		BFS_iter end() {
			return BFS_iter::end();
		}

		BFS& operator=(const BFS&) = delete;
		BFS& operator=(BFS&&) = delete;
		BFS(const BFS&) = delete;
	};


}