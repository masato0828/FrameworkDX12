#include "GraphicsDevice.h"
#include "../Pch.h"

bool GraphicsDevice::Init(HWND hwnd, int width, int height)
{
	if (!CreateFactory())
	{
		assert(0 &&"�t�@�N�g���[�쐬���s");
		return false;
	}

	if (!CreateDevice())
	{
		assert(0&& "D3D12�f�o�C�X�쐬���s");
		return false;
	}

	if (!CreateCommandList())
	{
		assert(0&&"�R�}���h���X�g�̍쐬���s");
		return false;
	}

	if (!CreateSwapchain( hwnd,  width,  height))
	{
		assert(0 && "�X���b�v�`�F�C���̍쐬���s");
		return false;
	}
	
	return true;
}

bool GraphicsDevice::CreateFactory()
{
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
	auto result = CreateDXGIFactory2(flagsDXGI,IID_PPV_ARGS(pDxgiFactory.GetAddressOf()));
	
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool GraphicsDevice::CreateDevice()
{
	ComPtr<IDXGIAdapter> pSelectAdapter = nullptr;
	std::vector<ComPtr<IDXGIAdapter>> pAdapters;
	std::vector<DXGI_ADAPTER_DESC>descs;

	// �g�p��PC�ɂ���GPU�h���C�o���������āA����Ίi�[����
	for (UINT index = 0;1;++index)
	{
		pAdapters.push_back(nullptr);
		HRESULT ret = pDxgiFactory->EnumAdapters(index,&pAdapters[index]);

		if (ret == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		descs.push_back({});
		pAdapters[index]->GetDesc(&descs[index]);
	}

	GPUTier gpuTier = GPUTier::Kind;

	// �D��x�̍���GPU�h���C�o�[���g�p����
	for (int i = 0;i<descs.size();++i)
	{
		if (std::wstring(descs[i].Description).find(L"NVIDIA")!= std::wstring::npos)
		{
			pSelectAdapter = pAdapters[i];
			break;
		}
		else if (std::wstring(descs[i].Description).find(L"Amd") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Amd)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Amd;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Intel") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Intel)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Intel;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Arm") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Arm)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Arm;
			}
		}
		else if (std::wstring(descs[i].Description).find(L"Qualcomm") != std::wstring::npos)
		{
			if (gpuTier > GPUTier::Qualcomm)
			{
				pSelectAdapter = pAdapters[i];
				gpuTier = GPUTier::Qualcomm;
			}
		}

		
	}

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// Direct3D�f�o�C�X�̏�����
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(pSelectAdapter.Get(), lv, IID_PPV_ARGS(&pDevice)) == S_OK)
		{
			featureLevel = lv;
			break;    // �����\�ȃo�[�W������������Ȃ�������ł��؂�
		}
	}

	return true;
}

bool GraphicsDevice::CreateCommandList()
{
	auto hr = pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&pCmdAllocator));
	
	if (FAILED(hr))
	{
		return false;
	}

	hr = pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator.Get(),
		nullptr, IID_PPV_ARGS(&pCmdList));

	if (FAILED(hr))
	{
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;// �^�C���A�E�g�Ȃ�
	cmdQueueDesc.NodeMask = 0;// �A�_�v�^�[��1�����g��Ȃ�����0�ł���
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;// �v���C�I���e�B�͓��Ɏw��Ȃ��@
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;// �R�}���h���X�g�ƍ��킹��


	// �L���[����
	hr = pDevice->CreateCommandQueue(&cmdQueueDesc,IID_PPV_ARGS(&pCmdQueue));

	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool GraphicsDevice::CreateSwapchain(HWND hwnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = width;
	swapchainDesc.Height = height;
	swapchainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	auto result = pDxgiFactory->CreateSwapChainForHwnd(pCmdQueue.Get(),hwnd,&swapchainDesc,
		nullptr,nullptr,(IDXGISwapChain1**)pSwapChain.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}
	return true;
}
