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

		//探索したかのリストの作製
		auto vert = g.get_all_vertexes();
		std::unordered_map<unsigned int, bool> is_checked{};
		for (auto v : vert)
			is_checked.emplace(v, false);

		for (auto value : is_checked)
		{
			//未探索の場合
			if (!value.second)
			{
				std::vector<unsigned int> group{};
				//深さ優先探索で連結部分の検出
				for (auto v : DFS_preorder{ g,value.first })
				{
					group.emplace_back(v);
					is_checked.insert_or_assign(v, true);
				}

				result.emplace_back(std::move(group));
			}
		}

		return result;
	}

	//実装！
	template<typename... Args>
	std::vector<std::vector<unsigned int>> strong_connected_components(const adjacency_list<Args...>& g)
	{
		std::vector<std::vector<unsigned int>> result{};

		auto vert = g.get_all_vertexes();
		std::unordered_map<unsigned int, bool> is_checked{};
		for (auto v : vert)
			is_checked.emplace(v, false);

		//後攻順のメモ
		std::vector<unsigned int> postorder{};

		for (auto value : is_checked)
		{
			//未探索の場合
			if (!value.second)
			{
				for (auto v : DFS_postorder{ g,value.first })
				{
					postorder.emplace_back(v);
					is_checked.insert_or_assign(v, true);
				}
			}
		}

		auto reverse = [](std::pair<unsigned int, unsigned int>& p) {return std::make_pair(p.second, p.first); };
		auto edge1 = g.get_all_edges();
		auto edge = std::vector<std::pair<unsigned int, unsigned int>>{};
		std::transform(edge1.begin(), edge1.end(),std::back_inserter(edge), std::move(reverse));

		auto newG = adjacency_list<Args...>{ std::move(edge) };

		//探索したかどうかのリスト作成
		is_checked.clear();
		for (auto v : vert)
			is_checked.emplace(v, false);

		while (!postorder.empty())
		{
			//後行順に深さ優先探索
			auto start = postorder.back();
			postorder.pop_back();
			
			//startが未探索の場合
			if (!is_checked[start])
			{
				std::vector<unsigned int> group{};
				for (auto v : DFS_postorder{ newG,start }) {
					//未探索の場合Groupに追加
					if (!is_checked[v])
					{
						group.emplace_back(v);
						is_checked.insert_or_assign(v, true);
					}
				}
				result.emplace_back(group);
			}
			
		}

		return result;
	}




}