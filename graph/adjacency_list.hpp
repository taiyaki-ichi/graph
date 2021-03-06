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
		constexpr adjacency_list() = default;
		virtual ~adjacency_list() = default;

		template<typename Container>
		constexpr adjacency_list(Container&& list)
			:m_adjacency_list {}
			, m_vertex_property_list{}
			, m_edge_property_list{}
		{
			//コンテナの要素が頂点の遠きのとき
			if constexpr (std::is_same_v<typename Container::value_type, unsigned int>)
			{
				for (auto v : list)
					add_vertex(v);
			}
			//辺のとき
			else if constexpr (std::is_same_v<typename Container::value_type, std::pair<unsigned int, unsigned int>>)
			{
				for (auto edge : list)
				{
					//頂点追加
					add_vertex(edge.first);
					add_vertex(edge.second);

					add_edge(std::move(edge));
				}
			}
		}

		constexpr adjacency_list(const adjacency_list& rhs) {
			m_adjacency_list = rhs.m_adjacency_list;
			m_vertex_property_list = rhs.m_vertex_property_list;
			m_edge_property_list = rhs.m_edge_property_list;
		}
		constexpr adjacency_list(adjacency_list&& rhs) {
			m_adjacency_list = std::move(rhs.m_adjacency_list);
			m_vertex_property_list = std::move(rhs.m_vertex_property_list);
			m_edge_property_list = std::move(rhs.m_edge_property_list);
		}

		constexpr adjacency_list& operator=(const adjacency_list& rhs) {
			m_adjacency_list = rhs.m_adjacency_list;
			m_vertex_property_list = rhs.m_vertex_property_list;
			m_edge_property_list = rhs.m_edge_property_list;
			return *this;
		}
		constexpr adjacency_list& operator=(adjacency_list&& rhs) {
			m_adjacency_list = std::move(rhs.m_adjacency_list);
			m_vertex_property_list = std::move(rhs.m_vertex_property_list);
			m_edge_property_list = std::move(rhs.m_edge_property_list);
			return this;
		}
	
		//vertexを生成
		constexpr bool add_vertex(unsigned int n) {
			if (m_adjacency_list.try_emplace(n, std::set<unsigned int>{}).second)
			{
				m_vertex_property_list.emplace(n, vertex_property{});
				return true;
			}
			else
				return false;
		}

		//edge生成
		constexpr bool add_edge(const std::pair<unsigned int, unsigned int>& edge) {

			if (m_adjacency_list.find(edge.first) == m_adjacency_list.end() ||
				m_adjacency_list.find(edge.second) == m_adjacency_list.end())
				return false;

			auto add_if = [this](unsigned int a, unsigned int b)
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

		constexpr bool add_edge(const std::pair<unsigned int, unsigned int>& edge, const edge_property& property) {
			if (add_edge(edge))
			{
				set_property({ edge, property });
				return true;
			}
			else
				return false;
		}
		
		//頂点と関連する辺を削除
		constexpr void remove_vertex(unsigned int v)
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
		constexpr void remove_edge(const std::pair<unsigned int, unsigned int>& edge)
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
		constexpr void print_impl(const std::function<void(unsigned int)>& printFunc)
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
		constexpr void print() {
			const auto printFnc = [](unsigned int num) {
				std::cout << num;
			};
			print_impl(printFnc);
		}

		template<typename T>
		constexpr void print(T&& str) {
			const auto printFunc = [&str](unsigned int num) {
				std::cout << str[num];
			};
			print_impl(printFunc);
		}

		//全部のリストの取得
		constexpr const std::unordered_map<unsigned int, std::set<unsigned int>>& get_adjacency_vertex_list() const {
			return m_adjacency_list;
		}

		//ある頂点に隣接しているリストの取得
		constexpr const std::set<unsigned int>& get_adjacency_vertex(unsigned int v) const
		{
			return m_adjacency_list.at(v);
		}

		//すべての頂点を取得
		constexpr std::vector<unsigned int> get_all_vertexes() const
		{
			std::vector<unsigned int> result{};
			for (const auto& tmp : m_adjacency_list)
				result.emplace_back(tmp.first);
			return result;
		}

		//グラフのすべての辺の取得
		constexpr std::vector<std::pair<unsigned int, unsigned int>> get_all_edges() const
		{
			std::vector<std::pair<unsigned int, unsigned int>> result{};
			for (const auto& vert : m_adjacency_list)
			{
				unsigned int from = vert.first;
				for (auto to : vert.second)
					result.emplace_back(std::make_pair(from,to));
			}

			return result;
		}

		constexpr const vertex_property& operator[](unsigned int v) const {
			return m_vertex_property_list[v];
		}
		constexpr vertex_property& operator[](unsigned int v) {
			return m_vertex_property_list[v];
		}

		constexpr const edge_property& operator[](const std::pair<unsigned int, unsigned int>& edge) const {
			return m_edge_property_list.at(edge);
		}
		constexpr edge_property& operator[](const std::pair<unsigned int, unsigned int>& edge) {
			return m_edge_property_list[edge];
		}

		//頂点情報のセッティング
		constexpr void set_property(std::pair<unsigned int, vertex_property>&& p) {
			m_vertex_property_list[p.first] = std::move(p.second);
		}
		constexpr void set_property(std::vector<std::pair<unsigned int, vertex_property>>&& ps) {
			for (auto&& p : ps)
				set_property(std::move(p));
		}

		//辺の情報のセッティング
		constexpr void set_property(std::pair<std::pair<unsigned int, unsigned int>, edge_property>&& p) {
			if constexpr (is_directed::value)
				m_edge_property_list[p.first] = p.second;
			else
			{
				m_edge_property_list[p.first] = p.second;
				m_edge_property_list[{p.first.second, p.first.first}] = p.second;
			}
		}
		constexpr void set_property(std::vector<std::pair<std::pair<unsigned int, unsigned int>, edge_property>>&& ps) {
			for (auto&& p : ps)
				set_property(std::move(p));
		}

	};


}