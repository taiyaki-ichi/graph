#pragma once
#include<stack>
#include<unordered_map>
#include"graph.hpp"

namespace graph
{

	//DζAζspΜCe[^
	template<typename SearchType>
	struct search_iterator
	{
	private:
		using iter = search_iterator;

		//Y¦
		int m_num;

		//std::stack<unsigned int> m_stack;
		//std::unordered_map<unsigned int, bool> m_searched_vertex_list;

	public:
		search_iterator(int n)
			:m_num{n}
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

		//δr
		bool operator==(const iter& iter) {
			return m_num == iter.m_num;
		}
		bool operator!=(const iter& iter) {
			return !(*this == iter);
		}

		//Oi
		iter& operator++() {
			m_num = SearchType::increment(m_num);
			return *this;
		}

		//Y¦ΜζΎAΝΝforΆp
		int operator*() {
			return m_num;
		}

		//ΓΩIΙY¦ΙΟ·
		operator int() const {
			return m_num;
		}
	};

	


}