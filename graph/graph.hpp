#pragma once
#include"utility/get_optional_arg.hpp"
#include<set>
#include<unordered_map>
#include"utility/pair_hash.hpp"
#include<iostream>
#include<optional>

namespace graph
{

	//�L���O���t�̂Ƃ�
	struct directed {
		static const bool value = true;
	};

	struct default_vertex_property {};
	//���_�ɏ�����������Ƃ�
	template<typename T>
	struct set_vertex_property {
		using type = T;
	};

	template<typename>
	struct is_customize_vertex {
		static const bool value = false;
	};
	template<typename T>
	struct is_customize_vertex<set_vertex_property<T>> {
		static const bool value = true;
	};

	struct default_edge_property {};
	//�ӂɏ�����������Ƃ�
	template<typename T>
	struct set_edge_property {
		using type = T;
	};

	template<typename>
	struct is_customize_edge {
		static const bool value = false;
	};
	template<typename T>
	struct is_customize_edge<set_edge_property<T>> {
		static const bool value = true;
	};




	//�O���t�{��
	//�אڃ��X�g�ŊǗ�����\��
	template<typename... Args>
	class adjacency_list{
	public:
		//�g�p���钸�_���
		using vertex_property = typename utility::get_optional_arg<
			set_vertex_property<default_vertex_property>,
			is_customize_vertex,
			Args...
		>::type::type;

		//�g�p����ӂ̏��
		using edge_property = typename utility::get_optional_arg<
			set_edge_property<default_edge_property>,
			is_customize_edge,
			Args...
		>::type::type;

		//�L���O���t���ǂ���
		using is_directed = typename utility::contain<directed, Args...>::type;

	private:
		//�אڃ��X�g
		//��{�I�ɓY�����ŊǗ��A�Ή�����property������
		//�R���e�i�͍��̂Ƃ���vector�Œ��
		std::unordered_map<unsigned int,std::set<unsigned int>> m_adjacency_list;
		//�_�̏��
		std::unordered_map<unsigned int, vertex_property> m_vertex_property_list;
		//�ӂ̏��
		std::unordered_map<std::pair<unsigned int,unsigned int>, edge_property> m_edge_property_list;
		

	public:
		
		//vertex�𐶐�
		bool add_vertex(unsigned int n) {
			if (m_adjacency_list.insert({ n, {} }).second)
			{
				m_vertex_property_list.insert({ n, {} });
				return true;
			}
			else
				return false;
		}

		//edge����
		bool add_edge(const std::pair<unsigned int, unsigned int>& edge) {

			if (m_adjacency_list.find(edge.first) == m_adjacency_list.end() ||
				m_adjacency_list.find(edge.second) == m_adjacency_list.end())
				return false;

			const auto add_if = [this](unsigned int a, unsigned int b)
			{
				if (m_adjacency_list.at(a).insert(b).second)
					m_edge_property_list.insert({ std::make_pair(a,b),{} });
			};

			if constexpr (is_directed::value)
				add_if(edge.first, edge.second);
			else {
				add_if(edge.first, edge.second);
				add_if(edge.second, edge.first);
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
				else
					edgeProperty++;
			}
		}

		//�ӂ̍폜
		void remove_edge(const std::pair<unsigned int, unsigned int>& edge)
		{
			const auto erase_if = [this](unsigned int a, unsigned int b) {
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


	private:
		void print_impl(const std::function<void(unsigned int)>& printFunc)
		{
			for (const auto& v : m_adjacency_list)
			{
				printFunc(v.first);

				//�L���̏ꍇ
				if constexpr (adjacency_list<Args...>::is_directed::value)
					std::cout << " -->";
				else
					std::cout << " ---";

				for (auto num : v.second) {
					std::cout << " "; 
					printFunc(num);
				}

				std::cout << "\n";
			}
		}
	public:
		void print() {
			const auto printFnc = [](unsigned int num) {
				std::cout << num;
			};
			print_impl(printFnc);
		}

		template<typename T>
		void print(T&& str) {
			const auto printFunc = [&str](unsigned int num) {
				std::cout << str[num];
			};
			print_impl(printFunc);
		}

		const std::unordered_map<unsigned int, std::set<unsigned int>>& get_adjacency_vertex_list() const {
			return m_adjacency_list;
		}

		//���钸�_�ɗאڂ��Ă��郊�X�g�̎擾
		const std::set<unsigned int>& get_adjacency_vertex(unsigned int v) const
		{
			return m_adjacency_list.at(v);
		}

		//���ׂĂ̒��_�̓Y�������擾
		std::vector<unsigned int> get_all_vertex_index() const
		{
			std::vector<unsigned int> result;
			for (const auto& tmp : m_adjacency_list)
				result.emplace_back(tmp.first);
			return result;
		}
	};


}