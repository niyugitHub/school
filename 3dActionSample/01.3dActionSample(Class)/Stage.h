#pragma once

// �X�e�[�W���\����
class Stage
{
public:
	int GetModelHandle()const { return ModelHandle; }

	void Initialize();						// �X�e�[�W�̏���������
	void Finalie();							// �X�e�[�W�̌�n������
	void Draw();

private:
	int		ModelHandle;				// ���f���n���h��
};


