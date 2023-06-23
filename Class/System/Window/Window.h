#pragma once

#include <Windows.h>
#include <string>

class Window
{
public:

	/// <summary>
	/// �E�B���h�E�n���h���̎擾
	/// </summary>
	/// <returns>�E�B���h�E�n���h��</returns>
	HWND GetWndHndle()const { return hWnd_; };

	/// <summary>
	/// �E�B���h�E�̍쐬
	/// </summary>
	/// <param name="clientWidth">����</param>
	/// <param name="clientHeight">�c��</param>
	/// <param name="titleName">�^�C�g����</param>
	/// <param name="windowClassName">�N���X��</param>
	/// <returns>����������true</returns>
	bool Create(int clientWidth,int clientHeight,const std::wstring& titleName, const std::wstring& windowClassName);
	HWND hWnd_;// �E�B���h�E�n���h��

	/// <summary>
	/// �E�B���h�E���b�Z�[�W����
	/// </summary>
	/// <returns>�I�����b�Z�[�W��������false</returns>
	bool ProcessMesseage();

private:
};

