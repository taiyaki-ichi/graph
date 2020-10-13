# graph

グラフとそのアルゴリズム

# 使い方

無向グラフの作製

```C++

graph::adjacency_list g{};
g.add_vertex(0);
g.add_vertex(1);
g.add_vertex(2);
g.add_edge({0,1});
g.add_edge({0,2});
g.add_edge({1,2});

```
辺の情報からグラフの作製

```C++

std::vector<std::pair<unsigned int, unsigned int>> edge{
		{0,1},
    {0,2},
		{1,2},
};

graph::adjacency_list g{edge};

```

