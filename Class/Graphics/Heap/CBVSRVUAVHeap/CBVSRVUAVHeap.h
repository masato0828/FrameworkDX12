#pragma once
#include "../../../Pch.h"

class CBVSRVUAVHeap :public Heap<Math::Vector3>
{
public:
	CBVSRVUAVHeap() {};
	~CBVSRVUAVHeap() {};

	/// <summary>
	/// SRVの作成
	/// </summary>
	/// <param name="pBuffer">バッファーのポインタ</param>
	/// <returns>セーブの紐づけられた登録番号</returns>
	int CreateSRV(ID3D12Resource* pBuffer);

	/// <summary>
	/// SRVのGPU側のアドレス
	/// </summary>
	/// <param name="number">登録番号</param>
	/// <returns>SRVのGPUアドレス</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int number)override;

	/// <summary>
	/// ヒープをセットする関数
	/// </summary>
	void SetHeap();
};

