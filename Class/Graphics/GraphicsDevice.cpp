#include "GraphicsDevice.h"
#include "../Pch.h"

bool GraphicsDevice::Init(HWND hwnd, int width, int height)
{
	
	if (!CreateFactory())
	{
		assert(0 &&"ファクトリー作成失敗");
		return false;
	}

	if (!CreateDevice())
	{
		assert(0&& "D3D12デバイス作成失敗");
		return false;
	}

	if (!CreateCommandList())
	{
		assert(0&&"コマンドリストの作成失敗");
		return false;
	}

	if (!CreateSwapchain( hwnd,  width,  height))
	{
		assert(0 && "スワップチェインの作成失敗");
		return false;
	}


	pRTVHeap_ = std::make_unique<RTVHeap>();
	if (!pRTVHeap_->Create(pDevice_.Get(),100))
	{
		assert(0&&"RTVヒープの作成失敗");
		return false;
	}

	if (!CreateSwapchainRTV())
	{
		assert(0 && "スワップチェインRTVの作成失敗");
		return false;
	}

	if (!CreateFence())
	{
		assert(0&& "フェンスの作成失敗");
		return false;
	}
	
	return true;
}

void GraphicsDevice::ScreenFlip()
{
	// 1 リソースバリアのステートをレンダーターゲットに変更
	auto  bbIdx = pSwapChain_->GetCurrentBackBufferIndex();
	SetResourceBarrier(pSwapchainBuffers_[bbIdx].Get(),D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// 2 レンダーターゲットをセット
	auto rtvH = pRTVHeap_->GetRTVCPHandle(bbIdx);
	pCmdList_->OMSetRenderTargets(1,&rtvH,false,nullptr);

	// 3 セットしたレンダーターゲットの画面をクリア
	float crearColor[] = {1.0f,0.0f,1.0f,1.0f};// 紫色
	pCmdList_->ClearRenderTargetView(rtvH,crearColor,0,nullptr);

	// 4 リソースバリアのステートをプレゼントに戻す
	SetResourceBarrier(pSwapchainBuffers_[bbIdx].Get(),D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	// 5 コマンドリストを閉じて実行する
	pCmdList_->Close();
	ID3D12CommandList* cmdlists[] = { pCmdList_.Get() };
	pCmdQueue_->ExecuteCommandLists(1,cmdlists);

	// 6 コマンドリストの同期を待つ
	WaitForCommandQueue();

	// 7 コマンドアロケーターとコマンドリストを初期化
	pCmdAllocator_->Reset();							// コマンドアロケーターの初期化
	pCmdList_->Reset(pCmdAllocator_.Get(),nullptr);		// コマンドリストの初期化

	// 8 スワップチェインにプレゼント(送る)
	pSwapChain_->Present(1,0);
}

void GraphicsDevice::WaitForCommandQueue()
{
	pCmdQueue_->Signal(pFence_.Get(),++fenceVal_);

	if (pFence_->GetCompletedValue() != fenceVal_)
	{
		auto event = CreateEvent(nullptr,false,false,nullptr);
		if (!event)
		{
			assert(0&&"イベントエラー、アプリケーションを終了します");
		}
		pFence_->SetEventOnCompletion(fenceVal_,event);
		WaitForSingleObject(event,INFINITE);		// イベントが発生sるまで待ち続ける
		CloseHandle(event);							// イベントハンドルを閉じる
	}
}

bool GraphicsDevice::CreateFactory()
{
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG;
	auto result = CreateDXGIFactory2(flagsDXGI,IID_PPV_ARGS(pDxgiFactory_.GetAddressOf()));
	
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

	// 使用中PCにあるGPUドライバを検索して、あれば格納する
	for (UINT index = 0;1;++index)
	{
		pAdapters.push_back(nullptr);
		HRESULT ret = pDxgiFactory_->EnumAdapters(index,&pAdapters[index]);

		if (ret == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		descs.push_back({});
		pAdapters[index]->GetDesc(&descs[index]);
	}

	GPUTier gpuTier = GPUTier::Kind;

	// 優先度の高いGPUドライバーを使用する
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

	// Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;
	for (auto lv : levels)
	{
		if (D3D12CreateDevice(pSelectAdapter.Get(), lv, IID_PPV_ARGS(&pDevice_)) == S_OK)
		{
			featureLevel = lv;
			break;    // 生成可能なバージョンが見つからなかったら打ち切り
		}
	}

	return true;
}

bool GraphicsDevice::CreateCommandList()
{
	auto hr = pDevice_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&pCmdAllocator_));
	
	if (FAILED(hr))
	{
		return false;
	}

	hr = pDevice_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAllocator_.Get(),
		nullptr, IID_PPV_ARGS(&pCmdList_));

	if (FAILED(hr))
	{
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;// タイムアウトなし
	cmdQueueDesc.NodeMask = 0;// アダプターを1つしか使わない時は0でいい
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;// プライオリティは特に指定なし　
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;// コマンドリストと合わせる


	// キュー生成
	hr = pDevice_->CreateCommandQueue(&cmdQueueDesc,IID_PPV_ARGS(&pCmdQueue_));

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
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	auto result = pDxgiFactory_->CreateSwapChainForHwnd(pCmdQueue_.Get(),hwnd,&swapchainDesc,
		nullptr,nullptr,(IDXGISwapChain1**)pSwapChain_.ReleaseAndGetAddressOf());

	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool GraphicsDevice::CreateSwapchainRTV()
{
	for (int i = 0; i< (int)pSwapchainBuffers_.size();++i)
	{
		auto hr = pSwapChain_->GetBuffer(i, IID_PPV_ARGS(&pSwapchainBuffers_[i]));
		
		if (FAILED(hr))
		{
			return false;
		}
		
		pRTVHeap_->CreateRTV(pSwapchainBuffers_[i].Get());
	}

	return true;
}

bool GraphicsDevice::CreateFence()
{
	auto re = pDevice_->GetDeviceRemovedReason();

	auto result = pDevice_->CreateFence(fenceVal_,D3D12_FENCE_FLAG_NONE,IID_PPV_ARGS(&pFence_));
	
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void GraphicsDevice::SetResourceBarrier(ID3D12Resource* pResouce, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = pResouce;
	barrier.Transition.StateAfter = after;
	barrier.Transition.StateBefore = before;
	pCmdList_->ResourceBarrier(1,&barrier);
}
