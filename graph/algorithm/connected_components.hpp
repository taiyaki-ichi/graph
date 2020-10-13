#pragma once
#include"../adjacency_list.hpp"
#include"../iterator/DFS.hpp"

namespace graph
{
	struct weak {};
	struct strong {};

	//���ʂ̘A���̌��o�p
	template<typename... Args>
	std::vector<std::vector<unsigned int>> weak_connected_components(const adjacency_list<Args...>&);

	//�L���O���t�̋��A���̌��o�p
	template<typename... Args>
	std::vector<std::vector<unsigned int>> strong_connected_components(const adjacency_list<Args...>&);
	
	//WeakOrStrength�͗L���O���t�̏ꍇ�K�p�\
	//�����O���t�Ȃ疳��
	//�߂�l�̓O���[�v�A���̂Ƃ���
	template<typename WeakOrStrength=weak,typename... Args>
	std::vector<std::vector<unsigned int>> connected_components(const adjacency_list<Args...>& g)
	{
		using isDirected = typename utility::contain<directed, Args...>::type;
		using isStrong = typename std::is_same<WeakOrStrength, strong>::type;

		//�L���O���t�ł��苭�A�������߂�ꍇ
		if constexpr (std::conjunction_v<isDirected, isStrong>)
			return strong_connected_components(g);
		//����ȊO
		else
			return weak_connected_components(g);
	}


	//����
	template<typename... Args>
	std::vector<std::vector<unsigned int>> weak_connected_components(const adjacency_list<Args...>& g)
	{
		std::vector<std::vector<unsigned int>> result{};

		auto uncheked_index = g.get_all_vertexes();

		while (!uncheked_index.empty())
		{
			std::vector<unsigned int> group{};
			//�[���D��T���ŘA�������̌��o
			for (auto iter : DFS_preorder{ g,uncheked_index[0] })
			{
				auto v = static_cast<unsigned int>(iter);
				group.emplace_back(v);

				//O(n)�͌���
				uncheked_index.erase(std::remove(uncheked_index.begin(), uncheked_index.end(), v), uncheked_index.end());
			}

			result.emplace_back(std::move(group));
		}

		return result;
	}

	//�����I
	template<typename... Args>
	std::vector<std::vector<unsigned int>> strong_connected_components(const adjacency_list<Args...>& g)
	{
		std::vector<std::vector<unsigned int>> result{};

		auto unchecked_v = g.get_all_vertexes();
		//��U���̃���
		std::vector<unsigned int> postorder{};

		unsigned int num = 1;
		while (!unchecked_v.empty())
		{
			for (auto v : DFS_postorder{ g,unchecked_v[0] })
			{
				postorder.emplace_back(v);
				num++;

				//O(n)�͌���
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
			//��s���ɐ[���D��T��
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