#include "DirectGraphics.h"

LPDIRECT3D9 g_Interface    = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;

bool InitDirectGraphics(HWND window_handle)
{
	/*
		Direct3DCreate9
			IDirect3D9����֐�

			�����F
				DirectX�̃o�[�W����(D3D_SDK_VERSION)

			�߂�l�F
				�����F
					IDirect3D9*
				���s�F
					nullptr	
	*/

	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		// ���������s
		return false;
	}

	// Device�̏���ݒ肷�邽�߂̍\����
	D3DPRESENT_PARAMETERS parameters;

	// �w�肳�ꂽ�f�[�^���T�C�Y������0�ŏ���������
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// �o�b�N�o�b�t�@�[�̐�
	parameters.BackBufferCount = 1;

	// �o�b�N�o�b�t�@�[�̃t�H�[�}�b�g
	// D3DFMT�QUNKNOWN => �m��܂���
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// �E�B���h�E���[�h�w��
	/*
		true  => �E�B���h�E���[�h

		false => �t���X�N���[�����[�h
					���t���X�N���[���͕ʓr�����w�肷��K�v����
	*/
	parameters.Windowed = true;

	/*
		�X���b�v�G�t�F�N�g => D3DSWAPEFFECT_DISCARD(�����ݒ�)

		�X���b�v�G�t�F�N�g�Ƃ́F
			�o�b�N�t�����g�ƃt�����g�o�b�t�@�ւ̐؂�ւ����@
	*/
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT result = g_Interface->CreateDevice(
		// �f�B�X�v���C�A�_�v�^�̎��
		D3DADAPTER_DEFAULT,
		// �f�o�C�X�̎��
		D3DDEVTYPE_HAL,
		// �f�o�C�X���g�p����E�B���h�E�̃n���h��
		window_handle,
		// �f�o�C�X�̐�����@
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Device�̕ۑ��ϐ��w��
		&g_Device
	);

	if (FAILED(result))
	{
		return false;
	}
	/*
		// ��������
		If(SUCCEEDE(result))
		{

		}
	*/

	return true;
}

void ReleaseDirectGraphics()
{
	if (g_Device != nullptr)
	{
		// Relase => ����֐�
		g_Device->Release();
		// �����̕s���A�N�Z�X�Ή�
		g_Device = nullptr;
	}

	if (g_Interface != nullptr) 
	{
		// Release => ����֐�
		g_Interface->Release();
		// �����̕s���A�N�Z�X�Ή�
		g_Interface = nullptr;
	}
}

void StartDrawing()
{
	g_Device->Clear(
		0,							  // 0�Œ�
		nullptr,					  // nullptr�Œ�
		D3DCLEAR_TARGET,			  // D3DCLEAR_TARGET�Œ�
		D3DCOLOR_XRGB(255, 255, 200), // �h��Ԃ��F
		1.0f,						  // 1.0f1�Œ�
		0							  // 0�Œ�
	);

	// DirectX�ŕ`����J�n����
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirectX�ŕ`����J�n����
	g_Device->BeginScene();

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�ɓ]������
	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawPorigon()
{
	CustomVertex vertices[] =
	{
		{300.0f,100.0f,0.0f,0xffffff},
		{500.0f,300.0f,0.0f,0xffffff},
		{50.0f ,300.0f,0.0f,0xffffff},
	};

	// DirectX�ɒ��_�\����������K�v������
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// �|���S���̍���
		1,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertex)	// 1���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleLise()
{
	CustomVertex vertices[] =
	{
		// 1��
		{0.0f  ,0.0f  ,0.0f,0xffffff},
		{100.0f,100.0f,0.0f,0xffffff},
		{0.0f  ,100.0f,0.0f,0xffffff},
		// 2��
		{100.0f,0.0f  ,0.0f,0xffffff},
		{100.0f,100.0f,0.0f,0xffffff},
		{0.0f  ,0.0f  ,0.0f,0xffffff},
	};
	// DirextX�ɒ��_�\����������K�v������
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// �|���S���̍���
		2,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertex)	// �P���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleStrip()
{
	CustomVertex vertices[] =
	{
		{300.0f,100.0f,0.0f,0xffffff},
		{300.0f,0.0f  ,0.0f,0xffffff},
		{400.0f,100.0f,0.0f,0xffffff},
		{400.0f,0.0f  ,0.0f,0xffffff},
		{500.0f,100.0f,0.0f,0xffffff},
		{500.0f,0.0f  ,0.0f,0xffffff},
	};
	// DirextX�ɒ��_�\����������K�v������
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// �|���S���̍���
		4,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertex)	// �P���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleFan()
{
	CustomVertex vertices[] =
	{
		{200.0f,200.0f,0.0f,0xffffff},
		{300.0f,200.0f,0.0f,0xffffff},
		{300.0f,300.0f,0.0f,0xffffff},
		{200.0f,300.0f,0.0f,0xffffff},
	};
	// DirextX�ɒ��_�\����������K�v������
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,		// �|���S���̍���
		2,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertex)	// �P���_�̃T�C�Y
	);
}
