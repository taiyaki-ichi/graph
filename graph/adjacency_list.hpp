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
		adjacency_list() = default;
		virtual ~adjacency_list() = default;

		template<typename Container>
		adjacency_list(Container&& list)
			:m_adjacency_list {}
			, m_vertex_property_list{}
			, m_edge_property_list{}
		{
			//�R���e�i�̗v�f�����_�̉����̂Ƃ�
			if constexpr (std::is_same_v<typename Container::value_type, unsigned int>)
			{
				for (auto v : list)
					add_vertex(v);
			}
			//�ӂ̂Ƃ�
			else if constexpr (std::is_same_v<typename Container::value_type, std::pair<unsigned int, unsigned int>>)
			{
				for (auto edge : list)
				{
					//���_�ǉ�
					add_vertex(edge.first);
					add_vertex(edge.second);

					add_edge(std::move(edge));
				}
			}
		}

		adjacency_list(const adjacency_list& rhs) {
			m_adjacency_list = rhs.m_adjacency_list;
			m_vertex_property_list = rhs.m_vertex_property_list;
			m_edge_property_list = rhs.m_edge_property_list;
		}
		adjacency_list(adjacency_list&& rhs) {
			m_adjacency_list = std::move(rhs.m_adjacency_list);
			m_vertex_property_list = std::move(rhs.m_vertex_property_list);
			m_edge_property_list = std::move(m_edge_property_list);
		}

		adjacency_list& operator=(const adjacency_list& rhs) {
			m_adjacency_list = rhs.m_adjacency_list;
			m_vertex_property_list = rhs.m_vertex_property_list;
			m_edge_property_list = rhs.m_edge_property_list;
			return *this;
		}
		adjacency_list& operator=(adjacency_list&& rhs) {
			m_adjacency_list = std::move(rhs.m_adjacency_list);
			m_vertex_property_list = std::move(rhs.m_vertex_property_list);
			m_edge_property_list = std::move(m_edge_property_list);
			return this;
		}
	
		//vertex�𐶐�
		bool add_vertex(unsigned int n) {
			if (m_adjacency_list.try_emplace(n, std::set<unsigned int>{}).second)
			{
				m_vertex_property_list.emplace(n, vertex_property{});
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

		bool add_edge(const std::pair<unsigned int, unsigned int>& edge, const edge_property& property) {
			if (add_edge(edge))
			{
				set_property({ edge, property });
				return true;
			}
			else
				return false;
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

		//�S���̃��X�g�̎擾
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

		const vertex_property& operator[](unsigned int v) const {
			return m_vertex_property_list[v];
		}
		vertex_property& operator[](unsigned v) {
			return m_vertex_property_list[v];
		}

		const edge_property& operator[](const std::pair<unsigned int, unsigned int>& edge) const {
			return m_edge_property_list.at(edge);
		}
		edge_property& operator[](const std::pair<unsigned int, unsigned int>& edge) {
			return m_edge_property_list[edge];
		}

		//���_���̃Z�b�e�B���O
		void set_property(std::pair<unsigned int, vertex_property>&& p) {
			m_vertex_property_list[p.first] = std::move(p.second);
		}
		void set_property(std::vector<std::pair<unsigned int, vertex_property>>&& ps) {
			for (auto&& p : ps)
				set_property(std::move(p));
		}

		//�ӂ̏��̃Z�b�e�B���O
		void set_property(std::pair<std::pair<unsigned int, unsigned int>, edge_property>&& p) {
			if constexpr (is_directed::value)
				m_edge_property_list[p.first] = p.second;
			else
			{
				m_edge_property_list[p.first] = p.second;
				m_edge_property_list[{p.first.second, p.first.first}] = p.second;
			}
		}
		void set_property(std::vector<std::pair<std::pair<unsigned int, unsigned int>, edge_property>>&& ps) {
			for (auto&& p : ps)
				set_property(std::move(p));
		}





	};


}