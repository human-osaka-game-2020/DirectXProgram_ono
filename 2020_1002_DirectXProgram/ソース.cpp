#include<Windows.h>

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
	// メッセージを何も対応しないときに実行する関数
	//　引数にはウィンドウプロシージャーで渡されている引数をそのまま返す
	return DefWindowProc(window_handle, message_id, wparam, lparam);
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

	// 構造体の登録
	WNDCLASSA window_class =
	{
		CS_HREDRAW | CS_VREDRAW,       // クラスのスタイル(CS_HREDRAW と CS_VREDRAWは横と縦の変更許可設定)
		WindowsProcedure,              // ウィンドウプロシージャ
		0,                             // 補助メモリ
		0,                             // 補助メモリ
		hInstance,                     // このプログラムのインスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),// アイコン画像
		LoadCursor(NULL,IDC_ARROW),    // カーソル画像
		NULL,                          // 背景色
		NULL,                          // メニュー名
		"CreateWindow",                // クラス名
	};

	// 初期化したウィンドウ情報を登録する
	/*
	    RegisterClass
		    ウィンドウ情報を登録する関数

		戻り値
		    非０ => 登録成功
			０   => 登録失敗
	*/
	if (RegisterClassA(&window_class) == 0)
	{
		return 0;
	}

	// ウィンドウ作成
	HWND window_handle = CreateWindowA(
		// 登録しているウィンドウクラス構造体の名前
		"CreateWindow",
		// ウィンドウ名
		"ウィンドウ生成サンプル",
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

	// ウィンドウ表示
	ShowWindow(window_handle, SW_SHOW);

	if (window_handle == NULL)
	{
		return 0;
	}

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
				// ゲーム処理と画面処理を実行する
		}
	}

	return 0;
}
