#pragma once

// ����
class Input
{
public:
	void	Update();

	int GetNowFrameInput() const { return NowFrameInput; }
	int GetNowFrameNewInput() const { return NowFrameNewInput; }

private:
	int		NowFrameInput;				// ���݂̃t���[���ŉ�����Ă���{�^��
	int		NowFrameNewInput;			// ���݂̃t���[���ŐV���ɉ����ꂽ�{�^��
};


