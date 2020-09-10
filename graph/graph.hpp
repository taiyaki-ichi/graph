#pragma once
#include"utility/get_optional_arg.hpp"
#include<vector>
#include<unordered_map>

namespace graph
{

	//�L���O���t�̂Ƃ�
	struct directed {
		static const bool value = true;
	};

	struct default_vertex_property {};
	//���_�ɏ�����������Ƃ�
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
	//�ӂɏ�����������Ƃ�
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

	//�O���t�{��
	//�אڃ��X�g�ŊǗ�����\��
	template<typename... Args>
	class graph{

		//�g�p���钸�_���
		using vertex_property = typename utility::get_optional_arg<
			default_vertex_property,
			is_customize_vertex,
			Args...
		>::type;

		//�g�p����ӂ̏��
		using edge_property = typename utility::get_optional_arg<
			default_edge_property,
			is_customize_edge,
			Args...
		>::type;

		//�L���O���t���ǂ���
		template<typename T>
		using is_directed_impl = std::is_same<directed, T>;
		using is_directed = typename std::is_same <
			typename utility::find_if<is_directed_impl, Args...>::type,
			directed
		>;

		//�אڃ��X�g
		//��{�I�ɓY�����ŊǗ��A�Ή�����property������
		//�R���e�i�͍��̂Ƃ���vector�Œ��
		std::unordered_map<unsigned int,std::vector<unsigned int>> m_adjacency_list;
		//�_�̏��
		std::unordered_map<unsigned int, vertex_property> m_vertex_list;
		//�ӂ̏��
		std::unordered_map<std::pair<unsigned int,unsigned int>, edge_property> m_edge_list;
		

	public:
		
		//vertex�𐶐��A�����ŕێ��A�A�N�Z�X�q��n��
		bool add_vertex(unsigned int);
		//����
		bool add_edge(const std::pair<unsigned int, unsigned int>&);
		
		void remove_vertex(unsigned int);
		void remove_edge(const std::pair<unsigned int,unsigned int>&);
		void remove_edge(unsigned int);
	};

}