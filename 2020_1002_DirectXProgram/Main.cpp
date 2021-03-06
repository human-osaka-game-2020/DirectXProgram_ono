#include<Windows.h>
#include"DirectGraphics.h"
#include "DirectInput.h"

// prama commentによるlibファイルの追加
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

// ウィンドウプロシージャー作成
/*
	戻り値：
		実行結果

	引数：
		HWND
			メッセージの対象となっているウィンドウのハンドル

			HWND
				ウィンドウのハンドルを保存する型

		UNIT
			メッセージ情報
				クリックされた等の情報が保存されている

		WPARAM
			メッセージ付加情報その１

		LPARAM
			メッセージ付加情報その２

		・付加情報にはメッセージ情報の補足が保存されている
			例：キーの詳細情報(大文字、小文字など)
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
	default:
		// メッセージを何も対応しないときに実行する関数
		//　引数にはウィンドウプロシージャーで渡されている引数をそのまま返す
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}
	// メッセージの対応をした
	return 0;
}

/*
	WinMain
		Windowsアプリにおいてのエントリポイント
		戻り値：
			アプリの結果
		引数：
			HINSTANCE hInstance :: 重要
				アプリのインスタンスハンドル
				H => Handleの略

		HINSTANCE hPrevInstance :: 以下あまり必要なし
			古い仕様の残りなので使わない
			情報としての価値はない

		LPSTR IpCmdLine,
			コマンドラインのパラメーターリスト

		INT nCmdShow
			コマンドラインの引数の数
*/
int APIENTRY WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     IpCmdLine,
	INT       nCmdChow)
{
	int width  = 400;
	int height = 400;

	// ウィンドウの生成はメインループが始まる前に行う
	// 構造体の登録
	WNDCLASS window_class =
	{
		CS_HREDRAW | CS_VREDRAW,       // クラスのスタイル(CS_HREDRAW と CS_VREDRAWは横と縦の変更許可設定)
		WindowsProcedure,              // ウィンドウプロシージャ
		0,                             // 補助メモリ
		0,                             // 補助メモリ
		hInstance,                     // このプログラムのインスタンスハンドル
		LoadIcon(nullptr,IDI_APPLICATION),// アイコン画像
		LoadCursor(nullptr,IDC_ARROW),    // カーソル画像
		nullptr,                          // 背景色
		nullptr,                          // メニュー名
		// TEXT() char* => wchar_t* に変換してくれる
		TEXT("CreateWindow"),                // クラス名
	};

	// 初期化したウィンドウ情報を登録する
	/*
	    RegisterClass
		    ウィンドウ情報を登録する関数

		戻り値
		    非０ => 登録成功
			０   => 登録失敗
	*/
	if (RegisterClass(&window_class) == 0)
	{
		return 0;
	}

	// ウィンドウ作成
	HWND window_handle = CreateWindow(
		// 登録しているウィンドウクラス構造体の名前
		TEXT("CreateWindow"),
		// ウィンドウ名
		TEXT("提出する用"),
		// ウインドウスタイル
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// 表示位置
		/*
			ウィンドウを表示する位置を指定する
			(CW_USEDEFAULT => OS任せ)
		*/
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// サイズ
		width,
		height,
		// 親ハンドル
		NULL,
		// メニューハンドル
		NULL,
		// インスタンスハンドル
		hInstance,
		// WM_CREATEメッセージでlparamに渡したい値
		NULL);

	if (window_handle == NULL)
	{
		return 0;
	}

	// リサイズ
	RECT window_rect;
	RECT client_rect;

	// ウィンドウサイズ取得
	if (GetWindowRect(window_handle, &window_rect) == false)
	{
		// 失敗
		return 0;
	}
	
	// クライアント領域のサイズ取得
	if (GetClientRect(window_handle, &client_rect) == false)
	{
		// 失敗
		return 0;
	}

	// フレームサイズ算出
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// リサイズ用サイズの算出
	int resize_width   = frame_size_x + width;
	int resize_height  = frame_size_y + height;

	// 表示位置の更新
	SetWindowPos(
		window_handle,
		NULL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// リサイズの横幅
		resize_width,
		// リサイズの縦幅
		resize_height,
		// オプションの指定
		SWP_NOMOVE);

	// ウィンドウ表示
	ShowWindow(window_handle, SW_SHOW);

	if (InitDirectGraphics(window_handle) == false)
	{
		return 0;
	}

	if (InitDirectInput() == false)
	{
		return 0;
	}

	if (LoadTexture(TextureID::TexID1) == false)
	{
		return 0;
	}

	//	LoadXFile(TEXT("res/Sample01.x"));
	LoadXFile(TEXT("Witchwait.X"));

	float PosX = 0.0f;
	float PosY = 0.0f;
	float PosZ = 0.0f;

	float Angle = 0.0f;

	while (true)
	{
		// フレーム管理
		/*
			フレーム管理はループが一定の周期になるように管理する
			601FPSでゲームが進行するなら
			１回のループは約0.016秒で実行する

			※DirectXが60FPSでフレーム管理を自動で行うように
			今回はDirectXに任せる
		*/

		// メッセージ対応
		/*
			Windows(OS)から送られてくるメッセージの
			対応を行う必要がある

			メッセージの例
				ウィンドウをクリックした
				ウィンドウを移動させた
				ウィンドウを再描画した
		
		*/
		MSG message;
		/*
			Getmessage
				Windowsからメッセージが通知されるまで
				次の処理に移行しない

				scanfに近い
		*/
		/*
		if(GetMessage(&message,nullptr,0,100))
		{
		}
		*/
		/*
		    PeekMassage有無に関わらず次の処理を実行する

			戻り値：
			    true  => メッセージ有り
				false => メッセージ無し

			第一引数：
				受信したメッセージ情報

			第二引数：
				ウィンドウハンドル
				(nullptrの場合っはデフォルトのハンドルが用意される)

			第三〜五引数：
				固定値で問題ない
		*/

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// 受信したデータを翻訳する
				TranslateMessage(&message);

				// ウィンドウズプロシージャー
				DispatchMessage(&message);
			};
		}
		else
		{
			UpdateDirectInput();

			if (IsKeyHeld(DIK_LEFTARROW))
			{
				Angle -= 1.0f;
			}
			else if (IsKeyHeld(DIK_RIGHTARROW))
			{
				Angle += 1.0f;
			}

			if (IsKeyHeld(DIK_UPARROW))
			{
				float VecZ = cosf(D3DXToRadian(Angle));
				float VecX = sinf(D3DXToRadian(Angle));
				float Speed = 1.0f;

				PosX += VecX * Speed;
				PosZ += VecZ * Speed;
			}
			else if (IsKeyHeld(DIK_DOWNARROW))
			{
				float VecZ = cosf(D3DXToRadian(Angle));
				float VecX = sinf(D3DXToRadian(Angle));
				float Speed = 1.0f;

				PosX -= VecX * Speed;
				PosZ -= VecZ * Speed;
			}

			UpdateTPSCamera(0, 0, 0, Angle);

			StartDrawing();

			SetUpProjection();

			// ゲームに関連する処理
			// ゲーム処理と画面処理を実行する

			Draw3DPorigon();

			//DrawPorigon();
			//DrawPorigonWithTriangleList();
			//DrawPorigonWithTriangleStrip();
			//DrawPorigonWithTriangleFan();

			DrawXFile(PosX, PosY, PosZ, 0);

			FinishDrawing();
		}
	}

	// 解放
	ReleaseTexture();
	ReleaseDirectGraphics();
	ReleaseDirectInput();

	return 0;
}
