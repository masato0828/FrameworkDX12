#pragma once

#define lpGraphicsDevice  GraphicsDevice::Instance()

class GraphicsDevice
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����������true</returns>
	bool Init(HWND hwnd,int width,int height);

private:

	/// <summary>
	/// �t�@�N�g���[�̍쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateFactory();

	/// <summary>
	/// �f�o�C�X�̍쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateDevice();

	/// <summary>
	/// �R�}���h���X�g�̍쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateCommandList();

	/// <summary>
	/// �X���b�v�`�F�C���̍쐬
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <returns>����������true</returns>
	bool CreateSwapchain(HWND hwnd,int width,int height);

	/// <summary>
	/// �X���b�v�`�F�C��RTV�̍쐬
	/// </summary>
	/// <returns>����������true</returns>
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

