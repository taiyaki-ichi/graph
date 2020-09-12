#pragma once
#include<stack>
#include<unordered_map>
#include"graph.hpp"

namespace graph
{

	//幅優先、先行順用のイテレータ
	template<typename SearchType>
	struct search_iterator
	{
	private:
		using iter = search_iterator;

		//添え字
		int m_num;

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

		//比較
		bool operator==(const iter& iter) {
			return m_num == iter.m_num;
		}
		bool operator!=(const iter& iter) {
			return !(*this == iter);
		}

		//前進
		iter& operator++() {
			m_num = SearchType::increment(m_num);
			return *this;
		}

		//添え字の取得、範囲for文用
		int operator*() {
			return m_num;
		}

		//暗黙的に添え字に変換
		operator int() const {
			return m_num;
		}
	};

	


}