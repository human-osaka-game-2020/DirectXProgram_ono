#include<Windows.h>

// ウィンドウプロシージャー作成
LRESULT CALLBACK WindowsProcedure(
	HWND   window_handle,
	UINT   message_id,
	WPARAM wparam,
	LPARAM lparam)
{
	switch (message_id)
	{
		// ウィンドウを閉じるアイコンがクリックされた
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
		// メッセージを何も対応しないときに実行する関数
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}
	// メッセージの対応をしたので0を返す
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

	// 構造体の登録
	HWND hWnd;
	WNDCLASSEX window_class =
	{
		sizeof(WNDCLASSEX),            // 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,       // クラスのスタイル(CS_HREDRAW と CS_VREDRAWは横と縦の変更許可設定)
		WindowsProcedure,              // ウィンドウプロシージャ
		0,                             // 補助メモリ
		0,                             // 補助メモリ
		hInstance,                     // このプログラムのインスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),// アイコン画像
		LoadCursor(NULL,IDC_ARROW),    // カーソル画像
		NULL,                          // 背景色
		NULL,                          // メニュー名
		TEXT("CreateWindow"),          // クラス名
		NULL						   // 小さいアイコン
	};

	// 初期化したウィンドウ情報を登録する
	/*
	    RegisterClass
		    ウィンドウ情報を登録する関数

		戻り値
		    非０ => 登録成功
			０   => 登録失敗
	*/
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	// ウィンドウ作成
	hWnd = CreateWindow(
		// 登録しているウィンドウクラス構造体の名前
		TEXT("CreateWindow"),
		// ウィンドウ名
		TEXT("ウィンドウ生成サンプル"),
		// ウインドウスタイル
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
		// 表示位置
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

	ShowWindow(hWnd, SW_SHOW);

	if (hWnd == NULL)
	{
		return 0;
	}

	while (true)
	{
		MSG message;

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

			第三～五引数：
				固定値で問題ない
		*/

		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			// 受信したデータを翻訳する
			TranslateMessage(&message);

			// ウィンドウズプロシージャー
			DispatchMessage(&message);
		}
		else
		{
			// ゲームに関連する処理
		}
	}

	return 0;
}
