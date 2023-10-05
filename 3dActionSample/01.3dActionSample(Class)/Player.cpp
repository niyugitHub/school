#include <math.h>
#include "DxLib.h"
#include "Camera.h"
#include "Input.h"
#include "Stage.h"
#include "Player.h"

// �v���C���[�̏�����
void Player::Initialize()
{
	// �������W�͌��_
	Position = VGet(0.0f, 0.0f, 0.0f);

	// ��]�l�͂O
	Angle = 0.0f;

	// �W�����v�͂͏�����Ԃł͂O
	JumpPower = 0.0f;

	// ���f���̓ǂݍ���
	ModelHandle = MV1LoadModel("DxChara.x");

	// �e�`��p�̉摜�̓ǂݍ���
	ShadowHandle = LoadGraph("Shadow.tga");

	// ������Ԃł́u�����~��v���
	State = PlayerState::STAND;

	// ������ԂŃv���C���[�������ׂ������͂w������
	TargetMoveDirection = VGet(1.0f, 0.0f, 0.0f);

	// �A�j���[�V�����̃u�����h����������
	AnimBlendRate = 1.0f;

	// ������Ԃł̓A�j���[�V�����̓A�^�b�`����Ă��Ȃ��ɂ���
	CurrentPlayAnim = -1;
	PrevPlayAnim = -1;

	// ���������Ă���A�j���[�V�������Đ�
	// TODO: �}�W�b�N�i���o�[��4�����Ȃ̂���͂��Ē萔��
	PlayAnim(PlayerAnimKind::STOP);			// HACK: ���ł̓A�^�b�`�ƃJ�E���^�̏����������Ă��邾��
}

// �v���C���[�̌�n��
void Player::Finalize()
{
	// ���f���̍폜
	MV1DeleteModel(ModelHandle);

	// �e�p�摜�̍폜
	DeleteGraph(ShadowHandle);
}

