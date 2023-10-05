
// �v���C���[�֌W�̒�`
#define PLAYER_PLAY_ANIM_SPEED			250.0f		// �A�j���[�V�������x
#define PLAYER_MOVE_SPEED				30.0f		// �ړ����x
#define PLAYER_ANIM_BLEND_SPEED			0.1f		// �A�j���[�V�����̃u�����h���ω����x
#define PLAYER_ANGLE_SPEED				0.2f		// �p�x�ω����x
#define PLAYER_JUMP_POWER				100.0f		// �W�����v��
#define PLAYER_FALL_UP_POWER			20.0f		// ���𓥂݊O�������̃W�����v��
#define PLAYER_GRAVITY					3.0f		// �d��
#define PLAYER_MAX_HITCOLL				2048		// ��������R���W�����|���S���̍ő吔
#define PLAYER_ENUM_DEFAULT_SIZE		800.0f		// ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
#define PLAYER_HIT_WIDTH				200.0f		// �����蔻��J�v�Z���̔��a
#define PLAYER_HIT_HEIGHT				700.0f		// �����蔻��J�v�Z���̍���
#define PLAYER_HIT_TRYNUM				16			// �ǉ����o�������̍ő厎�s��
#define PLAYER_HIT_SLIDE_LENGTH			5.0f		// ��x�̕ǉ����o�������ŃX���C�h�����鋗��
#define PLAYER_SHADOW_SIZE				200.0f		// �e�̑傫��
#define PLAYER_SHADOW_HEIGHT			700.0f		// �e�������鍂��

class Input;
class Camera;
class Stage;

// ���
enum class PlayerState : int
{
	STAND = 0,		// �����~�܂�
	RUN = 1,		// ����
	JUMP = 2,		// �W�����v
};

// �A�j���[�V�������
enum class PlayerAnimKind : int
{
	NONE = -1,		// �Ȃ�
	UNKNOWN = 0,	// �s��
	RUN = 1,		// ����
	JUMP = 2,		// �W�����v
	FALL = 3,		// ������
	STOP = 4,		// �����~�܂�
};

// �v���C���[���\����
class Player
{
public:
	void Initialize();						// ������
	void Finalize();						// ��n��
	void Update(const Input& input, const Camera& camera, const Stage& stage);						
	void Draw(const Stage& stage);

	const VECTOR& GetPosition() const { return Position; }

private:
	VECTOR		Position;				// ���W
	VECTOR		TargetMoveDirection;	// ���f���������ׂ������̃x�N�g��
	float		Angle;					// ���f���������Ă�������̊p�x
	float		JumpPower;				// �x�������̑��x
	int			ModelHandle;			// ���f���n���h��
	int			ShadowHandle;			// �e�摜�n���h��
	PlayerState	State;					// ���

	int			CurrentPlayAnim;		// �Đ����Ă���A�j���[�V�����̃A�^�b�`�ԍ�( -1:�����A�j���[�V�������A�^�b�`����Ă��Ȃ� )
	float		CurrentAnimCount;		// �Đ����Ă���A�j���[�V�����̍Đ�����
	int			PrevPlayAnim;			// �O�̍Đ��A�j���[�V�����̃A�^�b�`�ԍ�( -1:�����A�j���[�V�������A�^�b�`����Ă��Ȃ� )
	float		PrevAnimCount;			// �O�̍Đ��A�j���[�V�����̍Đ�����
	float		AnimBlendRate;			// ���݂Ɖߋ��̃A�j���[�V�����̃u�����h��

	void Move(VECTOR MoveVector, const Stage& stage);	// �ړ�����
	void UpdateAngle();									// ��]����
	void PlayAnim(PlayerAnimKind PlayAnim);				// �V���ȃA�j���[�V�������Đ�����
	void UpdateAnimation();								// �A�j���[�V��������
	void DrawShadow(const Stage& stage);				// �e��`��
};