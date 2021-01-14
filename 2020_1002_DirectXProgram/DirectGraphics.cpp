#include "DirectGraphics.h"
#include "DirectInput.h"
#include <stdlib.h>

// XFile�f�[�^�ۑ��p�̍\����
/*
	�Ȃ��A�w�b�_�t�@�C���ł͂Ȃ��A�\�[�X�t�@�C���ɍ\���̂�錾�������H
		->XFile�\���̂��g�p����t�@�C����DirectGraphics.cpp����������
*/
struct XFile
{
	DWORD MaterialNum;				// !< ���b�V����
	LPD3DXMESH Meshes;				// !< ���b�V��
	LPD3DXBUFFER Materials;			// !< ���b�V���}�e���A��
	LPDIRECT3DTEXTURE9* Textures;	// !< �e�N�X�`���f�[�^
};
XFile g_XFile;

LPDIRECT3D9 g_Interface    = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;
// �e�N�X�`���f�[�^
LPDIRECT3DTEXTURE9 g_Textures[TextureID::TexIDMax];

LPCWSTR g_TextureNameList[] =
{
	TEXT("res/gazou.png"),
	TEXT("res/gazou.png"),
	TEXT("res/gazou.png"),
};

bool InitDirectGraphics(HWND window_handle)
{
	/*
		Direct3DCreate9
			IDirect3D9�����֐�

			�����F
				DirectX�̃o�[�W����(D3D_SDK_VERSION�Œ�)

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
		�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̐؂�ւ����@

		�X���b�v�G�t�F�N�g => D3DSWAPEFFECT_DISCARD(�����ݒ�)

		�X���b�v�G�t�F�N�g�Ƃ́F
			�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�ւ̐؂�ւ����@
	*/
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	/*
		Z�o�b�t�@�@���g���ꍇPresentParameter��
		�g�����Ƃ�ʒm����K�v�L
	*/
	// Z�o�b�t�@ON
	// EnableAutoDepthStencil => �f�v�X�A�X�e���V���o�b�t�@�̎d�l�t���O
	parameters.EnableAutoDepthStencil = TRUE;
	// AutoDepthStencilFormat => �f�v�X�A�X�e���V���o�b�t�@�̃t�H�[�}�b�g
	// D3DFMT_D16 => �f�v�X�o�b�t�@��1�f�[�^��16�r�b�g�ō쐬����
	parameters.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT result = g_Interface->CreateDevice(
		// �f�B�X�v���C�A�_�v�^�̎��
		D3DADAPTER_DEFAULT,
		// �f�o�C�X�̎��
		D3DDEVTYPE_HAL,
		// �f�o�C�X���g�p����E�B���h�E�̃n���h��
		window_handle,
		// �f�o�C�X�̐���̕��@
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Device�̕ۑ��ϐ��w��
		&g_Device
	);

	/*
		FAILED => HRESULT�̌��ʂ𔻒肵�Ă����}�N��
					true�Ȃ玸�s
					false�Ȃ琬��
	*/
	if (FAILED(result))
	{
		return false;
	}

	// �v���W�F�N�V�����̐ݒ�
	/*
		�v���W�F�N�V�����̐ݒ���Q�[�����ɕύX���Ȃ��ꍇ��x�����ݒ�������OK
	*/
	SetUpProjection();

	/*
		// ��������
		If(SUCCEEDED(result))
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
	// �u�����h�̐ݒ�(�ȉ��̐ݒ�ł͒��_�J���[�ƃe�N�X�`���̃��u�����h�������Ă���)
	/*
		SetTextureStageState
			���F�X�e�[�W��ID
			���F�ݒ�̎��
			��O�F�ݒ�̏ڍ�
	*/

	/*
		SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE)
			���F0
			���FD3DTSS_ALPHAOP  <= ���l�̍����Ɋւ���ݒ������
			��O�FD3DTOP_MODULATE <= �����͏�Z���g�p����
	*/
	g_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			���F0
			���FD3DTSS_COLORARG1  <=  ��Z�̒l�̐ݒ�
			��O�FD3DTOP_MODULATE   <=  �e�N�X�`���J���[���g�p����
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	/*
		SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			���F0
			���FD3DTSS_COLORARG2 <=  ��Z�̒l�̐ݒ�
			��O�FD3DTOP_DIFFUSE   <=  Stage�ɑ����Ă��������J���[
	*/
	g_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// ���l�̔��f��L���ɂ���
	/*
		SetRenderState <= �`��̐ݒ��ύX����
			���F�ύX�������ݒ�̎��
			���F�ݒ�̏ڍ�
	*/
	/*
		SetRenderState <= �`��̐ݒ��ύX����
			���FD3DRS_ALPHABLENDENABLE <= ���u�����h�̐ݒ��ύX����
			���Ftrue					 <= �L�� (false������)
	*/
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	/*
		SetRenderState <= �`��̐ݒ��ύX����
			���FD3DRS_SRCBLEND	<= �o�b�t�@�ɑ��M�����F�̐ݒ�
			���FD3DBLEND_SRCALPHA	<= �A���t�@�����̂܂܎g��
	*/
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	/*
		SetRenderState <= �`��̐ݒ��ύX����
			���FD3DRS_DESTBLEND		<= �o�b�t�@�̐F�̐ݒ�
			���FD3DBLEND_INVSRCALPHA	<= SRC(�o�b�t�@�ɑ��M�����F)�̃A���t�@�𗘗p����(1 - srcalpha)
	*/
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�e�B���O�𖳌��ɂ���
	g_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�o�b�t�@�̎d�l��ON�ɂ���
	g_Device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �o�b�N�o�b�t�@���N���A����
	g_Device->Clear(
		0,							  // 0�Œ�
		nullptr,					  // nullptr�Œ�
		// D3DCLEAR_ZBUFFER => Z�o�b�t�@���N���A�Ώۂɉ�����
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,			  // D3DCLEAR_TARGET�Œ�
		D3DCOLOR_XRGB(255, 255, 200), // �h��Ԃ��F
		1.0f,						  // 1.0f�Œ�
		0							  // 0�Œ�
	);

	// DirectX�ŕ`����J�n����
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirectX�̕`����I������
	g_Device->EndScene();

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�ɓ]������
	g_Device->Present(nullptr, nullptr, nullptr, nullptr);
}

void DrawPorigon()
{
	CustomVertexTex vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f, 0xffffff},
		{ 500.0f, 300.0f, 0.0f, 0xffffff},
		{ 50.0f , 300.0f, 0.0f, 0xffffff},
	};

	// DirectX�ɒ��_�\����������K�v������
	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// �|���S���̍���
		1,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertex)	// 1���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleList()
{
	CustomVertex vertices[] =
	{
		// 1��
		{   0.0f,   0.0f, 0.0f, 1.0f, 0xffffff},
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0xffffff},
		{   0.0f, 100.0f, 0.0f, 1.0f, 0xffffff},
		// 2��
		{ 100.0f,   0.0f, 0.0f, 1.0f, 0xffffff},
		{ 100.0f, 100.0f, 0.0f, 1.0f, 0xffffff},
		{   0.0f,   0.0f, 0.0f, 1.0f, 0xffffff},
	};
	// DirextX�ɒ��_�\����������K�v������
	g_Device->SetFVF(FVF_2D_VERTEX);


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
		{ 300.0f, 100.0f, 0.0f,1.0f, 0xffdc143c},
		{ 300.0f,   0.0f, 0.0f,1.0f, 0xff000088},
		{ 400.0f, 100.0f, 0.0f,1.0f, 0xffffffff},
		{ 400.0f,   0.0f, 0.0f,1.0f, 0xffffffff},
		{ 500.0f, 100.0f, 0.0f,1.0f, 0xffffffff},
		{ 500.0f,   0.0f, 0.0f,1.0f, 0xffffffff},
	};
	// DirextX�ɒ��_�\����������K�v������
	g_Device->SetFVF(FVF_2D_VERTEX);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// �|���S���̍���
		4,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertex)	// �P���_�̃T�C�Y
	);
}

void DrawPorigonWithTriangleFan(TextureID tex_id)
{
	if (g_Textures[tex_id] == nullptr)
	{
		return;
	}

	CustomVertexTex vertices[] =
	{
		//							 ARGB
		{ 200.0f, 200.0f, 0.0f,1.0f, 0xffffffff,0.0f,0.0f },
		{ 300.0f, 200.0f, 0.0f,1.0f, 0xffffffff,1.0f,0.0f },
		{ 300.0f, 300.0f, 0.0f,1.0f, 0xffffffff,1.0f,1.0f },
		{ 200.0f, 300.0f, 0.0f,1.0f, 0xffffffff,0.0f,1.0f },
	};

	// DirextX�ɒ��_�\����������K�v������
	g_Device->SetFVF(FVF_2D_VERTEX_TEXTURE);

	// DirectX(Computer)�ɍ���̃|���S���`��Ŏg�p����e�N�X�`����������
	g_Device->SetTexture(0, g_Textures[tex_id]);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,		// �|���S���̍���
		2,						// �|���S���̐�
		vertices,				// ���_���X�g
		sizeof(CustomVertexTex)	// �P���_�̃T�C�Y
	);
}

void DrawXFile(float x_, float y_, float z_, float angle_)
{
	float ScaleX = 1.0f;
	float ScaleY = 1.0f;
	float ScaleZ = 1.0f;

	float RadianX = D3DXToRadian(0.0f);
	float RadianY = D3DXToRadian(angle_);
	float RadianZ = D3DXToRadian(0.0f);

	D3DXMATRIX Matrix;
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX TransMatrix;
	D3DXMATRIX RotateX_Matrix;
	D3DXMATRIX RotateY_Matrix;
	D3DXMATRIX RotateZ_Matrix;

	// D3DXMatrixIdentity => �P�ʍs�񉻂���
	D3DXMatrixIdentity(&Matrix);

	// �g�k
	D3DXMatrixScaling(&ScaleMatrix, ScaleX, ScaleY, ScaleZ);

	// ��]
	D3DXMatrixRotationX(&RotateX_Matrix, RadianX);
	D3DXMatrixRotationY(&RotateY_Matrix, RadianY);
	D3DXMatrixRotationZ(&RotateZ_Matrix, RadianZ);

	// �ړ�
	D3DXMatrixTranslation(&TransMatrix, x_, y_, z_);

	Matrix *= ScaleMatrix;

	Matrix *= RotateX_Matrix;
	Matrix *= RotateY_Matrix;
	Matrix *= RotateZ_Matrix;

	Matrix *= TransMatrix;


	g_Device->SetTransform(D3DTS_WORLD, &Matrix);

	// �`�揈��
	/*
		XFile�̕`��̓}�e���A���P�ʂōs��
		�e�}�e���A���P�ʂŃ}�e���A���̐ݒ�Ƃ��̃}�e���A���Ŏg���Ă���e�N�X�`���̐ݒ���s��
		���̐ݒ肪���������烁�b�V����`�悷��
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();
	for (DWORD i = 0; i < g_XFile.MaterialNum; i++)
	{
		// �}�e���A���̐ݒ�
		g_Device->SetMaterial(&materials[i].MatD3D);
		// �e�N�X�`���̐ݒ�
		g_Device->SetTexture(0, g_XFile.Textures[i]);
		// ���b�V����`��
		g_XFile.Meshes->DrawSubset(i);
	}
}

void SetUpView()
{
	D3DXMATRIX Matrix; // �J��������ۑ����邽�߂̍s��
	D3DXVECTOR3 CameraPos = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 EyePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 UpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	/*
		D3DXMatrixLookAtLH
			�J�����������s����쐬����
	*/
	// L => Left H => Hand LH => ����n�ŃJ�������W��ݒ肷��
	D3DXMatrixLookAtLH(
		&Matrix,		// ���ʕۑ��p�s��̎w��
		&CameraPos,	// �J�������W
		&EyePos,		// �����_���W
		&UpVec			// ���[���h���W��ɂ�����J�����̏�����̃x�N�g��
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_VIEW, &Matrix);
}

