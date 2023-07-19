#pragma once
#include "../Heap.h"

class RTVHeap:public Heap<int>
{
public:
	RTVHeap() {};
	~RTVHeap() {};

	/// <summary>
	/// RTVの作成
	/// </summary>
	/// <param name="pBuffer">バッファーのポインタ</param>
	/// <returns>ヒープに紐づけられた登録番号</returns>
	int CreateRTV(ID3D12Resource* pBuffer);
};

