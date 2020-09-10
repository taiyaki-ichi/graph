#pragma once
#include<type_traits>
#include"find_if.hpp"

namespace graph
{
	namespace utility
	{
		//List‚©‚çPredicate‚ÌğŒ‚ğ–‚½‚·‚à‚Ì‚ª‚È‚¯‚ê‚ÎNullTag‚ğ•Ô‚·
		//‚ ‚ê‚Î‚»‚ÌŒ^‚ğ•Ô‚·
		template<typename NullTag,template<typename>typename Predicate,typename... List>
		struct get_optional_arg
		{
		private:
			using result = typename find_if<Predicate, List...>::type;

		public:
			using type = typename std::conditional<
				!std::is_same_v<not_found, result>,
				result,
				NullTag
			>::type;
		};
		


	}
}