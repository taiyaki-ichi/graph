#pragma once
#include"utility/get_optional_arg.hpp"
#include<set>
#include<unordered_map>
#include"pair_hash.hpp"
#include<iostream>

namespace graph
{

	//�L���O���t�̂Ƃ�
	struct directed {
		static const bool value = true;
	};

	struct default_vertex_property {};
	//���_�ɏ�����������Ƃ�
	template<typename T>
	struct customize_vertex_property {};

	template<typename>
	struct is_customize_vertex {
		static const bool value = false;
	};
	template<typename T>
	struct is_customize_vertex<customize_vertex_property<T>> {
		static const bool value = true;
	};

	struct default_edge_property {};
	//�ӂɏ�����������Ƃ�
	template<typename T>
	struct customize_edge_property {};

	template<typename>
	struct is_customize_edge {
		static const bool value = false;
	};
	template<typename T>
	struct is_customize_edge<customize_edge_property<T>> {
		static const bool value = true;
	};




	//�O���t�{��
	//�אڃ��X�g�ŊǗ�����\��
	template<typename... Args>
	class graph{

		//�g�p���钸�_���
		using vertex_property = typename utility::get_optional_arg<
			default_vertex_property,
			is_customize_vertex,
			Args...
		>::type;

		//�g�p����ӂ̏��
		using edge_property = typename utility::get_optional_arg<
			default_edge_property,
			is_customize_edge,
			Args...
		>::type;

		//�L���O���t���ǂ���
		using is_directed = typename utility::contain<directed, Args...>::type;

		//�אڃ��X�g
		//��{�I�ɓY�����ŊǗ��A�Ή�����property������
		//�R���e�i�͍��̂Ƃ���vector�Œ��
		std::unordered_map<unsigned int,std::set<unsigned int>> m_adjacency_list;
		//�_�̏��
		std::unordered_map<unsigned int, vertex_property> m_vertex_property_list;
		//�ӂ̏��
		std::unordered_map<std::pair<unsigned int,unsigned int>, edge_property> m_edge_property_list;
		

	public:
		
		graph() = default;
		~graph() = default;

		
		//vertex�𐶐��A�����ŕێ��A�A�N�Z�X�q��n��
		bool add_vertex(unsigned int n) {
			return m_adjacency_list.insert({ n, {} }).second;
		}
	
		bool add_edge(const std::pair<unsigned int, unsigned int>& edge) {

			if (m_adjacency_list.find(edge.first) == m_adjacency_list.end() ||
				m_adjacency_list.find(edge.second) == m_adjacency_list.end())
				return false;

			if constexpr (is_directed::value)
				m_adjacency_list.at(edge.first).insert(edge.second);
			else {
				m_adjacency_list.at(edge.first).insert(edge.second);
				m_adjacency_list.at(edge.second).insert(edge.first);
			}
				

			return true;
		}
		
		//���_�Ɗ֘A����ӂ��폜
		void remove_vertex(unsigned int v)
		{
			//���X�g�̑���
			auto iter = m_adjacency_list.find(v);
			if (iter != m_adjacency_list.end())
			{
				m_adjacency_list.erase(iter);
				for (auto& set : m_adjacency_list)
					set.second.erase(v);
			}

			auto vertexProperty = m_vertex_property_list.find(v);
			if (vertexProperty != m_vertex_property_list.end())
				m_vertex_property_list.erase(vertexProperty);

			auto edgeProperty = m_edge_property_list.begin();
			while (edgeProperty != m_edge_property_list.end())
			{
				if (edgeProperty->first.first == v || edgeProperty->first.second == v)
					edgeProperty = m_edge_property_list.erase(edgeProperty);
				edgeProperty++;
			}
		}

		//�ӂ̍폜
		void remove_edge(const std::pair<unsigned int, unsigned int>& edge)
		{
			auto erase_if = [this](unsigned int a, unsigned int b) {
				auto iter = m_adjacency_list.find(a);
				if (iter != m_adjacency_list.end())
					iter->second.erase(b);
			};

			//�L���O���t�̏ꍇ
			if constexpr (is_directed::value)
				erase_if(edge.first, edge.second);
			//�����O���t
			else
			{
				erase_if(edge.first, edge.second);
				erase_if(edge.second, edge.first);
			}

			auto edgeProperty = m_edge_property_list.find(edge);
			if (edgeProperty != m_edge_property_list.end())
				m_edge_property_list.erase(edgeProperty);
		}

		//�o��
		template<typename... Args>
		friend std::ostream& operator<<(std::ostream&, const graph<Args...>&);
	};

	template<typename... Args>
	std::ostream& operator<<(std::ostream& os, const graph<Args...>& g)
	{
		for (const auto& v : g.m_adjacency_list)
		{
			std::cout << v.first;

			//�L���̏ꍇ
			if constexpr (graph<Args...>::is_directed::value)
				std::cout << " -->";
			else
				std::cout << " ---";

			for (auto num : v.second)
				std::cout << " " << num;

			std::cout << "\n";
		}

		return os;
	}

}