#ifndef DIRECT_GRAPHICS_H_
#define DIRECT_GRAPHICS_H_

// DirectGraphics�ŕK�v�Ȋ֐�����`����Ă���w�b�_�t�@�C��
#include <d3d9.h>
#include <d3dx9.h>

/*
	@brief     DirectGraphics�������֐�
	@details   �`����g�p���邽�߂̏��������s��
	@retval    true����������
	@retval    false ���������s
	@param[in] window_handle �`��Ɏg�p����window�n���h��
*/

bool InitDirectGraphics(HWND window_handle);

void ReleaseDirectGraphics();


#endif