// �v���C���[�̏���
void Player::Update(const Input& input, const Camera& camera, const Stage& stage)
{
	VECTOR	UpMoveVec;		// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	LeftMoveVec;	// �����{�^���u���v����͂������Ƃ��̃v���C���[�̈ړ������x�N�g��
	VECTOR	MoveVec;		// ���̃t���[���̈ړ��x�N�g��
	bool	IsMove;			// �ړ��������ǂ����̃t���O( TRUE:�ړ�����  FALSE:�ړ����Ă��Ȃ� )

	// HACK:
	// �EDX���C�u�����̃��f���t�@�C�����ɂ́A�����̃��b�V���i�|���S���̏W���j��J�����A���C�g�����邱�Ƃ��ł���
	// �E�u���������̃t�@�C���́A�e�q�֌W��������AUnity�̃q�G�����L�[�݂����ɁA�K�w�\��������
	// �E���̊K�w���ꂼ��ɂ͖��O���t������ Root-Meshes-Model1
	//											         |-Model2
	// �E���̖��O�̕t�����K�w�̂��Ƃ�DX���C�u�����ł̓t���[���Ƃ���
	// �E��Ԑe�̊K�w���u���[�g�t���[���v�ƌĂԁB���[�g�t���[���͈��
	// 
	// HACK: ���̂��߂ɁH���f���̈�Ԑe�t���[���i�e�K�w�j��Z�������̈ړ��p�����[�^���[���ɂ��Ă���

	// ���[�g�t���[���̂y�������̈ړ��p�����[�^�𖳌��ɂ���
	{
		MATRIX LocalMatrix;

		// ���[�U�[�s�����������
		MV1ResetFrameUserLocalMatrix(ModelHandle, 2);

		// ���݂̃��[�g�t���[���̍s����擾����
		LocalMatrix = MV1GetFrameLocalMatrix(ModelHandle, 2);

		// �y�������̕��s�ړ������𖳌��ɂ���
		LocalMatrix.m[3][2] = 0.0f;

		// ���[�U�[�s��Ƃ��ĕ��s�ړ������𖳌��ɂ����s������[�g�t���[���ɃZ�b�g����
		MV1SetFrameUserLocalMatrix(ModelHandle, 2, LocalMatrix);
	}

	// �v���C���[�̈ړ������̃x�N�g�����Z�o
	{
		// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���̓J�����̎�����������x�����𔲂�������
		UpMoveVec = VSub(camera.GetTarget(), camera.GetEye());
		UpMoveVec.y = 0.0f;

		// �����{�^���u���v���������Ƃ��̃v���C���[�̈ړ��x�N�g���͏���������Ƃ��̕����x�N�g���Ƃx���̃v���X�����̃x�N�g���ɐ����ȕ���
		LeftMoveVec = VCross(UpMoveVec, VGet(0.0f, 1.0f, 0.0f));

		// ��̃x�N�g���𐳋K��( �x�N�g���̒������P�D�O�ɂ��邱�� )
		UpMoveVec = VNorm(UpMoveVec);
		LeftMoveVec = VNorm(LeftMoveVec);
	}

	// ���̃t���[���ł̈ړ��x�N�g����������
	MoveVec = VGet(0.0f, 0.0f, 0.0f);

	// �ړ��������ǂ����̃t���O��������Ԃł́u�ړ����Ă��Ȃ��v��\��FALSE�ɂ���
	IsMove = false;

	// �p�b�h�̂R�{�^���ƍ��V�t�g���ǂ����������Ă��Ȃ�������v���C���[�̈ړ�����
	if (CheckHitKey(KEY_INPUT_LSHIFT) == 0 && (input.GetNowFrameInput() & PAD_INPUT_C) == 0)
	{
		// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă���������猩�č������Ɉړ�����
		if (input.GetNowFrameInput() & PAD_INPUT_LEFT)
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g�������Z����
			MoveVec = VAdd(MoveVec, LeftMoveVec);

			// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
			IsMove = true;
		}
		else
			// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă���������猩�ĉE�����Ɉړ�����
			if (input.GetNowFrameInput() & PAD_INPUT_RIGHT)
			{
				// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g���𔽓]�������̂����Z����
				MoveVec = VAdd(MoveVec, VScale(LeftMoveVec, -1.0f));

				// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
				IsMove = true;
			}

		// �����{�^���u���v�����͂��ꂽ��J�����̌��Ă�������Ɉړ�����
		if (input.GetNowFrameInput() & PAD_INPUT_UP)
		{
			// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g�������Z����
			MoveVec = VAdd(MoveVec, UpMoveVec);

			// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
			IsMove = true;
		}
		else
			// �����{�^���u���v�����͂��ꂽ��J�����̕����Ɉړ�����
			if (input.GetNowFrameInput() & PAD_INPUT_DOWN)
			{
				// �ړ��x�N�g���Ɂu���v�����͂��ꂽ���̈ړ��x�N�g���𔽓]�������̂����Z����
				MoveVec = VAdd(MoveVec, VScale(UpMoveVec, -1.0f));

				// �ړ��������ǂ����̃t���O���u�ړ������v�ɂ���
				IsMove = true;
			}

		// �v���C���[�̏�Ԃ��u�W�����v�v�ł͂Ȃ��A���{�^���P��������Ă�����W�����v����
		if (State != PlayerState::JUMP && (input.GetNowFrameNewInput() & PAD_INPUT_A))
		{
			// ��Ԃ��u�W�����v�v�ɂ���
			State = PlayerState::JUMP;

			// �x�������̑��x���Z�b�g
			JumpPower = PLAYER_JUMP_POWER;

			// �W�����v�A�j���[�V�����̍Đ�
			PlayAnim(PlayerAnimKind::JUMP);
		}
	}

	// �ړ��{�^���������ꂽ���ǂ����ŏ����𕪊�
	if (IsMove)
	{
		// �ړ��x�N�g���𐳋K���������̂��v���C���[�������ׂ������Ƃ��ĕۑ�
		TargetMoveDirection = VNorm(MoveVec);

		// �v���C���[�������ׂ������x�N�g�����v���C���[�̃X�s�[�h�{�������̂��ړ��x�N�g���Ƃ���
		MoveVec = VScale(TargetMoveDirection, PLAYER_MOVE_SPEED);

		// �������܂Łu�����~�܂�v��Ԃ�������
		if (State == PlayerState::STAND)
		{
			// ����A�j���[�V�������Đ�����
			PlayAnim(PlayerAnimKind::RUN);

			// ��Ԃ��u����v�ɂ���
			State = PlayerState::RUN;
		}
	}
	else
	{
		// ���̃t���[���ňړ����Ă��Ȃ��āA����Ԃ��u����v��������
		if (State == PlayerState::RUN)
		{
			// �����~��A�j���[�V�������Đ�����
			PlayAnim(PlayerAnimKind::STOP);

			// ��Ԃ��u�����~��v�ɂ���
			State = PlayerState::STAND;
		}
	}

	// ��Ԃ��u�W�����v�v�̏ꍇ��
	if (State == PlayerState::JUMP)
	{
		// �x�������̑��x���d�͕����Z����
		JumpPower -= PLAYER_GRAVITY;

		// �����������Ă��Ċ��Đ�����Ă���A�j���[�V�������㏸���p�̂��̂������ꍇ��
		if (JumpPower < 0.0f && MV1GetAttachAnim(ModelHandle, CurrentPlayAnim) == 2)
		{
			// �������悤�̃A�j���[�V�������Đ�����
			PlayAnim(PlayerAnimKind::JUMP);
		}

		// �ړ��x�N�g���̂x�������x�������̑��x�ɂ���
		MoveVec.y = JumpPower;
	}

	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	UpdateAngle();

	// �ړ��x�N�g�������ɃR���W�������l�����v���C���[���ړ�
	Move(MoveVec, stage);

	// �A�j���[�V��������
	UpdateAnimation();
}

