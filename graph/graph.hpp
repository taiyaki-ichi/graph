#pragma once
#include"utility/get_optional_arg.hpp"
#include<set>
#include<unordered_map>
#include"utility/pair_hash.hpp"
#include<iostream>
#include<optional>

namespace graph
{

	//有効グラフのとき
	struct directed {
		static const bool value = true;
	};

	struct default_vertex_property {};
	//頂点に情報を持たせるとき
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
	//辺に情報を持たせるとき
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




	//グラフ本体
	//隣接リストで管理する予定
	template<typename... Args>
	class adjacency_list{
	public:
		//使用する頂点情報
		using vertex_property = typename utility::get_optional_arg<
			set_vertex_property<default_vertex_property>,
			is_customize_vertex,
			Args...
		>::type::type;

		//使用する辺の情報
		using edge_property = typename utility::get_optional_arg<
			set_edge_property<default_edge_property>,
			is_customize_edge,
			Args...
		>::type::type;

		//有効グラフかどうか
		using is_directed = typename utility::contain<directed, Args...>::type;

	private:
		//隣接リスト
		//基本的に添え字で管理、対応するpropertyが存在
		//コンテナは今のところvector固定で
		std::unordered_map<unsigned int,std::set<unsigned int>> m_adjacency_list;
		//点の情報
		std::unordered_map<unsigned int, vertex_property> m_vertex_property_list;
		//辺の情報
		std::unordered_map<std::pair<unsigned int,unsigned int>, edge_property> m_edge_property_list;
		

	public:
		
		//vertexを生成
		bool add_vertex(unsigned int n) {
			if (m_adjacency_list.insert({ n, {} }).second)
			{
				m_vertex_property_list.insert({ n, {} });
				return true;
			}
			else
				return false;
		}

		//edge生成
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
		
		//頂点と関連する辺を削除
		void remove_vertex(unsigned int v)
		{
			//リストの操作
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

		//辺の削除
		void remove_edge(const std::pair<unsigned int, unsigned int>& edge)
		{
			const auto erase_if = [this](unsigned int a, unsigned int b) {
				auto iter = m_adjacency_list.find(a);
				if (iter != m_adjacency_list.end())
					iter->second.erase(b);
			};

			//有効グラフの場合
			if constexpr (is_directed::value)
				erase_if(edge.first, edge.second);
			//無効グラフ
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

				//有効の場合
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

		//ある頂点に隣接しているリストの取得
		const std::set<unsigned int>& get_adjacency_vertex(unsigned int v) const
		{
			return m_adjacency_list.at(v);
		}

		//すべての頂点の添え字を取得
		std::vector<unsigned int> get_all_vertex_index() const
		{
			std::vector<unsigned int> result;
			for (const auto& tmp : m_adjacency_list)
				result.emplace_back(tmp.first);
			return result;
		}
	};


}