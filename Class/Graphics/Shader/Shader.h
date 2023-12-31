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
	/// 作成
	/// </summary>
	/// <param name="pGraphicsDevice">グラフィックスデバイスのポインタ</param>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="renderingSetting">描画設定</param>
	/// <param name="rangeTypes">レンジタイプ</param>
	void Create(GraphicsDevice* pGraphicsDevice,const std::wstring& filePath,
		const RenderingSetting& renderingSetting,const std::vector<RangeType>& rangeTypes);

	/// <summary>
	/// 描画開始
	/// </summary>
	/// <param name="w">横幅</param>
	/// <param name="h">縦幅</param>
	void Begin(int w,int h);

	/// <summary>
	/// メッシュの描画
	/// </summary>
	/// <param name="mesh">メッシュ</param>
	void DrawMesh(const Mesh& mesh);
private:

	/// <summary>
	/// シェーダーファイルのロード
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadShaderFile(const std::wstring& filePath);

	GraphicsDevice* pDevice_ = nullptr;

	std::unique_ptr<Pipeline> upPipeline_ = nullptr;
	std::unique_ptr<RootSignature> upRootSignature_ = nullptr;

	ID3DBlob* pVSBlob = nullptr;// 頂点シェーダー
	ID3DBlob* pHSBlob = nullptr;// ハルシェーダー
	ID3DBlob* pDSBlob = nullptr;// ドメインシェーダー
	ID3DBlob* pGSBlob = nullptr;// ジオメトリシェーダー
	ID3DBlob* pPSBlob = nullptr;// ピクセルシェーダー
};

