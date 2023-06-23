#pragma once

#include <Windows.h>
#include <string>

class Window
{
public:

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	HWND GetWndHndle()const { return hWnd_; };

	/// <summary>
	/// ウィンドウの作成
	/// </summary>
	/// <param name="clientWidth">横幅</param>
	/// <param name="clientHeight">縦幅</param>
	/// <param name="titleName">タイトル名</param>
	/// <param name="windowClassName">クラス名</param>
	/// <returns>成功したらtrue</returns>
	bool Create(int clientWidth,int clientHeight,const std::wstring& titleName, const std::wstring& windowClassName);
	HWND hWnd_;// ウィンドウハンドル

	/// <summary>
	/// ウィンドウメッセージ処理
	/// </summary>
	/// <returns>終了メッセージが来たらfalse</returns>
	bool ProcessMesseage();

private:
};