void SetUpProjection()
{
	D3DXMATRIX matrix; // �v���W�F�N�V��������ۑ����邽�߂̍s��
	float aspect = 640.0f / 480.0f;	// �A�X�y�N�g��(�E�B���h�E�T�C�Y�� / �c)

	// Perspective => �������e
	D3DXMatrixPerspectiveFovLH(
		&matrix,				// ���ʕۑ��p�̍s��̎w��
		// D3DXToRadian => �x���@�����W�A���ɕϊ�
		D3DXToRadian(60.0f),	// ��p(���W�A���w��)
		aspect,					// �A�X�y�N�g��
		0.1f,					// �������Near�l
		1000.0f					// �������Far�l
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_PROJECTION, &matrix);
}

void Draw3DPorigon()
{
	// �|���S���`��
	CustomVertex3D vertices[]
	{
		{ -1.0f, -1.0f, 0.0f, 0xffff0000 },
		{ 0.0f, 1.0f, 0.0f, 0xffff0000 },
		{ 1.0f, -1.0f, 0.0f, 0xffff0000 }
	};

	g_Device->SetFVF(FVF_3D_VERTEX);

	g_Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 1, vertices, sizeof(CustomVertex3D));
}

void ReleaseTexture()
{
	for (int i = 0; i < TextureID::TexIDMax; i++)
	{
		if (g_Textures[i] != nullptr)
		{
			g_Textures[i]->Release();
			g_Textures[i] = nullptr;
		}
	}
}

