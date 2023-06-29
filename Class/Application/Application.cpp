#include "Application.h"
#include "../Pch.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// ���������[�N��m�点��

	CoInitializeEx(nullptr,COINIT_MULTITHREADED);// COM������

	lpApplication.Excute();

	CoUninitialize();// CO���
	
	return 0;
}

void Application::Excute()
{
	static const int width = 1280;
	static const int height = 720;

	if (!window_->Create(width,height,L"FreamWorkDX12", L"Window"))
	{
		assert(0 && "window�쐬���s");
		return;
	}

	if (!lpGraphicsDevice.Init(window_->GetWndHndle(),width,height))
	{
		assert(0&& "�O���t�B�b�N�X�f�o�C�X�̏��������s");
		return;
	}

	Mesh mesh;
	mesh.Create(&lpGraphicsDevice);

	RenderingSetting renderSetting = {};
	renderSetting.InputLayouts = {InputLayout::POSITION};
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
