#pragma once
#include "../../Pch.h"

struct Vertex
{
	Vertex(Math::Vector3 position, Math::Vector2 uv) :Position(position), UV(uv) {};

	Math::Vector3 Position;
	Math::Vector2 UV;
};

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

	//std::array<Math::Vector3, 3> vertices_;
	std::vector<Vertex> vertices_;

	std::vector<UINT> indices_;
};

