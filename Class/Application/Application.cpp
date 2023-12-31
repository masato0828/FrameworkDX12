#include "Application.h"
#include "../Pch.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// メモリリークを知らせる

	CoInitializeEx(nullptr,COINIT_MULTITHREADED);// COM初期化

	lpApplication.Excute();

	CoUninitialize();// CO解放
	
	return 0;
}

void Application::Excute()
{
	static const int width = 1280;
	static const int height = 720;

	if (!window_->Create(width,height,L"FreamWorkDX12", L"Window"))
	{
		assert(0 && "window作成失敗");
		return;
	}

	if (!lpGraphicsDevice.Init(window_->GetWndHndle(),width,height))
	{
		assert(0&& "グラフィックスデバイスの初期化失敗");
		return;
	}

	Mesh mesh;
	mesh.Create(&lpGraphicsDevice);

	RenderingSetting renderSetting = {};
	renderSetting.InputLayouts = {InputLayout::POSITION,InputLayout::TEXCOORD};
	renderSetting.Formats = {DXGI_FORMAT_R8G8B8A8_UNORM};
	renderSetting.IsDepth = false;
	renderSetting.IsDepthMask = false;

	Shader shader;
	shader.Create(&lpGraphicsDevice, L"SimpleShader", renderSetting, {});

	while (true)
	{
		if (!window_->ProcessMesseage())
		{
			break;
		}

		lpGraphicsDevice.Prepaer();

		shader.Begin(width,height);

		shader.DrawMesh(mesh);

		lpGraphicsDevice.ScreenFlip();
	}
}

Application::Application()
{
	window_ = std::make_unique<Window>();
}
