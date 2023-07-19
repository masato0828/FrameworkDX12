#pragma once
#include "../Heap.h"

class RTVHeap:public Heap<int>
{
public:
	RTVHeap() {};
	~RTVHeap() {};

	/// <summary>
	/// RTV�̍쐬
	/// </summary>
	/// <param name="pBuffer">�o�b�t�@�[�̃|�C���^</param>
	/// <returns>�q�[�v�ɕR�Â���ꂽ�o�^�ԍ�</returns>
	int CreateRTV(ID3D12Resource* pBuffer);
};

