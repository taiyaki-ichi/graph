# graph

グラフとそのアルゴリズム
templateの練習したい

# 使い方

グラフの作製

```C++

adjacency_list g{};
g.add_vertex(0);
g.add_vertex(1);
g.add_vertex(2);
g.add_edge({0,1});
g.add_edge({0,2});
g.add_edge({1,2});

```
辺の情報からグラフを作製

```C++

std::vector<std::pair<unsigned int, unsigned int>> edge{
	{0,1},
	{0,2},
	{1,2},
};
adjacency_list g{edge};

```

グラフの種類

```C++

//有向グラフ
adjacency_list<directed> g1{};

//無向グラフに辺の情報を持たせる
struct my_edge_property { int weight; };
adjacency_list<set_edge_property<my_edge_property>> g2{};

```

探索

```C++

//グラフgの頂点0からの幅優先探索
for(auto v : BFS{g,0})
	std::cout<<v;

//グラフgの頂点0からの深さ先行順、後行順の深さ優先探索
for(auto v : DFS_preorder{g,0})
	std::cout<<v;
for(auto v : DFS_postorder{g,0})
	std::cout<<v;

```

ダイクストラの最短経路

```C++

struct my_edge_property { int weight; };
adjacency_list<set_edge_property<my_edge_property>> g{};

...

std::unordered_map<unsigned int, unsigned int> parent{};
std::unordered_map<unsigned int, std::optional<int>> distance{};

//グラフgの頂点0からのそれぞれの頂点への最短経路
dijkstra_shortest_paths(g, 0, &my_edge_property::weight, &parent, &distance);

//boost.graphを参考にした

```

連結判定

```C++

//連結している頂点同士のグループに分ける
std::vector<std::vector<unsigned int>> result = connected_components(g);

//gが有効グラフの場合、強連結のグループを求めることができる
std::vector<std::vector<unsigned int>> result = connected_components<graph::strong_connection_tag>(g);

```
