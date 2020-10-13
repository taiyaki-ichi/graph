#pragma once
#include"../adjacency_list.hpp"
#include"../iterator/DFS.hpp"

namespace graph
{
	struct weak {};
	struct strong {};

	//普通の連結の検出用
	template<typename... Args>
	std::vector<std::vector<unsigned int>> weak_connected_components(const adjacency_list<Args...>&);

	//有効グラフの強連結の検出用
	template<typename... Args>
	std::vector<std::vector<unsigned int>> strong_connected_components(const adjacency_list<Args...>&);
	
	//WeakOrStrengthは有効グラフの場合適用可能
	//無効グラフなら無視
	//戻り値はグループ、今のところ
	template<typename WeakOrStrength=weak,typename... Args>
	std::vector<std::vector<unsigned int>> connected_components(const adjacency_list<Args...>& g)
	{
		using isDirected = typename utility::contain<directed, Args...>::type;
		using isStrong = typename std::is_same<WeakOrStrength, strong>::type;

		//有効グラフであり強連結を求める場合
		if constexpr (std::conjunction_v<isDirected, isStrong>)
			return strong_connected_components(g);
		//それ以外
		else
			return weak_connected_components(g);
	}


	//実装
	template<typename... Args>
	std::vector<std::vector<unsigned int>> weak_connected_components(const adjacency_list<Args...>& g)
	{
		std::vector<std::vector<unsigned int>> result{};

		auto uncheked_index = g.get_all_vertexes();

		while (!uncheked_index.empty())
		{
			std::vector<unsigned int> group{};
			//深さ優先探索で連結部分の検出
			for (auto iter : DFS_preorder{ g,uncheked_index[0] })
			{
				auto v = static_cast<unsigned int>(iter);
				group.emplace_back(v);

				//O(n)は嫌だ
				uncheked_index.erase(std::remove(uncheked_index.begin(), uncheked_index.end(), v), uncheked_index.end());
			}

			result.emplace_back(std::move(group));
		}

		return result;
	}

	//実装！
	template<typename... Args>
	std::vector<std::vector<unsigned int>> strong_connected_components(const adjacency_list<Args...>& g)
	{
		std::vector<std::vector<unsigned int>> result{};

		auto unchecked_v = g.get_all_vertexes();
		//後攻順のメモ
		std::vector<unsigned int> postorder{};

		unsigned int num = 1;
		while (!unchecked_v.empty())
		{
			for (auto v : DFS_postorder{ g,unchecked_v[0] })
			{
				postorder.emplace_back(v);
				num++;

				//O(n)は嫌だ
				unchecked_v.erase(std::remove(unchecked_v.begin(), unchecked_v.end(), v), unchecked_v.end());
			}
		}

		auto reverse = [](std::pair<unsigned int, unsigned int>& p) {return std::make_pair(p.second, p.first); };
		auto edge1 = g.get_all_edges();
		auto edge = std::vector<std::pair<unsigned int, unsigned int>>{};
		std::transform(edge1.begin(), edge1.end(),std::back_inserter(edge), std::move(reverse));

		auto newG = adjacency_list<Args...>{ std::move(edge) };

		while (!postorder.empty())
		{
			//後行順に深さ優先探索
			auto start = *(postorder.end() - 1);
			postorder.erase(postorder.end() - 1);

			std::vector<unsigned int> group{};
			for (auto i : DFS_postorder{ newG,start })
			{
				auto v = static_cast<unsigned int>(i);
				group.emplace_back(v);
			}

			for (auto v : group)
				newG.remove_vertex(v);
			
			if (!group.empty())
				result.emplace_back(group);
		}

		return result;
	}




}