#include "Mesh.h"

void Mesh::Create(GraphicsDevice* pGraphicsDevice)
{
    pDevice_ = pGraphicsDevice;

    // ���_�̍��W
    //vertices_[0] = {-0.5f,-0.5f};// ��
    //vertices_[1] = {0.f,0.5f};//��
    //vertices_[2] = {0.5f,-0.5f};// �E
    vertices_.emplace_back(Math::Vector3( - 0.75f, -0.75f, 0.0f), Math::Vector2(0.0f, 1.0f));
    vertices_.emplace_back(Math::Vector3( - 0.75f, 0.75f, 0.0f), Math::Vector2(0.0f, 1.0f));
    vertices_.emplace_back(Math::Vector3(0.75f,-0.75f,0.0f), Math::Vector2(1.0f, 1.0f));
    vertices_.emplace_back(Math::Vector3(0.75f,0.75f,0.0f), Math::Vector2(1.0f, 0.0f));


    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = sizeof(Vertex) * vertices_.size();
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
        assert(0&& "���_�o�b�t�@�쐬���s");
    }

    vbView_.BufferLocation = pVBuffer_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = (UINT)resDesc.Width;
    vbView_.StrideInBytes = sizeof(Vertex);

    // �C���f�b�N�X�o�b�t�@�̍쐬
    {
        // �C���f�b�N�X�f�[�^
        indices_.emplace_back(0);
        indices_.emplace_back(1);
        indices_.emplace_back(2);
        indices_.emplace_back(2);
        indices_.emplace_back(1);
        indices_.emplace_back(3);

        resDesc.Width = sizeof(UINT)* indices_.size();

        // �C���f�b�N�X�o�b�t�@�쐬
        hr = pDevice_->GetDevice()->CreateCommittedResource(&heapProp,
            D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
            IID_PPV_ARGS(&pIBuffer_));

        if (FAILED(hr))
        {
            assert(0&&"�C���f�b�N�X�o�b�t�@�[�쐬���s");
        }

        // �C���f�b�N�X�o�b�t�@�̃f�[�^���r���[�ɏ�������
        ibView_.BufferLocation = pIBuffer_->GetGPUVirtualAddress();
        ibView_.SizeInBytes = (UINT)resDesc.Width;
        ibView_.Format = DXGI_FORMAT_R32_UINT;
    }

    // ���_�o�b�t�@�ɏ�����������
    Vertex* vbMap = nullptr;
    {
        pVBuffer_->Map(0, nullptr, (void**)&vbMap);
        std::copy(std::begin(vertices_),std::end(vertices_),vbMap);
        pVBuffer_->Unmap(0,nullptr);
    }

    // �C���f�b�N�X�o�b�t�@�ɏ�����������
    UINT* ibMap = nullptr;
    {
        pIBuffer_->Map(0,nullptr,(void**)&ibMap);
        std::copy(std::begin(indices_),std::end(indices_),ibMap);
        pIBuffer_->Unmap(0,nullptr);
    }
}

void Mesh::DrawInstanced() const
{
    pDevice_->GetCmdList()->IASetVertexBuffers(0,1,&vbView_);
    //pDevice_->GetCmdList()->DrawInstanced(3,1,0,0);
    //pDevice_->GetCmdList()->DrawInstanced(4,1,0,0);

    pDevice_->GetCmdList()->IASetIndexBuffer(&ibView_);

    pDevice_->GetCmdList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
