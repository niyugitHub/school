#include "DxLib.h"
#include "Stage.h"

// �X�e�[�W�̏���������
void Stage::Initialize()
{
	// �X�e�[�W���f���̓ǂݍ���
	ModelHandle = MV1LoadModel("ColTestStage.mqo");

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(ModelHandle, -1);
}

// �X�e�[�W�̌�n������
void Stage::Finalie()
{
	// �X�e�[�W���f���̌�n��
	MV1DeleteModel(ModelHandle);
}

// �`��
void Stage::Draw()
{
	MV1DrawModel(ModelHandle);
}