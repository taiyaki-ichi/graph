# graph

グラフとそのアルゴリズム

# 使い方

グラフの作製

```C++

graph::adjacency_list<graph::directed> g{};
g.add_vertex(0);
g.add_vertex(1);
g.add_vertex(2);
g.add_edge({0,1});
g.add_edge({0,2});

```
