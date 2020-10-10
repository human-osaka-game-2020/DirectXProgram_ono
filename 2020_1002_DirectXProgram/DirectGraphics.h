#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphicsで必要な関数が定義されているヘッダファイル
#include <d3d9.h>
#include <d3dx9.h>

/*
	@brief     DirectGraphics初期化関数
	@details   描画を使用するための初期化を行う
	@retval    true初期化成功
	@retval    false 初期化失敗
	@param[in] window_handle 描画に使用するwindowハンドル
*/

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();


#endif