// �`��
void Player::Draw(const Stage& stage)
{
	MV1DrawModel(ModelHandle);
	DrawShadow(stage);
}

// �v���C���[�̈ړ�����
void Player::Move(VECTOR MoveVector, const Stage& stage)
{
	int		i, j, k;						// �ėp�J�E���^�ϐ�
	int		IsMove;							// ���������Ɉړ��������ǂ����̃t���O( TRUE:�ړ����Ă��Ȃ�  FALSE:�ړ����� )
	bool	DoFixPos;						// �ǂ��牟���o������(�|�W�V�����̕␳)���s�����ǂ����̃t���O
	MV1_COLL_RESULT_POLY_DIM HitDim;		// �v���C���[�̎��͂ɂ���|���S�������o�������ʂ��������铖���蔻�茋�ʍ\����
	int KabeNum;							// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int YukaNum;							// ���|���S���Ɣ��f���ꂽ�|���S���̐�
	MV1_COLL_RESULT_POLY* Kabe[PLAYER_MAX_HITCOLL];	// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* Yuka[PLAYER_MAX_HITCOLL];	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* Poly;				// �|���S���̍\���̂ɃA�N�Z�X���邽�߂Ɏg�p����|�C���^( �g��Ȃ��Ă��ς܂����܂����v���O�����������Ȃ�̂ŁE�E�E )
	HITRESULT_LINE LineRes;					// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
	VECTOR OldPos;							// �ړ��O�̍��W	
	VECTOR NowPos;							// �ړ���̍��W

	// �ړ��O�̍��W��ۑ�
	OldPos = Position;

	// �ړ���̍��W���Z�o
	NowPos = VAdd(Position, MoveVector);

	// �v���C���[�̎��͂ɂ���X�e�[�W�|���S�����擾����
	// ( ���o����͈͈͂ړ��������l������ )
	HitDim = MV1CollCheck_Sphere(stage.GetModelHandle(), -1, Position, PLAYER_ENUM_DEFAULT_SIZE + VSize(MoveVector));

	// HACK: �ړ�������0.01�����Ŕ����Ɉړ����Ă����ꍇ�͂�����ړ����ăo�O��
	// x����y�������� 0.01f �ȏ�ړ������ꍇ�́u�ړ������v�t���O���P�ɂ���
	if (fabs(MoveVector.x) > 0.01f || fabs(MoveVector.z) > 0.01f)
	{
		IsMove = true;
	}
	else
	{
		IsMove = false;
	}

	// HACK: �ǂ�XZ���ʂɐ����ł���O��Ő��藧���Ă���B����ȊO��u���ƃo�O��
	// ���o���ꂽ�|���S�����ǃ|���S��( �w�y���ʂɐ����ȃ|���S�� )�����|���S��( �w�y���ʂɐ����ł͂Ȃ��|���S�� )���𔻒f����
	{
		// �ǃ|���S���Ə��|���S���̐�������������
		KabeNum = 0;
		YukaNum = 0;

		// ���o���ꂽ�|���S���̐������J��Ԃ�
		for (i = 0; i < HitDim.HitNum; i++)
		{
			// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
			if (HitDim.Dim[i].Normal.y < 0.000001f && HitDim.Dim[i].Normal.y > -0.000001f)
			{
				// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
				if (HitDim.Dim[i].Position[0].y > Position.y + 1.0f ||
					HitDim.Dim[i].Position[1].y > Position.y + 1.0f ||
					HitDim.Dim[i].Position[2].y > Position.y + 1.0f)
				{
					// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
					if (KabeNum < PLAYER_MAX_HITCOLL)
					{
						// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
						Kabe[KabeNum] = &HitDim.Dim[i];

						// �ǃ|���S���̐������Z����
						KabeNum++;
					}
				}
			}
			else
			{
				// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if (YukaNum < PLAYER_MAX_HITCOLL)
				{
					// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
					Yuka[YukaNum] = &HitDim.Dim[i];

					// ���|���S���̐������Z����
					YukaNum++;
				}
			}
		}
	}

	// �|�W�V�����̕␳�͂��Ȃ���Ԃɂ��Ă���
	DoFixPos = false;

	// �ǃ|���S���Ƃ̓����蔻�菈��
	if (KabeNum != 0)
	{
		// �ړ��������ǂ����ŏ����𕪊�
		if (IsMove)
		{
			// �ǃ|���S���̐������J��Ԃ�
			for (i = 0; i < KabeNum; i++)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				Poly = Kabe[i];

				// �|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
				if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == FALSE)
				{
					continue;
				}

				// �����ɂ�����|���S���ƃv���C���[���������Ă���Ƃ������ƂȂ̂ŁA��������|�W�V�����␳���s����Ԃɂ���
				DoFixPos = true;

				// �ǂɓ���������ǂɎՂ��Ȃ��ړ����������ړ�����
				{
					VECTOR MoveNormalCross;	// �i�s�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g��
					VECTOR SlideVec;		// �v���C���[���X���C�h������x�N�g��

					// �i�s�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o
					MoveNormalCross = VCross(MoveVector, Poly->Normal);

					// �Z�o�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o�A���ꂪ
					// ���̈ړ���������Ǖ����̈ړ������𔲂����x�N�g��
					// ���̃x�N�g�����g���ăX���C�h���邱�ƂŁA�ǂɂ����Ĉړ������ꍇ�̈ʒu�ɋ߂��ꏊ���o��
					// ���ǂ���͂͂ݏo�Ȃ�
					SlideVec = VCross(Poly->Normal, MoveNormalCross);

					// NowPos�͈ړ���̍��W
					// ���X�͈ړ��x�N�g�������݂̍��W�ɑ���������
					// ������ړ��ʂƊ֌W�Ȃ����S�ɏ㏑�����Ă���
					
					// ������ړ��O�̍��W�ɑ��������̂�V���ȍ��W�Ƃ���
					NowPos = VAdd(OldPos, SlideVec);
				}

				// �V���Ȉړ����W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
				for (j = 0; j < KabeNum; j++)
				{
					// j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					Poly = Kabe[j];

					// �������Ă����烋�[�v���甲����
					if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE)
					{
						// NowPos���X�V���������ŕǂɓ����������Ƃɂ���̂ŁA�̂��̉����o���������s��
						break;
					}
				}

				// j �� KabeNum �������ꍇ�͂ǂ̃|���S���Ƃ�������Ȃ������Ƃ������ƂȂ̂�
				// �ǂɓ��������t���O��|������Ń��[�v���甲����
				// NowPos���X�V���������ŕǂɓ�����Ȃ��������Ƃɂ���̂ŁA�̂��̉����o���������s��Ȃ�
				if (j == KabeNum)
				{
					DoFixPos = false;
					break;
				}
			}
		}
		else
		{
			// �ړ����Ă��Ȃ��ꍇ�̏���

			// �ǃ|���S���̐������J��Ԃ�
			for (i = 0; i < KabeNum; i++)
			{
				// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				Poly = Kabe[i];

				// �|���S���ɓ������Ă�����|�W�V�����␳���s��
				if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE)
				{
					DoFixPos = true;
					break;
				}
			}
		}

		// �ǂɓ�����Ȃǂ��āA�|�W�V�����̕␳���K�v�ȏꍇ�A�␳���s��
		if (DoFixPos)
		{
			// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
			for (k = 0; k < PLAYER_HIT_TRYNUM; k++)
			{
				// �ǃ|���S���̐������J��Ԃ�
				for (i = 0; i < KabeNum; i++)
				{
					// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
					Poly = Kabe[i];

					// �v���C���[�Ɠ������Ă��邩�𔻒�
					if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == FALSE) continue;

					// �������Ă�����K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
					NowPos = VAdd(NowPos, VScale(Poly->Normal, PLAYER_HIT_SLIDE_LENGTH));

					// �ړ�������ŕǃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
					for (j = 0; j < KabeNum; j++)
					{
						// �������Ă����烋�[�v�𔲂���
						Poly = Kabe[j];
						if (HitCheck_Capsule_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), PLAYER_HIT_WIDTH, Poly->Position[0], Poly->Position[1], Poly->Position[2]) == TRUE) break;
					}

					// �S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
					if (j == KabeNum) break;
				}

				// i �� KabeNum �ł͂Ȃ��ꍇ�͑S���̃|���S���ŉ����o�������݂�O�ɑS�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����Ƃ������ƂȂ̂Ń��[�v���甲����
				if (i != KabeNum) break;
			}
		}
	}

	// ���|���S���Ƃ̓����蔻��
	if (YukaNum != 0)
	{
		// �����������ǂ����̃t���O��������
		bool IsHitYuka = false;

		// �W�����v�����㏸���̏ꍇ�͏����𕪊�
		if (State == PlayerState::JUMP && JumpPower > 0.0f)
		{
			float MinY;

			// �V��ɓ����Ԃ��鏈�����s��

			// ��ԒႢ�V��ɂԂ���ׂ̔���p�ϐ���������
			MinY = 0.0f;

			// ���|���S���̐������J��Ԃ�
			for (i = 0; i < YukaNum; i++)
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				Poly = Yuka[i];

				// ���悩�瓪�̍����܂ł̊ԂŃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
				LineRes = HitCheck_Line_Triangle(NowPos, VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);

				// �ڐG���Ă��Ȃ������牽�����Ȃ�
				if (LineRes.HitFlag == false) continue;

				// ���Ƀ|���S���ɓ������Ă��āA�����܂Ō��o�����V��|���S����荂���ꍇ�͉������Ȃ�
				if (IsHitYuka == true && MinY < LineRes.Position.y) continue;

				// �|���S���ɓ��������t���O�𗧂Ă�
				IsHitYuka = true;

				// �ڐG�����x���W��ۑ�����
				MinY = LineRes.Position.y;
			}

			// �ڐG�����|���S�������������ǂ����ŏ����𕪊�
			if (IsHitYuka == true)
			{
				// �ڐG�����ꍇ�̓v���C���[�̂x���W��ڐG���W�����ɍX�V
				NowPos.y = MinY - PLAYER_HIT_HEIGHT;

				// �x�������̑��x�͔��]
				JumpPower = -JumpPower;
			}
		}
		else
		{
			float MaxY;

			// ���~�����W�����v���ł͂Ȃ��ꍇ�̏���
			// ��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
			MaxY = 0.0f;

			// ���|���S���̐������J��Ԃ�
			for (i = 0; i < YukaNum; i++)
			{
				// i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
				Poly = Yuka[i];

				// �W�����v�����ǂ����ŏ����𕪊�
				if (State == PlayerState::JUMP)
				{
					// �W�����v���̏ꍇ�͓��̐悩�瑫���菭���Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�
					LineRes = HitCheck_Line_Triangle(VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(NowPos, VGet(0.0f, -1.0f, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);
				}
				else
				{
					// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
					LineRes = HitCheck_Line_Triangle(VAdd(NowPos, VGet(0.0f, PLAYER_HIT_HEIGHT, 0.0f)), VAdd(NowPos, VGet(0.0f, -40.0f, 0.0f)), Poly->Position[0], Poly->Position[1], Poly->Position[2]);
				}

				// �������Ă��Ȃ������牽�����Ȃ�
				if (LineRes.HitFlag == FALSE) continue;

				// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
				if (IsHitYuka == true && MaxY > LineRes.Position.y) continue;

				// �|���S���ɓ��������t���O�𗧂Ă�
				IsHitYuka = true;

				// �ڐG�����x���W��ۑ�����
				MaxY = LineRes.Position.y;
			}

			// ���|���S���ɓ����������ǂ����ŏ����𕪊�
			if (IsHitYuka == true)
			{
				// ���������ꍇ

				// �ڐG�����|���S���ň�ԍ����x���W���v���C���[�̂x���W�ɂ���
				NowPos.y = MaxY;

				// �x�������̈ړ����x�͂O��
				JumpPower = 0.0f;

				// �����W�����v���������ꍇ�͒��n��Ԃɂ���
				if (State == PlayerState::JUMP)
				{
					// �ړ����Ă������ǂ����Œ��n��̏�ԂƍĐ�����A�j���[�V�����𕪊򂷂�
					if (IsMove)
					{
						// �ړ����Ă���ꍇ�͑����Ԃ�
						PlayAnim(PlayerAnimKind::RUN);
						State = PlayerState::RUN;
					}
					else
					{
						// �ړ����Ă��Ȃ��ꍇ�͗����~���Ԃ�
						PlayAnim(PlayerAnimKind::STOP);
						State = PlayerState::STAND;
					}

					// ���n���̓A�j���[�V�����̃u�����h�͍s��Ȃ�
					AnimBlendRate = 1.0f;
				}
			}
			else
			{
				// ���R���W�����ɓ������Ă��Ȃ��Ċ��W�����v��Ԃł͂Ȃ������ꍇ��
				if (State != PlayerState::JUMP)
				{
					// �W�����v���ɂ���
					State = PlayerState::JUMP;

					// ������Ƃ����W�����v����
					JumpPower = PLAYER_FALL_UP_POWER;

					// �A�j���[�V�����͗������̂��̂ɂ���
					PlayAnim(PlayerAnimKind::JUMP);
				}
			}
		}
	}

	// �V�������W��ۑ�����
	Position = NowPos;

	// �v���C���[�̃��f���̍��W���X�V����
	MV1SetPosition(ModelHandle, Position);

	// ���o�����v���C���[�̎��͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(HitDim);
}

