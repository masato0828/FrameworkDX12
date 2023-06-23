#include "Window.h"

LRESULT WindowProcedure(HWND hWnd,UINT messeage, WPARAM wParam,LPARAM lParam)
{
	// メッセージ毎に処理を選択
	switch (messeage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);   // OSに対して終了を伝える
		break;
	default:
		// メッセージのデフォルト処理
		return DefWindowProc(hWnd,messeage,wParam,lParam);
		break;
	}
	return 0;
}

bool Window::Create(int clientWidth, int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName)
{
	// インスタンスハンドル取得
	HINSTANCE hInst = GetModuleHandle(0);

	//=====================================
	// メインウィンドウ作成
	//=====================================

	// ウィンドウクラスの定義
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);				// 構造体のサイズ
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;  // ウィンドウ関数
	wc.lpszClassName = windowClassName.c_str();	// ウィンドウクラス名
	wc.hInstance = hInst;						// インスタンスハンドル

	// ウィンドウクラスの登録
	if (!RegisterClassEx(&wc))
	{
		return true;
	}

	// ウィンドウの作成
	hWnd_ = CreateWindow(
		windowClassName.c_str(),			// ウィンドウクラス名
		titleName.c_str(),					// ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW-WS_THICKFRAME,	// ウィンドウタイプを標準タイプに
		0,									// ウィンドウの位置（X座標）
		0,									// ウィンドウの位置（Y座標）
		clientWidth,						// ウィンドウの幅
		clientHeight,						// ウィンドウの高さ
		nullptr,							// 親ウィンドウのハンドル
		nullptr,							// メニューのハンドル
		hInst,								// インスタンスハンドル
		this
	);

	if (hWnd_ == nullptr)
	{
		return false;
	}

	// ウィンドウの表示
	ShowWindow(hWnd_,SW_SHOW);

	// ウィンドウの更新
	UpdateWindow(hWnd_);


	return true;
}

bool Window::ProcessMesseage()
{
	// メッセージ取得
	MSG msg;
	while (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
	{
		// 終了メッセージがきた
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		// メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}
