#ifndef INPUT_H
#define INPUT_H

#include<dinput.h>

enum InputState
{
	NoHold,		// ‰Ÿ‚³‚ê‚Ä‚¢‚È‚¢ó‘Ô
	Pushed,		// ‰Ÿ‚µ‚½uŠÔ
	Held,		// ‰Ÿ‚³‚ê‚Ä‚¢‚éó‘Ô
	Released	// —£‚³‚ê‚½uŠÔ
};

bool InitDirectInput();
void ReleaseDirectInput();
void UpdateDirectInput();

// ƒL[‚ª‰Ÿ‚³‚ê‚Ä‚¢‚é‚©‚ğ’²‚×‚éŠÖ”
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
