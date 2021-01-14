#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphics�ŕK�v�Ȋ֐�����`����Ă���w�b�_�t�@�C��
#include <d3d9.h>
#include <d3dx9.h>

// �e�N�X�`�������̍\����(2D�p)
struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

// �e�N�X�`���L��̍\����(2D�p)
struct CustomVertexTex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
	float Tu;
	float Tv;
};

/*
	DrawPrimitiveUp�Ȃǂ�DirectX��CustomVertex���g�p�����֐��ɂ���
		���ۂ�3D�`����s���Ă���
		3D�`��Ȃ̂Ŗ{���̓��[���h���W�ϊ��A�r���[���W�ϊ��A�v���W�F�N�V�������W�ϊ��̂��߂�
		�ݒ���s���K�v������
		
		�������A2D�`����s�������ꍇ�͂킴�킴��L�̍��W�ϊ�������̂��ʓ|
		�����ŁARHW�����g�p���Ďw�肳�ꂽCustomVertex�̏�����C�ɃX�N���[�����W�ɕϊ����Ă���
*/
// 3D�p�̒��_�\��
struct CustomVertex3D
{
	// ���W���
	float X;
	float Y;
	float Z;
	
	// �J���[
	DWORD Color;
};

// FVF�ݒ�(2D�p�e�N�X�`������)
#define FVF_2D_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// FVF�ݒ�(2D�p�e�N�X�`���L��)
#define FVF_2D_VERTEX_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// FVF�ݒ�(3D�p)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

/*
	#define�}�N��
		�R���p�C�����Ɏg�p���Ă���ꏊ��define�ƒ�`���Ă�����e�̒u����������������

		#define ABC (1.0f)
		int a = ABC;
		�R���p�C����
		int a = (1.0f);	ABC => (1.0f)

		#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE);
		g_Device->SetFVF(FVF_3D_VERTEX);
		�R���p�C����
		g_Device->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE););

		#define Add(a, b) (a + b)
		int a = Add(1, 2);
		�R���p�C����
		int a = 1 + 2

		#define Add(a, b) (a + b)
		int b = Add(1, 2) * 3;
		�R���p�C����
		int b = (1 + 2) * 3
*/

// �e�N�X�`���̎�ޕ���enum���쐬
enum TextureID
{
	TexID1,
	TexID2,
	TexID3,
	TexIDMax
};

/*
	@brief     DirectGraphics�������֐�
	@details   �`����g�p���邽�߂̏��������s��
	@retval    true����������
	@retval    false ���������s
	@param[in] window_handle �`��Ɏg�p����window�n���h��
*/

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

void StartDrawing();

void FinishDrawing();

void DrawPorigon();

void DrawPorigonWithTriangleList();
void DrawPorigonWithTriangleStrip();
void DrawPorigonWithTriangleFan(TextureID tex_id);

// View���W�ϊ��̐ݒ�
void SetUpView();

// �v���W�F�N�V�������W�ύX�̐ݒ�
void SetUpProjection();

// 3D�|���S���`��
void Draw3DPorigon();
void DrawXFile(float x_, float y_, float z_, float angle_);

bool LoadTexture(TextureID tex_id);
bool LoadXFile(LPCWSTR file_name);

void UpdateTPSCamera(float x_, float y_, float z_, float angle_);

void ReleaseXFile();
void ReleaseTexture();

#endif
