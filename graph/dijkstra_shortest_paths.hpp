#pragma once
#include"adjacency_list.hpp"
#include"utility/find_if.hpp"
#include<optional>

namespace graph
{
	template<typename T,typename Property,typename... Args>
	void dijkstra_shortest_paths(const adjacency_list<Args...>& g, unsigned int from,
		T Property::* edge_property_ptr,
		std::unordered_map<unsigned int,unsigned int>* parent, 
		std::unordered_map<unsigned int,std::optional<T>>* distance)
	{
		auto vertex = g.get_all_vertex_index();

		//初期化
		parent->clear();
		for (auto num : vertex)
			parent->emplace(num, num);

		distance->clear();
		for (auto num : vertex)
			distance->emplace(num, std::nullopt);
		(*distance)[0] = T{};

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
		const auto get_min_index_and_distance = [distance](std::vector<unsigned int>&& list)
			->unsigned int
		{
			auto minDistance = distance->at(list[0]);
			unsigned int minIndex = list[0];

			for (unsigned int i = 0; i < list.size(); i++)
			{
				if (minDistance && distance->at(list[i])) {
					if (minDistance.value() > distance->at(list[i]).value())
					{
						minDistance = distance->at(list[i]).value();
						minIndex = list[i];
					}
				}
				else if (!minDistance && distance->at(list[i]))
				{
					minDistance = distance->at(list[i]).value();
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

			//最小の距離が有効な値でない場合終了
			if (!(*distance)[minIndex])
				break;

			checked[minIndex] = true;
			
			for (auto v : g.get_adjacency_vertex(minIndex))
			{
				T newDistance = distance->at(minIndex).value() + g[{minIndex, v}].*edge_property_ptr;
				if (!(distance->at(v)) || distance->at(v).value() > newDistance)
				{
					(*distance)[v] = newDistance;
					(*parent)[v] = minIndex;
				}
			}
		}
	}

	template<typename T, typename Property, typename... Args>
	void dijkstra_shortest_paths(const adjacency_list<Args...>& g, unsigned  int from,
		T Property::* edge_property_ptr,
		std::unordered_map<unsigned int, unsigned int>* parent)
	{
		std::unordered_map<unsigned int, std::optional<T>> distance{};
		dijkstra_shortest_paths(g, from, edge_property_ptr, parent, &distance);
	}
}