// �v���C���[�̉�]����
void Player::UpdateAngle()
{
	// �v���C���[�̈ړ������Ƀ��f���̕������߂Â���
	float TargetAngle;			// �ڕW�p�x
	float difference;			// �ڕW�p�x�ƌ��݂̊p�x�Ƃ̍�

	// �ڕW�̕����x�N�g������p�x�l���Z�o����
	TargetAngle = static_cast<float>(atan2(TargetMoveDirection.x, TargetMoveDirection.z));

	// �ڕW�̊p�x�ƌ��݂̊p�x�Ƃ̍�������o��
	{
		// �ŏ��͒P���Ɉ����Z
		difference = TargetAngle - Angle;

		// ����������炠������̍����P�W�O�x�ȏ�ɂȂ邱�Ƃ͖����̂�
		// ���̒l���P�W�O�x�ȏ�ɂȂ��Ă�����C������
		if (difference < -DX_PI_F)
		{
			difference += DX_TWO_PI_F;
		}
		else
			if (difference > DX_PI_F)
			{
				difference -= DX_TWO_PI_F;
			}
	}

	// �p�x�̍����O�ɋ߂Â���
	if (difference > 0.0f)
	{
		// �����v���X�̏ꍇ�͈���
		difference -= PLAYER_ANGLE_SPEED;
		if (difference < 0.0f)
		{
			difference = 0.0f;
		}
	}
	else
	{
		// �����}�C�i�X�̏ꍇ�͑���
		difference += PLAYER_ANGLE_SPEED;
		if (difference > 0.0f)
		{
			difference = 0.0f;
		}
	}

	// ���f���̊p�x���X�V
	Angle = TargetAngle - difference;
	MV1SetRotationXYZ(ModelHandle, VGet(0.0f, Angle + DX_PI_F, 0.0f));
}

