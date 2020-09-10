#pragma once
#include"utility/get_optional_arg.hpp"
#include<vector>
#include<unordered_map>

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
		template<typename T>
		using is_directed_impl = std::is_same<directed, T>;
		using is_directed = typename std::is_same <
			typename utility::find_if<is_directed_impl, Args...>::type,
			directed
		>;

		//隣接リスト
		//基本的に添え字で管理、対応するpropertyが存在
		//コンテナは今のところvector固定で
		std::unordered_map<unsigned int,std::vector<unsigned int>> m_adjacency_list;
		//点の情報
		std::unordered_map<unsigned int, vertex_property> m_vertex_list;
		//辺の情報
		std::unordered_map<std::pair<unsigned int,unsigned int>, edge_property> m_edge_list;
		

	public:
		
		//vertexを生成、内部で保持、アクセス子を渡す
		bool add_vertex(unsigned int);
		//同じ
		bool add_edge(const std::pair<unsigned int, unsigned int>&);
		
		void remove_vertex(unsigned int);
		void remove_edge(const std::pair<unsigned int,unsigned int>&);
		void remove_edge(unsigned int);
	};

}