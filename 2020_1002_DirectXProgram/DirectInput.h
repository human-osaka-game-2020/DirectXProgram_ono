#ifndef INPUT_H
#define INPUT_H

#include<dinput.h>

enum InputState
{
	NoHold,		// 押されていない状態
	Pushed,		// 押した瞬間
	Held,		// 押されている状態
	Released	// 離された瞬間
};

bool InitDirectInput();
void ReleaseDirectInput();
void UpdateDirectInput();

// キーが押されているかを調べる関数
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
