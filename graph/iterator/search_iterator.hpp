#pragma once
#include<stack>
#include<unordered_map>
#include<memory>
#include<type_traits>
#include"../adjacency_list.hpp"

namespace graph
{

	//���ƂƂȂ�C�e���[�^���ǂ�
	//SerchBody�Ƀf�t�H���g�R���X�g���N�^,init,increment��v��
	template<typename SearchBody>
	struct search_iterator
	{
	private:
		using iter = search_iterator;

		//�Y����
		int m_num;

		//SearchBody�ւ̃|�C���^
		std::shared_ptr<SearchBody> m_search_body;

	public:
		search_iterator(int n,std::shared_ptr<SearchBody>&& ptr)
			:m_num{ n }
			, m_search_body{std::move(ptr)}
		{
			static_assert(std::is_default_constructible_v<SearchBody>);
		}

		template<typename... Ts>
		static search_iterator begin(Ts... ts) {

			auto ptr = std::make_shared<SearchBody>();
			int num = ptr->init(std::forward<Ts>(ts)...);

			return { num,std::move(ptr) };
		}

		static const search_iterator& end() {
			static search_iterator<SearchBody> end{ -1 ,nullptr };
			return end;
		}

		//��r
		bool operator==(const iter& iter) {
			return m_num == iter.m_num;
		}
		bool operator!=(const iter& iter) {
			return !(*this == iter);
		}

		//�O�i
		iter& operator++() {
			if (m_search_body)
				m_num = m_search_body->increment(m_num);
			return *this;
		}

		//�Y�����̎擾�A�͈�for���p
		int operator*() {
			return m_num;
		}

		//�ÖٓI�ɓY�����ɕϊ�
		operator int() const {
			return m_num;
		}
	};




}