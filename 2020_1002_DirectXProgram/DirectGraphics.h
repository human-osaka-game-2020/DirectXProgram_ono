#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphicsで必要な関数が定義されているヘッダファイル
#include <d3d9.h>
#include <d3dx9.h>

// テクスチャ無しの構造体(2D用)
struct CustomVertex
{
	float X;
	float Y;
	float Z;
	float Rhw;
	DWORD Color;
};

// テクスチャ有りの構造体(2D用)
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
	DrawPrimitiveUpなどのDirectXのCustomVertexを使用した関数について
		実際は3D描画を行っている
		3D描画なので本来はワールド座標変換、ビュー座標変換、プロジェクション座標変換のための
		設定を行う必要がある
		
		しかし、2D描画を行いたい場合はわざわざ上記の座標変換をするのが面倒
		そこで、RHW情報を使用して指定されたCustomVertexの情報を一気にスクリーン座標に変換している
*/
// 3D用の頂点構造
struct CustomVertex3D
{
	// 座標情報
	float X;
	float Y;
	float Z;
	
	// カラー
	DWORD Color;
};

// FVF設定(2D用テクスチャ無し)
#define FVF_2D_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// FVF設定(2D用テクスチャ有り)
#define FVF_2D_VERTEX_TEXTURE (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// FVF設定(3D用)
#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

/*
	#defineマクロ
		コンパイル時に使用している場所のdefineと定義している内容の置き換えが発生する

		#define ABC (1.0f)
		int a = ABC;
		コンパイル後
		int a = (1.0f);	ABC => (1.0f)

		#define FVF_3D_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE);
		g_Device->SetFVF(FVF_3D_VERTEX);
		コンパイル後
		g_Device->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE););

		#define Add(a, b) (a + b)
		int a = Add(1, 2);
		コンパイル後
		int a = 1 + 2

		#define Add(a, b) (a + b)
		int b = Add(1, 2) * 3;
		コンパイル後
		int b = (1 + 2) * 3
*/

// テクスチャの種類分のenumを作成
enum TextureID
{
	TexID1,
	TexID2,
	TexID3,
	TexIDMax
};

/*
	@brief     DirectGraphics初期化関数
	@details   描画を使用するための初期化を行う
	@retval    true初期化成功
	@retval    false 初期化失敗
	@param[in] window_handle 描画に使用するwindowハンドル
*/

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();

void StartDrawing();

void FinishDrawing();

void DrawPorigon();

void DrawPorigonWithTriangleList();
void DrawPorigonWithTriangleStrip();
void DrawPorigonWithTriangleFan(TextureID tex_id);

// View座標変換の設定
void SetUpView();

// プロジェクション座標変更の設定
void SetUpProjection();

// 3Dポリゴン描画
void Draw3DPorigon();
void DrawXFile(float x_, float y_, float z_, float angle_);

bool LoadTexture(TextureID tex_id);
bool LoadXFile(LPCWSTR file_name);

void UpdateTPSCamera(float x_, float y_, float z_, float angle_);

void ReleaseXFile();
void ReleaseTexture();

#endif
