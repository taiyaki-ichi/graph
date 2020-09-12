#pragma once
#include<functional>

namespace std
{
	template<>
	struct hash<std::pair<unsigned int, unsigned int>> {
		size_t operator()(const std::pair<unsigned int, unsigned int>& edge) const {

			size_t seed = 0;
			auto xHash = hash<unsigned int>()(edge.first);
			auto yHash = hash<unsigned int>()(edge.second);

			seed ^= xHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= yHash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

