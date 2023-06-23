#include "Window.h"

LRESULT WindowProcedure(HWND hWnd,UINT messeage, WPARAM wParam,LPARAM lParam)
{
	// ���b�Z�[�W���ɏ�����I��
	switch (messeage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);   // OS�ɑ΂��ďI����`����
		break;
	default:
		// ���b�Z�[�W�̃f�t�H���g����
		return DefWindowProc(hWnd,messeage,wParam,lParam);
		break;
	}
	return 0;
}

bool Window::Create(int clientWidth, int clientHeight, const std::wstring& titleName, const std::wstring& windowClassName)
{
	// �C���X�^���X�n���h���擾
	HINSTANCE hInst = GetModuleHandle(0);

	//=====================================
	// ���C���E�B���h�E�쐬
	//=====================================

	// �E�B���h�E�N���X�̒�`
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);				// �\���̂̃T�C�Y
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;  // �E�B���h�E�֐�
	wc.lpszClassName = windowClassName.c_str();	// �E�B���h�E�N���X��
	wc.hInstance = hInst;						// �C���X�^���X�n���h��

	// �E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&wc))
	{
		return true;
	}

	// �E�B���h�E�̍쐬
	hWnd_ = CreateWindow(
		windowClassName.c_str(),			// �E�B���h�E�N���X��
		titleName.c_str(),					// �E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW-WS_THICKFRAME,	// �E�B���h�E�^�C�v��W���^�C�v��
		0,									// �E�B���h�E�̈ʒu�iX���W�j
		0,									// �E�B���h�E�̈ʒu�iY���W�j
		clientWidth,						// �E�B���h�E�̕�
		clientHeight,						// �E�B���h�E�̍���
		nullptr,							// �e�E�B���h�E�̃n���h��
		nullptr,							// ���j���[�̃n���h��
		hInst,								// �C���X�^���X�n���h��
		this
	);

	if (hWnd_ == nullptr)
	{
		return false;
	}

	// �E�B���h�E�̕\��
	ShowWindow(hWnd_,SW_SHOW);

	// �E�B���h�E�̍X�V
	UpdateWindow(hWnd_);


	return true;
}

bool Window::ProcessMesseage()
{
	// ���b�Z�[�W�擾
	MSG msg;
	while (PeekMessage(&msg,nullptr,0,0,PM_REMOVE))
	{
		// �I�����b�Z�[�W������
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		// ���b�Z�[�W����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}