// �v���C���[�̃A�j���[�V�������Đ�����
void Player::PlayAnim(PlayerAnimKind PlayAnim)
{
	// HACK: �w�肵���ԍ��̃A�j���[�V�������A�^�b�`���A���O�ɍĐ����Ă����A�j���[�V�����̏���prev�Ɉڍs���Ă���
	// ����ւ����s���̂ŁA�P�O�̃��[�V���������L����������f�^�b�`����
	if (PrevPlayAnim != -1)
	{
		MV1DetachAnim(ModelHandle, PrevPlayAnim);
		PrevPlayAnim = -1;
	}

	// ���܂ōĐ����̃��[�V�������������̂̏���Prev�Ɉړ�����
	PrevPlayAnim = CurrentPlayAnim;
	PrevAnimCount = CurrentAnimCount;

	// �V���Ɏw��̃��[�V���������f���ɃA�^�b�`���āA�A�^�b�`�ԍ���ۑ�����
	CurrentPlayAnim = MV1AttachAnim(ModelHandle, static_cast<int>(PlayAnim));
	CurrentAnimCount = 0.0f;

	// �u�����h����Prev���L���ł͂Ȃ��ꍇ�͂P�D�O��( ���݃��[�V�������P�O�O���̏�� )�ɂ���
	AnimBlendRate = PrevPlayAnim == -1 ? 1.0f : 0.0f;
}

