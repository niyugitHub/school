#include "ObstructBase.h"



ObstructBase::ObstructBase(int handle)
{
	// �R�c���f���̓ǂݍ���
	modelHandle = MV1DuplicateModel(handle);
	if (modelHandle < 0)
	{
		printfDx("ObstructFloat:�f�[�^�ǂݍ��݂Ɏ��s");
	}

	pos = VGet(0, 0, 0);
}

ObstructBase::~ObstructBase()
{
	// ���f���̃A�����[�h.
	MV1DeleteModel(modelHandle);
}

void ObstructBase::Update()
{
	// �RD���f���̃|�W�V�����ݒ�
	MV1SetPosition(modelHandle, pos);
}

void ObstructBase::Draw()
{
	// �R�c���f���̕`��
	MV1DrawModel(modelHandle);
}
