#pragma once

#define CAMERA_ANGLE_SPEED				0.05f		// ���񑬓x
#define CAMERA_PLAYER_TARGET_HEIGHT		400.0f		// �v���C���[���W����ǂꂾ�������ʒu�𒍎��_�Ƃ��邩
#define CAMERA_PLAYER_LENGTH			1600.0f		// �v���C���[�Ƃ̋���
#define CAMERA_COLLISION_SIZE			50.0f		// �J�����̓����蔻��T�C�Y

class Input;
class Player;
class Stage;

class Camera
{
public:
	void Initialize();						// �J�����̏���������
	void Update(const Input& input, const Player& player, const Stage& stage);		// �J�����̏���

	const VECTOR& GetEye() const { return Eye; }
	const VECTOR& GetTarget() const { return Target; }

private:
	float		AngleH;					// �����p�x
	float		AngleV;					// �����p�x
	VECTOR		Eye;					// �J�������W
	VECTOR		Target;					// �����_���W
};

