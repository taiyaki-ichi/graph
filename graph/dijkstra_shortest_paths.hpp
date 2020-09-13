#pragma once
#include"adjacency_list.hpp"
#include"utility/find_if.hpp"
#include<optional>

namespace graph
{
	template<typename T,typename Property,typename... Args>
	void dijkstra_shortest_paths_impl(const adjacency_list<Args...>& g, unsigned  int from,
		std::unordered_map<unsigned int,unsigned int>& parent, 
		std::unordered_map<unsigned int,std::optional<T>>& distance,T Property::* edge_property_ptr)
	{
		auto vertex = g.get_all_vertex_index();

		//初期化
		parent.clear();
		for (auto num : vertex)
			parent.emplace(num, num);

		distance.clear();
		for (auto num : vertex)
			distance.emplace(num, std::nullopt);
		distance[0] = T{};

		std::unordered_map<unsigned int,bool> checked{};
		for (auto num : vertex)
			checked.emplace(num, false);


		//vertexのうち、checkedがfalseな要素の集合
		const auto get_not_checked_vertex = [&vertex, &checked]()->std::vector<unsigned int>
		{
			std::vector<unsigned int> result{};
			for (auto v : vertex)
				if (!checked.at(v))
					result.emplace_back(v);
			return result;
		};


		//与えられた頂点から最小の距離の添え字を返す
		const auto get_min_index_and_distance = [&distance](std::vector<unsigned int>&& list)
			->unsigned int
		{
			auto minDistance = distance.at(list[0]);
			unsigned int minIndex = list[0];

			for (unsigned int i = 0; i < list.size(); i++)
			{
				if (minDistance && distance.at(list[i])) {
					if (minDistance.value() > distance.at(list[i]).value())
					{
						minDistance = distance.at(list[i]).value();
						minIndex = list[i];
					}
				}
				else if (!minDistance && distance.at(list[i]))
				{
					minDistance = distance.at(list[i]).value();
					minIndex = list[i];
				}
			}
			return minIndex;
		};


		//すべての辺をチェックしたかどうか
		const auto all_vertex_is_checked = [&checked]()->bool {
			bool result = true;
			for (auto v : checked)
				if (!v.second) {
					result = false;
					break;
				}

			return result;
		};

		//本体
		while (!all_vertex_is_checked())
		{
			//checkedがfalseでないvertexの中からdistance最小の頂点
			auto vs = get_not_checked_vertex();
			auto minIndex= get_min_index_and_distance(std::move(vs));
			
			/*
			std::cout << " not checked list: ";
			for (auto v : vs)
				std::cout << v << " ";
			std::cout << "\n";

			std::cout << "minIndex: " << minIndex << "\n";
			*/

			//最小の距離が有効な値でない場合終了
			if (!distance[minIndex])
				break;

			checked[minIndex] = true;

			/*
			std::cout << "v: ";
			for (auto v : g.get_adjacency_vertex(minIndex))
				std::cout << v << " ";
			std::cout << "\n";
			*/

			
			for (auto v : g.get_adjacency_vertex(minIndex))
			{
				T newDistance = distance.at(minIndex).value() + g[{minIndex, v}].*edge_property_ptr;
				if (!distance.at(v) || distance.at(v).value() > newDistance)
				{
					distance[v] = newDistance;
					parent[v] = minIndex;
				}
			}
		}
	}


	/*


	//最短炉におけるその頂点の一つ前の頂点の記録
	struct predecessor_map
	{
		std::vector<unsigned int>* const m_predecessor_map_ptr;

		predecessor_map(std::vector<unsigned int>& map)
			:m_predecessor_map_ptr{&map}
		{}
	};

	template<typename Head,typename... Tail>
	std::vector<unsigned int>* get_predecessor_map_ptr(const Head& head, const Tail& ...tail)
	{
		if constexpr (std::is_same_v<Head, predecessor_map>)
			return head.m_predecessor_map_ptr;
		else
			return get_predecessor_map_ptr(tail...);
	}

	template<typename T>
	struct distance_map
	{
		std::vector<T>* const m_distance_map_ptr;
		distance_map(std::vector<T>& map)
			:m_distance_map_ptr{ &map }
		{}
	};

	template<typename... Args>
	void dijkstra_shortest_paths_impl(std::unordered_map<unsigned int,std::set<unsigned int>>* list, unsigned int from,Args ...args)
	{
		//Args...内にpredecessor_mapが存在するならvectorの参照型
		using predecessor_map_type = typename std::conditional<
			utility::contain<predecessor_map, Args...>::type::value,
			std::vector<unsigned int>&,
			std::vector<unsigned int>
		>::type;

		//args内にpredecessor_mapが存在するかしないかで初期化の方法を変更
		predecessor_map_type parent;
		if constexpr (utility::contain<predecessor_map, Args...>::type::value)
			parent = *get_predecessor_map_ptr(args...);
		else
			parent = {};

		parent.emplace_back(0);
		parent.emplace_back(1);
		parent.emplace_back(2);

		for (auto n : parent)
			std::cout << n << " ";

	}

	template<typename... Args>
	void aaa(Args&& ...args)
	{
		auto ptr = get_predecessor_map_ptr(args...);

		if (ptr)
		{
			std::vector<unsigned int>& list = *ptr;
	
			list.emplace_back(0);
			list.emplace_back(1);
			list.emplace_back(2);

			std::cout << "a ";
		}
	}
	*/

}