// �v���C���[�̃A�j���[�V��������
void Player::UpdateAnimation()
{
	float AnimTotalTime;		// �Đ����Ă���A�j���[�V�����̑�����

	// �u�����h�����P�ȉ��̏ꍇ�͂P�ɋ߂Â���
	if (AnimBlendRate < 1.0f)
	{
		AnimBlendRate += PLAYER_ANIM_BLEND_SPEED;
		if (AnimBlendRate > 1.0f)
		{
			AnimBlendRate = 1.0f;
		}
	}

	// �Đ����Ă���A�j���[�V�����P�̏���
	if (CurrentPlayAnim != -1)
	{
		// �A�j���[�V�����̑����Ԃ��擾
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, CurrentPlayAnim);

		// �Đ����Ԃ�i�߂�
		CurrentAnimCount += PLAYER_PLAY_ANIM_SPEED;

		// �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
		if (CurrentAnimCount >= AnimTotalTime)
		{
			CurrentAnimCount = static_cast<float>(fmod(CurrentAnimCount, AnimTotalTime));
		}

		// �ύX�����Đ����Ԃ����f���ɔ��f������
		MV1SetAttachAnimTime(ModelHandle, CurrentPlayAnim, CurrentAnimCount);

		// �A�j���[�V�����P�̃��f���ɑ΂��锽�f�����Z�b�g
		MV1SetAttachAnimBlendRate(ModelHandle, CurrentPlayAnim, AnimBlendRate);
	}

	// �Đ����Ă���A�j���[�V�����Q�̏���
	if (PrevPlayAnim != -1)
	{
		// �A�j���[�V�����̑����Ԃ��擾
		AnimTotalTime = MV1GetAttachAnimTotalTime(ModelHandle, PrevPlayAnim);

		// �Đ����Ԃ�i�߂�
		PrevAnimCount += PLAYER_PLAY_ANIM_SPEED;

		// �Đ����Ԃ������Ԃɓ��B���Ă�����Đ����Ԃ����[�v������
		if (PrevAnimCount > AnimTotalTime)
		{
			PrevAnimCount = static_cast<float>(fmod(PrevAnimCount, AnimTotalTime));
		}

		// �ύX�����Đ����Ԃ����f���ɔ��f������
		MV1SetAttachAnimTime(ModelHandle, PrevPlayAnim, PrevAnimCount);

		// �A�j���[�V�����Q�̃��f���ɑ΂��锽�f�����Z�b�g
		MV1SetAttachAnimBlendRate(ModelHandle, PrevPlayAnim, 1.0f - AnimBlendRate);
	}
}

