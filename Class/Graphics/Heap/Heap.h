#pragma once

enum class HeapType
{
	CBVSRVUAV = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	RTV = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	DSV = D3D12_DESCRIPTOR_HEAP_TYPE_DSV
};

template<typename T>
class Heap
{
public:
	Heap() {};
	~Heap() {};

	/// <summary>
	/// ヒープ作成
	/// </summary>
	/// <param name="pDevice">グラフィックっデバイスのポインタ</param>
	/// <param name="heapType">ヒープのタイプ</param>
	/// <param name="useCount">使用個数</param>
	/// <returns>成功したらtrue</returns>
	bool Create(GraphicsDevice* pDevice, HeapType heapType, T useCount)
	{

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

		heapDesc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(heapType);
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = ComputUseCount(useCount);
		heapDesc.Flags = heapType == HeapType::CBVSRVUAV ?
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		auto hr = pDevice->GetDevice()->CreateDescriptorHeap(&heapDesc,IID_PPV_ARGS(&pHeap_));

		if (FAILED(hr))
		{
			return false;
		}

		useCount_ = useCount;
		incrementSize_ = pDevice->GetDevice()->GetDescriptorHandleIncrementSize(
			static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(heapType));
		pDevice_ = pDevice;

		return true;
	}

	/// <summary>
	/// CPU側アドレスを返す
	/// </summary>
	/// <param name="number">登録番号</param>
	/// <returns>CPU側アドレス</returns>
	virtual const D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(int number)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handle = pHeap_->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += (UINT64)incrementSize_ * number;
		return handle;
	}

	/// <summary>
	/// GPU側アドレスを返す
	/// </summary>
	/// <param name="number">登録番号</param>
	/// <returns>GPU側アドレス</returns>
	virtual const D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int number)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE handle = pHeap_->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += (UINT64)incrementSize_ * number;
		return handle;
	}

protected:
	GraphicsDevice* pDevice_ = nullptr;
	ComPtr<ID3D12DescriptorHeap>pHeap_ = nullptr;
	T useCount_;
	int incrementSize_ = 0;
	int nextRegistNumber_ = 0;

private:
	UINT ComputUseCount(UINT useCount)
	{
		return useCount;
	}

	UINT ComputUseCount(Math::Vector3 useCount)
	{
		return (UINT)(useCount.x + useCount.y + useCount.z);
	}
};

