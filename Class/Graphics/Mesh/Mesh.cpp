#include "Mesh.h"

void Mesh::Create(GraphicsDevice* pGraphicsDevice)
{
    pDevice_ = pGraphicsDevice;

    // 頂点の座標
    vertices_[0] = {-1.f,-1.f};
    vertices_[2] = {1.f,1.f};
    vertices_[1] = {0.f,1.f};

    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeof(Math::Vector3) * vertices_.size();
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.Format = DXGI_FORMAT_UNKNOWN;
    resDesc.SampleDesc.Count = 1;
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    auto hr = pDevice_->GetDevice()->CreateCommittedResource(&heapProp,
        D3D12_HEAP_FLAG_NONE,&resDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(&pVBuffer_));

    if (FAILED(hr))
    {
        assert(0&& "頂点バッファ作成失敗");
    }

    vbView_.BufferLocation = pVBuffer_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = (UINT)resDesc.Width;
    vbView_.StrideInBytes = sizeof(Math::Vector3);

    // 頂点バッファに情報を書き込む
    Math::Vector3* vbMap = nullptr;
    {
        pVBuffer_->Map(0, nullptr, (void**)&vbMap);
        std::copy(std::begin(vertices_),std::end(vertices_),vbMap);
        pVBuffer_->Unmap(0,nullptr);
    }
}

void Mesh::DrawInstanced() const
{
    pDevice_->GetCmdList()->IASetVertexBuffers(0,1,&vbView_);
    pDevice_->GetCmdList()->DrawInstanced(3,1,0,0);
}
