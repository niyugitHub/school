#include "DxLib.h"
#include "Input.h"

// �X�V
void Input::Update()
{
	int Old;

	// �ЂƂO�̃t���[���̓��͂�ϐ��ɂƂ��Ă���
	Old = NowFrameInput;

	// ���݂̓��͏�Ԃ��擾
	NowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���̃t���[���ŐV���ɉ����ꂽ�{�^���̃r�b�g���������Ă���l�� NowFrameNewInput �ɑ������
	NowFrameNewInput = NowFrameInput & ~Old;
}