bool LoadTexture(TextureID tex_id)
{
	HRESULT hr = D3DXCreateTextureFromFile(
		g_Device,					// DirectX9�̃f�o�C�X
		g_TextureNameList[tex_id],	// �t�@�C����
		&g_Textures[tex_id]			// �ǂݍ��܂ꂽ�e�N�X�`����񂪕ۑ������ϐ�
	);

	// FAILED => HRESULT�^�̕ϐ����w�肵�āA���̒l�����s���Ă�����true
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool LoadXFile(LPCWSTR file_name)
{
	HRESULT hr = D3DXLoadMeshFromX(
		file_name,				// ���t�@�C����
		D3DXMESH_IB_SYSTEMMEM,	// ���b�V���쐬�̃I�v�V����
		g_Device,				// DirectGraphics�̃f�o�C�X(D3DXMESH_SYSTEMMEN�Œ�)
		nullptr,				// �����nullptr�ŗǂ�
		&g_XFile.Materials,		// ���}�e���A���ۑ��p
		nullptr,				// �����nullptr�ŗǂ�
		&g_XFile.MaterialNum,	// ���}�e���A���̐��ۑ��p
		&g_XFile.Meshes			// �����b�V���f�[�^�ۑ��p
	);

	// ���b�V��::���f���̌`�󂪂킩����
	// �}�e���A��::���f���̍ގ����킩����
	//		�}�e���A���Ŏg���Ă���e�N�X�`���̓ǂݍ���
	//		�}�e���A���ɂ���e�N�X�`����� => �t�@�C���̖��O(�p�X�t)

	if (FAILED(hr))
	{
		return false;
	}

	// �}�e���A���̐������e�N�X�`����ۑ��ł���悤�ɂ���
	g_XFile.Textures = new LPDIRECT3DTEXTURE9[g_XFile.MaterialNum];
	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	/*
		Materials�̌^���m�F����Ƃ킩�邪�AMaterials��LPD3DBUFFER�^�ɂȂ��Ă���
		Buffer�Ƃ����ėp�I�Ƀf�[�^��ۑ�����^�Ƀ}�e���A�����ۑ�����Ă��邽�߁A
		Material�{���̌^�ɖ߂��K�v������

		GetBufferPointer���g�p�����Buffer�̐擪�|�C���^���擾�ł���
	*/
	D3DXMATERIAL* materials = (D3DXMATERIAL*)g_XFile.Materials->GetBufferPointer();

	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		// pTexture
		if (materials[i].pTextureFilename != nullptr)
		{
			D3DXCreateTextureFromFileA(
				g_Device,
				(LPCSTR)materials[i].pTextureFilename,
				&g_XFile.Textures[i]);
		}
		else
		{
			g_XFile.Textures[i] = nullptr;
		}
	}

	return true;
}

