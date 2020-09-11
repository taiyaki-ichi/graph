#pragma once
#include"utility/get_optional_arg.hpp"
#include<set>
#include<unordered_map>
#include"pair_hash.hpp"
#include<iostream>

namespace graph
{

	//有効グラフのとき
	struct directed {
		static const bool value = true;
	};

	struct default_vertex_property {};
	//頂点に情報を持たせるとき
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
	//辺に情報を持たせるとき
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




	//グラフ本体
	//隣接リストで管理する予定
	template<typename... Args>
	class graph{

		//使用する頂点情報
		using vertex_property = typename utility::get_optional_arg<
			default_vertex_property,
			is_customize_vertex,
			Args...
		>::type;

		//使用する辺の情報
		using edge_property = typename utility::get_optional_arg<
			default_edge_property,
			is_customize_edge,
			Args...
		>::type;

		//有効グラフかどうか
		using is_directed = typename utility::contain<directed, Args...>::type;

		//隣接リスト
		//基本的に添え字で管理、対応するpropertyが存在
		//コンテナは今のところvector固定で
		std::unordered_map<unsigned int,std::set<unsigned int>> m_adjacency_list;
		//点の情報
		std::unordered_map<unsigned int, vertex_property> m_vertex_property_list;
		//辺の情報
		std::unordered_map<std::pair<unsigned int,unsigned int>, edge_property> m_edge_property_list;
		

	public:
		
		graph() = default;
		~graph() = default;

		
		//vertexを生成、内部で保持、アクセス子を渡す
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
				edgeProperty++;
			}
		}

		//辺の削除
		void remove_edge(const std::pair<unsigned int, unsigned int>& edge)
		{
			auto erase_if = [this](unsigned int a, unsigned int b) {
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

		//出力
		template<typename... Args>
		friend std::ostream& operator<<(std::ostream&, const graph<Args...>&);
	};

	template<typename... Args>
	std::ostream& operator<<(std::ostream& os, const graph<Args...>& g)
	{
		for (const auto& v : g.m_adjacency_list)
		{
			std::cout << v.first;

			//有効の場合
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