// �v���C���[�̉e��`��
void Player::DrawShadow(const Stage& stage)
{
	MV1_COLL_RESULT_POLY_DIM HitResultDim;
	MV1_COLL_RESULT_POLY* HitResult;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
	HitResultDim = MV1CollCheck_Capsule(stage.GetModelHandle(), -1, Position, VAdd(Position, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

	// ���_�f�[�^�ŕω��������������Z�b�g
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
	HitResult = HitResultDim.Dim;
	for (int i = 0; i < HitResultDim.HitNum; i++, HitResult++)
	{
		// �|���S���̍��W�͒n�ʃ|���S���̍��W
		Vertex[0].pos = HitResult->Position[0];
		Vertex[1].pos = HitResult->Position[1];
		Vertex[2].pos = HitResult->Position[2];

		// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
		SlideVec = VScale(HitResult->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// �|���S���̕s�����x��ݒ肷��
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (HitResult->Position[0].y > Position.y - PLAYER_SHADOW_HEIGHT)
			Vertex[0].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[0].y - Position.y) / PLAYER_SHADOW_HEIGHT)));

		if (HitResult->Position[1].y > Position.y - PLAYER_SHADOW_HEIGHT)
			Vertex[1].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[1].y - Position.y) / PLAYER_SHADOW_HEIGHT)));

		if (HitResult->Position[2].y > Position.y - PLAYER_SHADOW_HEIGHT)
			Vertex[2].dif.a = static_cast<BYTE>(128 * (1.0f - static_cast<float>(fabs(HitResult->Position[2].y - Position.y) / PLAYER_SHADOW_HEIGHT)));

		// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
		Vertex[0].u = (HitResult->Position[0].x - Position.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[0].v = (HitResult->Position[0].z - Position.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[1].u = (HitResult->Position[1].x - Position.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[1].v = (HitResult->Position[1].z - Position.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[2].u = (HitResult->Position[2].x - Position.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
		Vertex[2].v = (HitResult->Position[2].z - Position.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

		// �e�|���S����`��
		DrawPolygon3D(Vertex, 1, ShadowHandle, TRUE);
	}

	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(HitResultDim);

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}