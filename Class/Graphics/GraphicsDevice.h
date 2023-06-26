#pragma once

#define lpGraphicsDevice  GraphicsDevice::Instance()

class GraphicsDevice
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool Init(HWND hwnd,int width,int height);

private:

	/// <summary>
	/// ファクトリーの作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateFactory();

	/// <summary>
	/// デバイスの作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateDevice();

	/// <summary>
	/// コマンドリストの作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateCommandList();

	/// <summary>
	/// スワップチェインの作成
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns>成功したらtrue</returns>
	bool CreateSwapchain(HWND hwnd,int width,int height);

	/// <summary>
	/// スワップチェインRTVの作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateSwapchainRTV();

	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	ComPtr<ID3D12Device8>					pDevice = nullptr;
	ComPtr<IDXGIFactory6>					pDxgiFactory = nullptr;

	ComPtr<ID3D12CommandAllocator>			pCmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList6>		pCmdList = nullptr;
	ComPtr<ID3D12CommandQueue>				pCmdQueue = nullptr;

	ComPtr<IDXGISwapChain4>					pSwapChain = nullptr;

	std::array<ComPtr<ID3D12Resource>, 2>	pSwapchainBuffers_;
	std::unique_ptr<RTVHeap>				pRTVHeap_ = nullptr;

	GraphicsDevice() {};
	~GraphicsDevice() {};

public:
	static GraphicsDevice& Instance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};

