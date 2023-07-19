#include "RTVHeap.h"

int RTVHeap::CreateRTV(ID3D12Resource* pBuffer)
{
    if (useCount_ < nextRegistNumber_)
    {
        assert(0 && "確保済みのヒープ領域を超えました。");
        return 0;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE handle = pHeap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += (UINT64)nextRegistNumber_ * incrementSize_;
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    pDevice_->GetDevice()->CreateRenderTargetView(pBuffer,&rtvDesc,handle);
    return nextRegistNumber_++;
}
