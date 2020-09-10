#pragma once

namespace graph
{

	//有効グラフのとき
	struct directed {};
	
	//頂点に情報を持たせるとき
	template<typename T>
	struct vertexProperty {};

	//辺に情報を持たせるとき
	template<typename T>
	struct edgeProperty {};


	//グラフ本体
	//隣接リストで管理する予定
	template<typename... Args>
	class graph{};

}