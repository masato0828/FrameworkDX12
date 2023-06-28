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

	/// <summary>
	/// ���(�X���b�v�`�F�C��)�̐؂�ւ�
	/// </summary>
	void ScreenFlip();
	
	/// <summary>
	/// �R�}���h�L���[�̓����҂�
	/// </summary>
	void WaitForCommandQueue();

	/// <summary>
	/// �f�o�C�X�̎擾
	/// </summary>
	/// <returns>�f�o�C�X�̃|�C���^</returns>
	ID3D12Device8* GetDevice()const { return pDevice_.Get(); };

	/// <summary>
	/// �R�}���h���X�g�̎擾
	/// </summary>
	/// <returns>�R�}���h���X�g�̃|�C���^</returns>
	ID3D12GraphicsCommandList6* GetCmdList()const { return pCmdList_.Get(); };

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

	/// <summary>
	/// Fence�̍쐬
	/// </summary>
	/// <returns>����������true</returns>
	bool CreateFence();

	/// <summary>
	/// ���\�[�X�Ƃ��Ĉ����ɓn�����o�b�t�@�̈�����ύX����֐�
	/// </summary>
	/// <param name="pResouce">�w��o�b�t�@</param>
	/// <param name="before">���݂̏��</param>
	/// <param name="after">�V�������</param>
	void SetResourceBarrier(ID3D12Resource* pResouce,D3D12_RESOURCE_STATES before,D3D12_RESOURCE_STATES after);

	/// <summary>
	/// �f�o�b�O���C���[��K�p
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

