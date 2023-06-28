#pragma once
#include "../../../Pch.h"

enum class RangeType
{
	CBV,
	SRV,
	UAV,
};

enum class TextureAddressMode
{
	Wrap,
	Clamp,
};

enum class D3D12Filter
{
	Point,
	Linear,
};

class RootSignature
{
public:
	/// <summary>
	/// 作成
	/// </summary>
	/// <param name="pGraphicsDevice">グラフィックスドライブのポインタ</param>
	/// <param name="rangeTypes">レンジタイプリスト</param>
	void Create(GraphicsDevice* pGraphicsDevice,const std::vector<RangeType>& rangeTypes);

	/// <summary>
	/// ルートシグネチャの取得
	/// </summary>
	/// <returns>ルートシグネチャンのポインタ</returns>
	ID3D12RootSignature* GetRootSignature() { return pRootSignature.Get(); };
private:

	/// <summary>
	/// レンジの作成
	/// </summary>
	/// <param name="pRange">レンジのポインタ</param>
	/// <param name="type">レンジタイプ</param>
	/// <param name="count">登録数</param>
	void CreateRange(D3D12_DESCRIPTOR_RANGE& pRange,RangeType type,int count);

	/// <summary>
	/// サンプラーの作成
	/// </summary>
	/// <param name="pSamplerDesc">サンプラーデスクのポインタ</param>
	/// <param name="mode">アドレスモード</param>
	/// <param name="filter">フィルターモード</param>
	/// <param name="count">使用サンプラー数</param>
	void CreateStaticSampler(D3D12_STATIC_SAMPLER_DESC& pSamplerDesc,TextureAddressMode mode,
		D3D12Filter filter,int count);

	GraphicsDevice* pDevice_ = nullptr;
	ComPtr<ID3DBlob> pRootBlob = nullptr;
	ComPtr<ID3D12RootSignature> pRootSignature = nullptr;
};

