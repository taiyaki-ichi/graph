#pragma once
#include<stack>
#include<unordered_map>
#include<memory>
#include<type_traits>
#include"../adjacency_list.hpp"

namespace graph
{

	//もととなるイテレータもどき
	//SerchBodyにデフォルトコンストラクタ,init,incrementを要求
	template<typename SearchBody>
	struct search_iterator
	{
	private:
		using iter = search_iterator;

		//添え字
		int m_num;

		//SearchBodyへのポインタ
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

		//比較
		bool operator==(const iter& iter) {
			return m_num == iter.m_num;
		}
		bool operator!=(const iter& iter) {
			return !(*this == iter);
		}

		//前進
		iter& operator++() {
			if (m_search_body)
				m_num = m_search_body->increment(m_num);
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