#pragma once
#include "../../../Pch.h"

enum class CullMode
{
	None = D3D12_CULL_MODE_NONE,
	Front = D3D12_CULL_MODE_NONE,
	Back = D3D12_CULL_MODE_BACK,
};

enum class BlendMode
{
	Add,
	Alpha,
};

enum class InputLayout
{
	POSITION,
	TEXCOORD,
	NORMAL,
	TANGENT,
	COLOR,
	SKININDEX,
	SKINWEIGHT,
};

enum class PrimitiveTopologyType
{
	Undefined = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED,
	Point = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT,
	Line = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
	Triangle = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
	Patch = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH
};

class RootSignature;

class Pipeline
{
public:
	/// <summary>
	/// �`��ݒ�̃Z�b�g
	/// </summary>
	/// <param name="pGraphicsDevice">�O���t�B�b�N�X�f�o�C�X�̃|�C���^</param>
	/// <param name="pRootSignature">���[�g�V�O�l�`���̃|�C���^</param>
	/// <param name="inputLayout">���_���C�A�E�g���</param>
	/// <param name="cullMode">�J�����O���[�h</param>
	/// <param name="blendMode">�u�����h���[�h</param>
	/// <param name="topologyType">�v���~�e�B�u�g�|���W�[</param>
	void SetRenderSettings(GraphicsDevice * pGraphicsDevice,RootSignature* pRootSignature,
		const std::vector<InputLayout>& inputLayout,CullMode cullMode,BlendMode blendMode,
		PrimitiveTopologyType topologyType);

	/// <summary>
	/// �쐬
	/// </summary>
	/// <param name="pBlobs">�V�F�[�_�[�f�[�^���X�g</param>
	/// <param name="formats">�t�H�[�}�b�g���X�g</param>
	/// <param name="isDepth">�[�x�e�X�g</param>
	/// <param name="isDepthMask">�[�x��������</param>
	/// <param name="rtvCount">RTV��</param>
	/// <param name="bWireFrame">���C���[�t���[��</param>
	void Create(std::vector<ID3DBlob*> pBlobs,const std::vector<DXGI_FORMAT> formats,bool bDepth, bool bDepthMask,
		int rtvCount,bool bWireFrame);

	/// <summary>
	/// �p�C�v���C���̎擾
	/// </summary>
	/// <returns>�p�C�v���C��</returns>
	ID3D12PipelineState* GetPipeline() { return pPipelineState.Get(); };

	/// <summary>
	/// �g�|���W�[�^�C�v�̎擾
	/// </summary>
	/// <returns>�g�|���W�[�^�C�v</returns>
	PrimitiveTopologyType GetTopologyType() { return topologyType_; };
private:

	void SetInputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElements,
		const std::vector<InputLayout>& inputLayouts);


	/// <summary>
	/// �u�����h���[�h�̃Z�b�g
	/// </summary>
	/// <param name="blendDesc">�����_�[�^�[�Q�b�g�u�����h���</param>
	/// <param name="blendMode">�u�����h���[�h</param>
	void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc,BlendMode blendMode);

	GraphicsDevice* pDevice_ = nullptr;
	RootSignature* pRootSignature_ = nullptr;

	std::vector<InputLayout> inputLayouts_;
	CullMode  cullmode_;
	BlendMode blendMode_;
	PrimitiveTopologyType topologyType_;

	ComPtr<ID3D12PipelineState> pPipelineState = nullptr;
};

