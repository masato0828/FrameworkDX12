#pragma once
#include "../System/Window/Window.h"
#include <memory>

#define lpApplication Application::Instance()

class Application
{
public:
	
	/// <summary>
	/// アプリケーション実行
	/// </summary>
	void Excute();

private:

	std::unique_ptr<Window> window_;

	Application();
public:

	static Application & Instance()
	{
		static Application instance;
		return instance;
	}
};