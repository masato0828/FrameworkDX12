#include "../../Pch.h"
#include "RTVHeap.h"

bool RTVHeap::Create(ID3D12Device* pDevice, int useCount)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;// レンダーターゲットとして扱う
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = useCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特に指定なし
	
	auto hr = pDevice->CreateDescriptorHeap(&heapDesc,IID_PPV_ARGS(&pHeap_));
	
	if (FAILED(hr))
	{
		return false;
	}

	useCount_ = useCount;
	incrementSize_ = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	pDevice_ = pDevice;
	
	return true;
}

int RTVHeap::CreateRTV(ID3D12Resource* pBuffer)
{
	if (useCount_ < nextRegistNumber_)
	{
		assert(0&& "確保済みのヒープ領域を超えました");
		return 0;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE handle = pHeap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT)nextRegistNumber_ * incrementSize_;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	pDevice_->CreateRenderTargetView(pBuffer,&rtvDesc,handle);
	return nextRegistNumber_++;
}

D3D12_CPU_DESCRIPTOR_HANDLE RTVHeap::GetRTVCPHandle(int number)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = pHeap_->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)incrementSize_ * number;
	return handle;
}
