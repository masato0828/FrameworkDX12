#include "CBVSRVUAVHeap.h"

int CBVSRVUAVHeap::CreateSRV(ID3D12Resource* pBuffer)
{
    if (useCount_.y < nextRegistNumber_)
    {
        assert(0 && "確保済みのヒープ領域を超えました。");
        return 0;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE handle = pHeap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += ((UINT64)useCount_.x+1)*incrementSize_+(UINT64)nextRegistNumber_ * incrementSize_;
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = pBuffer->GetDesc().Format;

    if (pBuffer->GetDesc().Format == DXGI_FORMAT_R32_TYPELESS)
    {
        srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    }

    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    pDevice_->GetDevice()->CreateShaderResourceView(pBuffer, &srvDesc, handle);
    return nextRegistNumber_++;
}

const D3D12_GPU_DESCRIPTOR_HANDLE CBVSRVUAVHeap::GetGPUHandle(int number)
{
    D3D12_GPU_DESCRIPTOR_HANDLE handle = pHeap_->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += (UINT64)incrementSize_ * ((UINT64)useCount_.x + 1);
    handle.ptr += (UINT64)incrementSize_ * number;
    return handle;
}

void CBVSRVUAVHeap::SetHeap()
{
    ID3D12DescriptorHeap* ppHeaps[] = {pHeap_.Get()};
    pDevice_->GetCmdList()->SetDescriptorHeaps(1,ppHeaps);
}
