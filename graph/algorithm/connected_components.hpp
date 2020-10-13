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

		//�T���������̃��X�g�̍쐻
		auto vert = g.get_all_vertexes();
		std::unordered_map<unsigned int, bool> is_checked{};
		for (auto v : vert)
			is_checked.emplace(v, false);

		for (auto value : is_checked)
		{
			//���T���̏ꍇ
			if (!value.second)
			{
				std::vector<unsigned int> group{};
				//�[���D��T���ŘA�������̌��o
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

	//�����I
	template<typename... Args>
	std::vector<std::vector<unsigned int>> strong_connected_components(const adjacency_list<Args...>& g)
	{
		std::vector<std::vector<unsigned int>> result{};

		auto vert = g.get_all_vertexes();
		std::unordered_map<unsigned int, bool> is_checked{};
		for (auto v : vert)
			is_checked.emplace(v, false);

		//��U���̃���
		std::vector<unsigned int> postorder{};

		for (auto value : is_checked)
		{
			//���T���̏ꍇ
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

		//�T���������ǂ����̃��X�g�쐬
		is_checked.clear();
		for (auto v : vert)
			is_checked.emplace(v, false);

		while (!postorder.empty())
		{
			//��s���ɐ[���D��T��
			auto start = postorder.back();
			postorder.pop_back();
			
			//start�����T���̏ꍇ
			if (!is_checked[start])
			{
				std::vector<unsigned int> group{};
				for (auto v : DFS_postorder{ newG,start }) {
					//���T���̏ꍇGroup�ɒǉ�
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