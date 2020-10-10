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
