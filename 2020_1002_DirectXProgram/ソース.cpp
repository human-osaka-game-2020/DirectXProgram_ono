#include<Windows.h>

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
	// ���b�Z�[�W�������Ή����Ȃ��Ƃ��Ɏ��s����֐�
	//�@�����ɂ̓E�B���h�E�v���V�[�W���[�œn����Ă�����������̂܂ܕԂ�
	return DefWindowProc(window_handle, message_id, wparam, lparam);
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

	// �\���̂̓o�^
	WNDCLASSA window_class =
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
		"CreateWindow",                // �N���X��
	};

	// �����������E�B���h�E����o�^����
	/*
	    RegisterClass
		    �E�B���h�E����o�^����֐�

		�߂�l
		    ��O => �o�^����
			�O   => �o�^���s
	*/
	if (RegisterClassA(&window_class) == 0)
	{
		return 0;
	}

	// �E�B���h�E�쐬
	HWND window_handle = CreateWindowA(
		// �o�^���Ă���E�B���h�E�N���X�\���̖̂��O
		"CreateWindow",
		// �E�B���h�E��
		"�E�B���h�E�����T���v��",
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

	// �E�B���h�E�\��
	ShowWindow(window_handle, SW_SHOW);

	if (window_handle == NULL)
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
			// ��M�����f�[�^��|�󂷂�
			TranslateMessage(&message);

			// �E�B���h�E�Y�v���V�[�W���[
			DispatchMessage(&message);
		}
		else
		{
			// �Q�[���Ɋ֘A���鏈��
				// �Q�[�������Ɖ�ʏ��������s����
		}
	}

	return 0;
}
