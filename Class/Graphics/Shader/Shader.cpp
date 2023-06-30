#include "Shader.h"

void Shader::Create(GraphicsDevice* pGraphicsDevice, const std::wstring& filePath, const RenderingSetting& renderingSetting, const std::vector<RangeType>& rangeTypes)
{
	pDevice_ = pGraphicsDevice;

	LoadShaderFile(filePath);

	upRootSignature_ = std::make_unique<RootSignature>();
	upRootSignature_->Create(pGraphicsDevice,rangeTypes);

	upPipeline_ = std::make_unique<Pipeline>();
	upPipeline_->SetRenderSettings(pGraphicsDevice,upRootSignature_.get(),renderingSetting.InputLayouts,
		renderingSetting.CullMode,renderingSetting.BlendMode,renderingSetting.PrimitiveTopologyType);
	upPipeline_->Create({pVSBlob,pHSBlob,pDSBlob,pGSBlob,pPSBlob},renderingSetting.Formats,
		renderingSetting.IsDepth,renderingSetting.IsDepthMask,renderingSetting.RTVCount,renderingSetting.IsWireFream);
}

void Shader::Begin(int w, int h)
{
	pDevice_->GetCmdList()->SetPipelineState(upPipeline_->GetPipeline());

	// ルートシグネチャをセット
	pDevice_->GetCmdList()->SetGraphicsRootSignature(upRootSignature_->GetRootSignature());

	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType =
		static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(upPipeline_->GetTopologyType());

	switch (topologyType)
	{
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT:
		pDevice_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE:
		pDevice_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE:
		pDevice_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//pDevice_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;
	case D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH:
		pDevice_->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		break;
	}

	D3D12_VIEWPORT viewport = {};
	D3D12_RECT rect = {};

	viewport.Width = static_cast<float>(w);
	viewport.Height = static_cast<float>(h);

	rect.right = w;
	rect.bottom = h;

	lpGraphicsDevice.GetCmdList()->RSSetViewports(1,&viewport);
	lpGraphicsDevice.GetCmdList()->RSSetScissorRects(1,&rect);
}

void Shader::DrawMesh(const Mesh& mesh)
{
	mesh.DrawInstanced();
}

void Shader::LoadShaderFile(const std::wstring& filePath)
{
	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* pErrorBlob = nullptr;

	std::wstring format = L".hlsl";
	std::wstring currentPath = L"Asset/Shader/";

	// 頂点シェーダーのコンパイル
	{
		std::wstring fullFilepath = currentPath + filePath + L"_VS" + format;

		auto hr = D3DCompileFromFile(fullFilepath.c_str(),nullptr,include,"main",
			"vs_5_0",flag,0,&pVSBlob,&pErrorBlob);

		if (FAILED(hr))
		{
			assert(0 && "頂点シェーダーのコンパイルに失敗しました");
			return;
		}
	}
	
	// ハルシェーダーのコンパイル
	{
		std::wstring fullFilepath = currentPath + filePath + L"_HS" + format;
		auto hr = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"hs_5_0", flag, 0, &pHSBlob, &pErrorBlob);
	}

	// ドメインシェーダーのコンパイル
	{
		std::wstring fullFilepath = currentPath + filePath + L"_DS" + format;

		auto hr = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"ds_5_0", flag, 0, &pDSBlob, &pErrorBlob);
	}

	// ジオメトリーシェーダーのコンパイル
	{
		std::wstring fullFilepath = currentPath + filePath + L"_GS" + format;

		auto hr = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"gs_5_0", flag, 0, &pGSBlob, &pErrorBlob);
	}

	// ピクセルシェーダーのコンパイル
	{
		std::wstring fullFilepath = currentPath + filePath + L"_PS" + format;

		auto hr = D3DCompileFromFile(fullFilepath.c_str(), nullptr, include, "main",
			"ps_5_0", flag, 0, &pPSBlob, &pErrorBlob);

		if (FAILED(hr))
		{
			assert(0 && "ピクセルシェーダーのコンパイルに失敗しました");
			return;
		}
	}
}
