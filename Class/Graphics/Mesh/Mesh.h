#pragma once
#include "../../Pch.h"

class Mesh
{
public:

	/// <summary>
	/// �쐬
	/// </summary>
	/// <param name="pGraphicsDevice">�O���t�B�b�N�X�h���C�u�̃|�C���^</param>
	void Create(GraphicsDevice* pGraphicsDevice);

	/// <summary>
	/// �C���X�^���X�`��
	/// </summary>
	void DrawInstanced() const;
private:
	GraphicsDevice* pDevice_ = nullptr;
	ComPtr<ID3D12Resource>		pVBuffer_ = nullptr;
	ComPtr<ID3D12Resource>		pIBuffer_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW	vbView_;
	D3D12_INDEX_BUFFER_VIEW		ibView_;

	std::array<Math::Vector3, 3> vertices_;

};

