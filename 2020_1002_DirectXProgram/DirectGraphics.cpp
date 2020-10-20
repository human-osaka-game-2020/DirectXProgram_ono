#include "DirectGraphics.h"

LPDIRECT3D9 g_Interface    = nullptr;
LPDIRECT3DDEVICE9 g_Device = nullptr;

bool InitDirectGraphics(HWND window_handle)
{
	/*
		Direct3DCreate9
			IDirect3D9制作関数

			引数：
				DirectXのバージョン(D3D_SDK_VERSION)

			戻り値：
				成功：
					IDirect3D9*
				失敗：
					nullptr	
	*/

	g_Interface = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_Interface == nullptr)
	{
		// 初期化失敗
		return false;
	}

	// Deviceの情報を設定するための構造体
	D3DPRESENT_PARAMETERS parameters;

	// 指定されたデータをサイズ分だけ0で初期化する
	ZeroMemory(&parameters, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファーの数
	parameters.BackBufferCount = 1;

	// バックバッファーのフォーマット
	// D3DFMT＿UNKNOWN => 知りません
	parameters.BackBufferFormat = D3DFMT_UNKNOWN;

	// ウィンドウモード指定
	/*
		true  => ウィンドウモード

		false => フルスクリーンモード
					※フルスクリーンは別途情報を指定する必要あり
	*/
	parameters.Windowed = true;

	/*
		スワップエフェクト => D3DSWAPEFFECT_DISCARD(自動設定)

		スワップエフェクトとは：
			バックフロントとフロントバッファへの切り替え方法
	*/
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

	HRESULT result = g_Interface->CreateDevice(
		// ディスプレイアダプタの種類
		D3DADAPTER_DEFAULT,
		// デバイスの種類
		D3DDEVTYPE_HAL,
		// デバイスが使用するウィンドウのハンドル
		window_handle,
		// デバイスの制御方法
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		// PRESENT_PARAMETERS
		&parameters,
		// Deviceの保存変数指定
		&g_Device
	);

	if (FAILED(result))
	{
		return false;
	}
	/*
		// 成功判定
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
		// Relase => 解放関数
		g_Device->Release();
		// 解放後の不正アクセス対応
		g_Device = nullptr;
	}

	if (g_Interface != nullptr) 
	{
		// Release => 解放関数
		g_Interface->Release();
		// 解放後の不正アクセス対応
		g_Interface = nullptr;
	}
}

void StartDrawing()
{
	g_Device->Clear(
		0,							  // 0固定
		nullptr,					  // nullptr固定
		D3DCLEAR_TARGET,			  // D3DCLEAR_TARGET固定
		D3DCOLOR_XRGB(255, 255, 200), // 塗りつぶす色
		1.0f,						  // 1.0f1固定
		0							  // 0固定
	);

	// DirectXで描画を開始する
	g_Device->BeginScene();
}

void FinishDrawing()
{
	// DirectXで描画を開始する
	g_Device->BeginScene();

	// バックバッファをフロントバッファに転送する
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

	// DirectXに頂点構造を教える必要がある
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// ポリゴンの作り方
		1,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// 1頂点のサイズ
	);
}

void DrawPorigonWithTriangleLise()
{
	CustomVertex vertices[] =
	{
		// 1つ目
		{0.0f  ,0.0f  ,0.0f,0xffffff},
		{100.0f,100.0f,0.0f,0xffffff},
		{0.0f  ,100.0f,0.0f,0xffffff},
		// 2つ目
		{100.0f,0.0f  ,0.0f,0xffffff},
		{100.0f,100.0f,0.0f,0xffffff},
		{0.0f  ,0.0f  ,0.0f,0xffffff},
	};
	// DirextXに頂点構造を教える必要がある
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,		// ポリゴンの作り方
		2,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// １頂点のサイズ
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
	// DirextXに頂点構造を教える必要がある
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,	// ポリゴンの作り方
		4,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// １頂点のサイズ
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
	// DirextXに頂点構造を教える必要がある
	g_Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


	g_Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,		// ポリゴンの作り方
		2,						// ポリゴンの数
		vertices,				// 頂点リスト
		sizeof(CustomVertex)	// １頂点のサイズ
	);
}
