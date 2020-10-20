#include<Windows.h>
#include"DirectGraphics.h"

// prama comment�ɂ��lib�t�@�C���̒ǉ�
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// �E�B���h�E�v���V�[�W���[�쐬
/*
	�߂�l�F
		���s����

	�����F
		HWND
			���b�Z�[�W�̑ΏۂƂȂ��Ă���E�B���h�E�̃n���h��

			HWND
				�E�B���h�E�̃n���h����ۑ�����^

		UNIT
			���b�Z�[�W���
				�N���b�N���ꂽ���̏�񂪕ۑ�����Ă���

		WPARAM
			���b�Z�[�W�t����񂻂̂P

		LPARAM
			���b�Z�[�W�t����񂻂̂Q

		�E�t�����ɂ̓��b�Z�[�W���̕⑫���ۑ�����Ă���
			��F�L�[�̏ڍ׏��(�啶���A�������Ȃ�)
*/
LRESULT CALLBACK WindowsProcedure(
	HWND   window_handle,
	UINT   message_id,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	// ���b�Z�[�W�������Ή����Ȃ��Ƃ��Ɏ��s����֐�
	//�@�����ɂ̓E�B���h�E�v���V�[�W���[�œn����Ă�����������̂܂ܕԂ�
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}
	// ���b�Z�[�W�̑Ή�������
	return 0;
}

/*
	WinMain
		Windows�A�v���ɂ����ẴG���g���|�C���g
		�߂�l�F
			�A�v���̌���
		�����F
			HINSTANCE hInstance :: �d�v
				�A�v���̃C���X�^���X�n���h��
				H => Handle�̗�

		HINSTANCE hPrevInstance :: �ȉ����܂�K�v�Ȃ�
			�Â��d�l�̎c��Ȃ̂Ŏg��Ȃ�
			���Ƃ��Ẳ��l�͂Ȃ�

		LPSTR IpCmdLine,
			�R�}���h���C���̃p�����[�^�[���X�g

		INT nCmdShow
			�R�}���h���C���̈����̐�
*/
int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     IpCmdLine,
	INT       nCmdChow)
{
	int width  = 400;
	int height = 400;

	// �E�B���h�E�̐����̓��C�����[�v���n�܂�O�ɍs��
	// �\���̂̓o�^
	WNDCLASS window_class =
	{
		CS_HREDRAW | CS_VREDRAW,       // �N���X�̃X�^�C��(CS_HREDRAW �� CS_VREDRAW�͉��Əc�̕ύX���ݒ�)
		WindowsProcedure,              // �E�B���h�E�v���V�[�W��
		0,                             // �⏕������
		0,                             // �⏕������
		hInstance,                     // ���̃v���O�����̃C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),// �A�C�R���摜
		LoadCursor(NULL,IDC_ARROW),    // �J�[�\���摜
		NULL,                          // �w�i�F
		NULL,                          // ���j���[��
		TEXT("CreateWindow"),                // �N���X��
	};

	// �����������E�B���h�E����o�^����
	/*
	    RegisterClass
		    �E�B���h�E����o�^����֐�

		�߂�l
		    ��O => �o�^����
			�O   => �o�^���s
	*/
	if (RegisterClass(&window_class) == 0)
	{
		return 0;
	}

	// �E�B���h�E�쐬
	HWND window_handle = CreateWindow(
		// �o�^���Ă���E�B���h�E�N���X�\���̖̂��O
		TEXT("CreateWindow"),
		// �E�B���h�E��
		TEXT("��o����p"),
		// �E�C���h�E�X�^�C��
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// �\���ʒu
		/*
			�E�B���h�E��\������ʒu���w�肷��
			(CW_USEDEFAULT => OS�C��)
		*/
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

	

	if (window_handle == NULL)
	{
		return 0;
	}

	// ���T�C�Y
	RECT window_rect;
	RECT client_rect;

	// �E�B���h�E�T�C�Y�擾
	GetWindowRect(window_handle, &window_rect);
	
	// �N���C�A���g�̈�̃T�C�Y�擾
	GetClientRect(window_handle, &client_rect);

	// �t���[���T�C�Y�Z�o
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom);

	// ���T�C�Y�p�T�C�Y�̎Z�o
	int resize_width   = frame_size_x + width;
	int resize_height  = frame_size_y + height;

	// �\���ʒu�̍X�V
	SetWindowPos(
		window_handle,
		NULL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// ���T�C�Y�̉���
		resize_width,
		// ���T�C�Y�̏c��
		resize_height,
		// �I�v�V�����̎w��
		SWP_NOMOVE);

	// �E�B���h�E�\��
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false)
	{
		return 0;
	}

	while (true)
	{
		// �t���[���Ǘ�
		/*
			�t���[���Ǘ��̓��[�v�����̎����ɂȂ�悤�ɊǗ�����
			601FPS�ŃQ�[�����i�s����Ȃ�
			�P��̃��[�v�͖�0.016�b�Ŏ��s����

			��DirectX��60FPS�Ńt���[���Ǘ��������ōs���悤��
			�����DirectX�ɔC����
		*/

		// ���b�Z�[�W�Ή�
		/*
			Windows(OS)���瑗���Ă��郁�b�Z�[�W��
			�Ή����s���K�v������

			���b�Z�[�W�̗�
				�E�B���h�E���N���b�N����
				�E�B���h�E���ړ�������
				�E�B���h�E���ĕ`�悵��
		
		*/
		MSG message;
		/*
			Getmessage
				Windows���烁�b�Z�[�W���ʒm�����܂�
				���̏����Ɉڍs���Ȃ�

				scanf�ɋ߂�
		*/
		/*
		if(GetMessage(&message,nullptr,0,100))
		{
		}
		*/
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
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ��M�����f�[�^��|�󂷂�
				TranslateMessage(&message);

				// �E�B���h�E�Y�v���V�[�W���[
				DispatchMessage(&message);
			};
			
		}
		else
		{
			// �Q�[���Ɋ֘A���鏈��
				// �Q�[�������Ɖ�ʏ��������s����

			StartDrawing();

//			DrawPorigon();

			DrawPorigonWithTriangleLise();

			DrawPorigonWithTriangleStrip();

			DrawPorigonWithTriangleFan();

			FinishDrawing();
		}
	}

	// ���
	ReleaseDirectGraphics();

	return 0;
}