void UpdateTPSCamera(float x_, float y_, float z_, float angle_)
{
	// �^�[�Q�b�g�̌����x�N�g�������߂�
	float VecX = sinf(D3DXToRadian(angle_));
	float VecZ = cosf(D3DXToRadian(angle_));

	// �x�N�g���̔��]
	VecX *= -1.0f;
	VecZ *= -1.0f;

	// �x�N�g���ɋ������|���Ē�����ω�������
	float Distance = 200.0f;
	VecX *= Distance;
	VecZ *= Distance;

	// �^�[�Q�b�g�̈ʒu�Ƀx�N�g�������Z�����l��V�����J�����̍��W�Ƃ���
	float NewCameraPosX = x_ + VecX;
	float NewCameraPosY = y_;
	float NewCameraPosZ = z_ + VecZ;

	D3DXMATRIX Matrix; // �J��������ۑ����邽�߂̍s��
	D3DXVECTOR3 CameraPos = D3DXVECTOR3(NewCameraPosX, NewCameraPosY, NewCameraPosZ);
	D3DXVECTOR3 EyePos = D3DXVECTOR3( x_, y_, z_);
	D3DXVECTOR3 UpVec = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

	/*
		D3DXMatrixLookAtLH
			�J�����������s����쐬����
	*/
	// L => Left H => Hand LH => ����n�ŃJ�������W��ݒ肷��
	D3DXMatrixLookAtLH(
		&Matrix,		// ���ʕۑ��p�s��̎w��
		&CameraPos,	// �J�������W
		&EyePos,		// �����_���W
		&UpVec			// ���[���h���W��ɂ�����J�����̏�����̃x�N�g��
	);

	// SetTransform => ���W�ϊ��n�̏���DirectX�ɒʒm����
	g_Device->SetTransform(D3DTS_VIEW, &Matrix);
}

void ReleaseXFile()
{	
	for (int i = 0; i < g_XFile.MaterialNum; i++)
	{
		if (g_XFile.Textures[i] != nullptr)
		{
			g_XFile.Textures[i]->Release();
			g_XFile.Textures[i] = nullptr;
		}
	}

	//
	delete[] g_XFile.Textures;

	if (g_XFile.Materials != nullptr)
	{
		g_XFile.Materials->Release();
		g_XFile.Materials = nullptr;
	}
	
	if (g_XFile.Meshes != nullptr)
	{
		g_XFile.Meshes->Release();
		g_XFile.Meshes = nullptr;
	}
}
