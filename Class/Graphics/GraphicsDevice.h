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

	/// <summary>
	/// 画面(スワップチェイン)の切り替え
	/// </summary>
	void ScreenFlip();
	
	/// <summary>
	/// コマンドキューの同期待ち
	/// </summary>
	void WaitForCommandQueue();

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>デバイスのポインタ</returns>
	ID3D12Device8* GetDevice()const { return pDevice_.Get(); };

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns>コマンドリストのポインタ</returns>
	ID3D12GraphicsCommandList6* GetCmdList()const { return pCmdList_.Get(); };

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

	/// <summary>
	/// Fenceの作成
	/// </summary>
	/// <returns>成功したらtrue</returns>
	bool CreateFence();

	/// <summary>
	/// リソースとして引数に渡したバッファの扱いを変更する関数
	/// </summary>
	/// <param name="pResouce">指定バッファ</param>
	/// <param name="before">現在の状態</param>
	/// <param name="after">新しい状態</param>
	void SetResourceBarrier(ID3D12Resource* pResouce,D3D12_RESOURCE_STATES before,D3D12_RESOURCE_STATES after);

	/// <summary>
	/// デバッグレイヤーを適用
	/// </summary>
	void EnableDebugLayer();

	enum class GPUTier
	{
		NVIDIA,
		Amd,
		Intel,
		Arm,
		Qualcomm,
		Kind,
	};

	ComPtr<ID3D12Device8>					pDevice_ = nullptr;
	ComPtr<IDXGIFactory6>					pDxgiFactory_ = nullptr;

	ComPtr<ID3D12CommandAllocator>			pCmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList6>		pCmdList_ = nullptr;
	ComPtr<ID3D12CommandQueue>				pCmdQueue_ = nullptr;

	ComPtr<IDXGISwapChain4>					pSwapChain_ = nullptr;

	std::array<ComPtr<ID3D12Resource>, 2>	pSwapchainBuffers_;
	std::unique_ptr<RTVHeap>				pRTVHeap_ = nullptr;

	ComPtr<ID3D12Fence>						pFence_ = nullptr;
	UINT64									fenceVal_ = 0;

	GraphicsDevice() {};
	~GraphicsDevice() {};

public:
	static GraphicsDevice& Instance()
	{
		static GraphicsDevice instance;
		return instance;
	}
};

