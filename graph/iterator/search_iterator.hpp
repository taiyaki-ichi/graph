#pragma once
#include<stack>
#include<unordered_map>
#include"../adjacency_list.hpp"

namespace graph
{

	//���ƂƂȂ�C�e���[�^���ǂ�
	//SearchType��init��increment��v��
	template<typename SearchType>
	struct search_iterator
	{
	private:
		using iter = search_iterator;

		//�Y����
		int m_num;

	public:
		search_iterator(int n)
			:m_num{ n }
		{}

		template<typename... Ts>
		static search_iterator begin(Ts... ts) {
			int num = SearchType::init(std::forward<Ts>(ts)...);
			return { num };
		}

		static const search_iterator& end() {
			static search_iterator end{ -1 };
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
			m_num = SearchType::increment(m_num);
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