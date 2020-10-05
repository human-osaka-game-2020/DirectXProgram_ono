#include<Windows.h>

// �E�B���h�E�v���V�[�W���[�쐬
LRESULT CALLBACK WindowsProcedure(
	HWND   window_handle,
	UINT   message_id,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (message_id)
	{
		// �E�B���h�E�����A�C�R�����N���b�N���ꂽ
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
		// ���b�Z�[�W�������Ή����Ȃ��Ƃ��Ɏ��s����֐�
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}
	// ���b�Z�[�W�̑Ή��������̂�0��Ԃ�
	return 0;
}

int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     IpCmdLine,
	INT       nCmdChow)
{
	int width  = 400;
	int height = 400;

	// �\���̂̓o�^
	HWND hWnd;
	WNDCLASSEX window_class =
	{
		sizeof(WNDCLASSEX),            // �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,       // �N���X�̃X�^�C��(CS_HREDRAW �� CS_VREDRAW�͉��Əc�̕ύX���ݒ�)
		WindowsProcedure,              // �E�B���h�E�v���V�[�W��
		0,                             // �⏕������
		0,                             // �⏕������
		hInstance,                     // ���̃v���O�����̃C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),// �A�C�R���摜
		LoadCursor(NULL,IDC_ARROW),    // �J�[�\���摜
		NULL,                          // �w�i�F
		NULL,                          // ���j���[��
		TEXT("CreateWindow"),          // �N���X��
		NULL						   // �������A�C�R��
	};

	// �����������E�B���h�E����o�^����
	/*
	    RegisterClass
		    �E�B���h�E����o�^����֐�

		�߂�l
		    ��O => �o�^����
			�O   => �o�^���s
	*/
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	// �E�B���h�E�쐬
	hWnd = CreateWindow(
		// �o�^���Ă���E�B���h�E�N���X�\���̖̂��O
		TEXT("CreateWindow"),
		// �E�B���h�E��
		TEXT("�E�B���h�E�����T���v��"),
		// �E�C���h�E�X�^�C��
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// �\���ʒu
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// �T�C�Y
		width,
		height,
		// �e�n���h��
		NULL,
		// ���j���[�n���h��
		NULL,
		// �C���X�^���X�n���h��
		hInstance,
		// WM_CREATE���b�Z�[�W��lparam�ɓn�������l
		NULL);

	ShowWindow(hWnd, SW_SHOW);

	if (hWnd == NULL)
	{
		return 0;
	}

	while (true)
	{
		MSG message;

		/*
		    PeekMassage�L���Ɋւ�炸���̏��������s����

			�߂�l�F
			    true  => ���b�Z�[�W�L��
				false => ���b�Z�[�W����

			�������F
				��M�������b�Z�[�W���

			�������F
				�E�B���h�E�n���h��
				(nullptr�̏ꍇ���̓f�t�H���g�̃n���h�����p�ӂ����)

			��O�`�܈����F
				�Œ�l�Ŗ��Ȃ�
		*/

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			// ��M�����f�[�^��|�󂷂�
			TranslateMessage(&message);

			// �E�B���h�E�Y�v���V�[�W���[
			DispatchMessage(&message);
		}
		else
		{
			// �Q�[���Ɋ֘A���鏈��
		}
	}

	return 0;
}
