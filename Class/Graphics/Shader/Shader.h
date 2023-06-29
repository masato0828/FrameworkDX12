#pragma once

#include "Pipeline/Pipeline.h"
#include "RootSignature/RootSignature.h"

struct RenderingSetting
{
	std::vector<InputLayout> InputLayouts;
	std::vector<DXGI_FORMAT> Formats;
	CullMode CullMode = CullMode::Back;
	BlendMode BlendMode = BlendMode::Alpha;
	PrimitiveTopologyType PrimitiveTopologyType = PrimitiveTopologyType::Triangle;
	bool IsDepth = true;
	bool IsDepthMask = true;
	int RTVCount = 1;
	bool IsWireFream = false;
};

class Shader
{
public:
	/// <summary>
	/// �쐬
	/// </summary>
	/// <param name="pGraphicsDevice">�O���t�B�b�N�X�f�o�C�X�̃|�C���^</param>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="renderingSetting">�`��ݒ�</param>
	/// <param name="rangeTypes">�����W�^�C�v</param>
	void Create(GraphicsDevice* pGraphicsDevice,const std::wstring& filePath,
		const RenderingSetting& renderingSetting,const std::vector<RangeType>& rangeTypes);

	/// <summary>
	/// �`��J�n
	/// </summary>
	/// <param name="w">����</param>
	/// <param name="h">�c��</param>
	void Begin(int w,int h);

	/// <summary>
	/// ���b�V���̕`��
	/// </summary>
	/// <param name="mesh">���b�V��</param>
	void DrawMesh(const Mesh& mesh);
private:

	/// <summary>
	/// �V�F�[�_�[�t�@�C���̃��[�h
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadShaderFile(const std::wstring& filePath);

	GraphicsDevice* pDevice_ = nullptr;

	std::unique_ptr<Pipeline> upPipeline_ = nullptr;
	std::unique_ptr<RootSignature> upRootSignature_ = nullptr;

	ID3DBlob* pVSBlob = nullptr;// ���_�V�F�[�_�[
	ID3DBlob* pHSBlob = nullptr;// �n���V�F�[�_�[
	ID3DBlob* pDSBlob = nullptr;// �h���C���V�F�[�_�[
	ID3DBlob* pGSBlob = nullptr;// �W�I���g���V�F�[�_�[
	ID3DBlob* pPSBlob = nullptr;// �s�N�Z���V�F�[�_�[
};

