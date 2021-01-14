#ifndef INPUT_H
#define INPUT_H

#include<dinput.h>

enum InputState
{
	NoHold,		// ������Ă��Ȃ����
	Pushed,		// �������u��
	Held,		// ������Ă�����
	Released	// �����ꂽ�u��
};

bool InitDirectInput();
void ReleaseDirectInput();
void UpdateDirectInput();

// �L�[��������Ă��邩�𒲂ׂ�֐�
bool IsKeyHeld(int key);
bool IsKeyPushed(int key);
bool IsKeyReleased(int key);

#endif
