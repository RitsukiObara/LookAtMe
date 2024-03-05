//============================================
//
// �v���C���[�̃��C������[player.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "area.h"
#include "sound.h"
#include "useful.h"

#include "player.h"
#include "player_action.h"
#include "handgun.h"
#include "dagger.h"
#include "aim.h"
#include "bulletUI.h"
#include "gold_bone_UI.h"
#include "lifeUI.h"
#include "player_controller.h"
#include "airplane.h"
#include "shadowCircle.h"

#include "collision.h"
#include "camera.h"
#include "objectElevation.h"
#include "motion.h"
#include "block.h"
#include "orbit.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
namespace
{
	const float GRAVITY = 1.0f;						// �d��
	const float LAND_GRAVITY = -50.0f;				// ���n���̏d��
	const float SHADOW_RADIUS = 70.0f;				// �e�̔��a
	const D3DXVECTOR3 INIT_CAMERA_ROT = D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f);		// �J�����̏�������
	const float INIT_POSV_CAMERA_Y = 250.0f;		// �J�����̎��_��Y���W
	const float ROT_CORRECT = 0.2f;					// �����̕␳�{��
	const D3DXVECTOR3 AIRPLANE_ARRIVAL_MOVE = D3DXVECTOR3(0.0f, 30.0f, 0.0f);			// ��s�@�������̈ړ���
	const D3DXVECTOR3 AIRPLANE_ARRIVAL_POSV = D3DXVECTOR3(-1000.0f, 1500.0f, 0.0f);		// ��s�@�������̃J�����̎��_�̈ʒu
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 200.0f, 40.0f);				// �����蔻�莞�̃T�C�Y
	const D3DXVECTOR3 GUN_POS[NUM_HANDGUN] =		// ���e�̈ʒu
	{
		D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
		D3DXVECTOR3(+10.0f, 0.0f, 0.0f)
	};
	const D3DXVECTOR3 GUN_ROT[NUM_HANDGUN] =		// ���e�̌���
	{
		D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, -D3DX_PI * 0.5f),
		D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, D3DX_PI * 0.5f)
	};
	const int INIT_GUN_IDX = 9;						// �e�̏����l�̃C���f�b�N�X
	const int INIT_DAGGER_IDX = 9;					// �_�K�[�̃C���f�b�N�X
	const int MAX_LIFE = 100;						// �̗͂̍ő�l
	const int DAMAGE_COUNT = 30;					// �_���[�W��Ԃ̃J�E���g��
	const D3DXCOLOR DAMAGE_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// �_���[�W��Ԃ̐F
	const int INVINCIBLE_COUNT = 90;				// ���G��Ԃ̃J�E���g��
	const int INVINCIBLE_FLUSH_COUNT = 10;			// ���G��Ԃ̓_�ł̃J�E���g

	const float KNOCKBACK_MOVE = 23.0f;				// ������Ԉړ���
	const float KNOCKBACK_JUMP = 15.0f;				// ������ԍ���
	const int WIND_SHOT_DAMAGE = 10;				// ���U���̃_���[�W
	const int FIRE_SHOT_DAMAGE = 20;				// ���U���̃_���[�W
	const int BOSS_RUSH_DAMAGE = 50;				// �{�X�ːi�̃_���[�W
}

//=========================================
// �R���X�g���N�^
//=========================================
CPlayer::CPlayer() : CCharacter(CObject::TYPE_PLAYER, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_pMotion = nullptr;					// ���[�V�����̏��
	m_pAction = nullptr;					// �v���C���[�̍s���̏��
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;		// ���e�̏��
	}
	m_pAim = nullptr;						// �G�C���̏��
	m_pDagger = nullptr;					// �_�K�[�̏��
	m_pBulletUI = nullptr;					// �e��UI�̏��
	m_pGoldBoneUI = nullptr;				// ���̍���UI�̏��
	m_pLifeUI = nullptr;					// ����UI�̏��
	m_pController = nullptr;				// �v���C���[�̃R���g���[���[�̏��
	m_pAirplane = nullptr;					// ��s�@�̏��
	m_pShadow = nullptr;					// �ۉe�̏��

	m_stateInfo.state = STATE_NONE;			// ���
	m_stateInfo.nCount = 0;					// ��ԃJ�E���g
	m_stateInfo.col = NONE_D3DXCOLOR;		// �̂̐F
	m_rotDest = NONE_D3DXVECTOR3;			// �ڕW�̌���
	m_move = NONE_D3DXVECTOR3;				// �ړ���
	m_nLife = MAX_LIFE;						// �̗�
	m_nAreaIdx = 0;							// �敪�̔ԍ�
	m_bMove = false;						// �ړ���
	m_bJump = false;						// �W�����v��
}

//=========================================
// �f�X�g���N�^
//=========================================
CPlayer::~CPlayer()
{

}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CPlayer::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���f���̑�����ݒ肷��
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_PLAYER));

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // ���[�V������ NULL �������ꍇ

		// ���[�V�����̐�������
		m_pMotion = CMotion::Create();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�V�����̏����擾����
		m_pMotion->SetInfo(CMotion::STYLE_PLAYER, GetHierarchy(), GetNumModel());
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	// ���[�V�����̐ݒ菈��
	m_pMotion->Set(MOTIONTYPE_NEUTRAL);

	if (m_pAction == nullptr)
	{ // �s���̏�� NULL �̏ꍇ

		// �v���C���[�A�N�V�����̐�������
		m_pAction = CPlayerAction::Create();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pBulletUI == nullptr)
	{ // �c�eUI�� NULL �̏ꍇ

		// �e��UI�̐���
		m_pBulletUI = CBulletUI::Create();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pGoldBoneUI == nullptr)
	{ // ���̍�UI�� NULL �̏ꍇ

		// ���̍���UI�̐���
		m_pGoldBoneUI = CGoldBoneUI::Create();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pLifeUI == nullptr)
	{ // ���̍�UI�� NULL �̏ꍇ

		// ���̍���UI�̐���
		m_pLifeUI = CLifeUI::Create(MAX_LIFE);
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pController == nullptr)
	{ // �v���C���[�̃R���g���[���[�̏�� NULL �̏ꍇ

		// �v���C���[�R���g���[���[�̐���
		m_pController = CPlayerController::Create();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	// �J�����̌�����ݒ�
	CManager::Get()->GetCamera()->SetRot(INIT_CAMERA_ROT);

	// �S�Ă̒l������������
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;	// ���e�̏��
	}
	m_pAim = nullptr;					// �G�C���̏��
	m_pDagger = nullptr;				// �_�K�[�̏��
	m_pShadow = nullptr;				// �ۉe�̏��

	m_stateInfo.state = STATE_NONE;		// ���
	m_stateInfo.nCount = 0;				// ��ԃJ�E���g
	m_stateInfo.col = NONE_D3DXCOLOR;	// �̂̐F
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_move = NONE_D3DXVECTOR3;			// �ړ���
	m_nLife = MAX_LIFE;					// �̗�
	m_bMove = false;					// �ړ���
	m_bJump = false;					// �W�����v��

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // ���[�V������ NULL ����Ȃ��ꍇ

		// ���[�V�����̃��������J������
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pAction != nullptr)
	{ // �s���� NULL ����Ȃ��ꍇ

		// �s���̃��������J������
		m_pAction->Uninit();
		m_pAction = nullptr;
	}

	if (m_pAim != nullptr)
	{ // �G�C���� NULL ����Ȃ��ꍇ

		// �G�C���̃��������J������
		m_pAim->Uninit();
		m_pAim = nullptr;
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // ���e�̏�� NULL ����Ȃ��ꍇ

			// ���e�̏I������
			m_apHandGun[nCntGun]->Uninit();
			m_apHandGun[nCntGun] = nullptr;
		}
	}

	if (m_pDagger != nullptr)
	{ // �_�K�[�̏�� NULL ����Ȃ��ꍇ

		// �_�K�[�̏I������
		m_pDagger->Uninit();
		m_pDagger = nullptr;
	}

	if (m_pBulletUI != nullptr)
	{ // �c�eUI�̏�� NULL ����Ȃ��ꍇ

		// �c�eUI�� NULL �ɂ���
		m_pBulletUI = nullptr;
	}

	if (m_pGoldBoneUI != nullptr)
	{ // ���̍��̏�� NULL ����Ȃ��ꍇ

		// ���̍�UI�� NULL �ɂ���
		m_pGoldBoneUI = nullptr;
	}

	if (m_pLifeUI != nullptr)
	{ // ����UI�̏�� NULL ����Ȃ��ꍇ

		// ����UI�� NULL �ɂ���
		m_pLifeUI = nullptr;
	}

	if (m_pController != nullptr)
	{ // �R���g���[���[�̏�� NULL ����Ȃ��ꍇ

		// �I������
		m_pController->Uninit();
		m_pController = nullptr;
	}

	if (m_pAirplane != nullptr)
	{ // ��s�@�̏�� NULL ����Ȃ��ꍇ

		// ��s�@�̏I������
		m_pAirplane->Uninit();
		m_pAirplane = nullptr;
	}

	// �I������
	CCharacter::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CPlayer::Update(void)
{
	switch (CGame::GetState())
	{
	case CGame::STATE_START:	// �J�n���

		// �ړ�����
		Move();

		if (m_pAirplane != nullptr &&
			m_pAirplane->GetState() == CAirplane::STATE_MOVE)
		{ // ��s�@�� NULL ����Ȃ��ꍇ

			// �ʒu���s�@�ɍ��킹��
			SetPos(m_pAirplane->GetPos());

			// �X�V����
			m_pAirplane->Update(this);
		}

		if (m_pMotion != nullptr)
		{ // ���[�V������ NULL ����Ȃ��ꍇ

			// ���[�V�����̍X�V����
			m_pMotion->Update();
		}

		// �N���n�ʂƂ̓����蔻�菈��
		ElevationCollision();

		if (m_bJump == false)
		{ // �W�����v�󋵂� false �̏ꍇ

			// �v���C��Ԃɂ���
			CGame::SetState(CGame::STATE_PLAY);

			// �ʏ�J�����ɂ���
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_NONE);
		}

		break;

	case CGame::STATE_PLAY:			// �v���C���

		// �O��̈ʒu�̐ݒ菈��
		SetPosOld(GetPos());

		// ��ԊǗ�����
		StateManager();

		if (m_stateInfo.state == STATE_NONE ||
			m_stateInfo.state == STATE_INVINSIBLE)
		{ // �ʏ��ԁE���G��Ԃ̏ꍇ

			// ���쏈��
			m_pController->Control(this);
		}

		// �ړ�����
		Move();

		if (m_pMotion != nullptr)
		{ // ���[�V������ NULL ����Ȃ��ꍇ

			// ���[�V�����̍X�V����
			m_pMotion->Update();
		}

		if (m_pAction != nullptr)
		{ // �s���� NULL ����Ȃ��ꍇ

			// �s���̍X�V����
			m_pAction->Update(this);
		}

		for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
		{
			if (m_apHandGun[nCntGun] != nullptr)
			{ // ���e�� NULL ����Ȃ��ꍇ

				// �X�V����
				m_apHandGun[nCntGun]->Update();
			}
		}

		// �ً}�̃����[�h����
		EmergentReload();

		if (m_pAim != nullptr)
		{ // �G�C���� NULL ����Ȃ��ꍇ

			// �G�C���̍X�V����
			m_pAim->Update();
		}

		if (m_pLifeUI != nullptr)
		{ // ������ NULL ����Ȃ��ꍇ

			// ������ݒ肷��
			m_pLifeUI->SetLife(m_nLife);
		}

		// �敪�̐ݒ菈��
		m_nAreaIdx = area::SetFieldIdx(GetPos());

		// ���V�̎��Ƃ̓����蔻��
		collision::PalmFruitHit(this, COLLISION_SIZE.x, COLLISION_SIZE.y);

		// ���̍��Ƃ̓����蔻��
		collision::GoldBoneCollision(*this, COLLISION_SIZE);

		// �Ւd�̎��͂̓����蔻��
		collision::AlterSurrounding(GetPos(), COLLISION_SIZE.x);

		// �N���n�ʂƂ̓����蔻�菈��
		ElevationCollision();

		// �����蔻�菈��
		Collision();

		// �Ւd�Ƃ̓����蔻��
		AlterCollision();

		// �X�e�[�W�̓����蔻��
		StageCollision();

		CManager::Get()->GetDebugProc()->Print("�ʒu�F%f %f %f", GetPos().x, GetPos().y, GetPos().z);

		break;

	case CGame::STATE_BOSSMOVIE:	// �{�X�o�����

		// �N���n�ʂƂ̓����蔻�菈��
		ElevationCollision();

		break;

	case CGame::STATE_GAMEOVER:		// �Q�[���I�[�o�[���

		if (m_pMotion != nullptr)
		{ // ���[�V������ NULL ����Ȃ��ꍇ

			// ���[�V�����̍X�V����
			m_pMotion->Update();
		}

		// �N���n�ʂƂ̓����蔻�菈��
		ElevationCollision();

		break;

	case CGame::STATE_FINISH:		// �I�����

		if (m_pMotion != nullptr)
		{ // ���[�V������ NULL ����Ȃ��ꍇ

			// ���[�V�����̍X�V����
			m_pMotion->Update();
		}

		// �N���n�ʂƂ̓����蔻�菈��
		ElevationCollision();

		break;

	case CGame::STATE_CONTINUE:		// �R���e�B�j���[���

		// ���ɖ���

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �e�̈ʒu�ݒ菈��
	ShadowPosSet();
}

//===========================================
// �v���C���[�̕`�揈��
//===========================================
void CPlayer::Draw(void)
{
	if (m_pAirplane != nullptr)
	{ // ��s�@�� NULL ����Ȃ��ꍇ

		// ��s�@�̕`�揈��
		m_pAirplane->Draw();
	}

	switch (m_stateInfo.state)
	{
	case STATE_NONE:		// �ʏ���
	case STATE_DEATH:		// ���S���

		// �`�揈��
		CCharacter::Draw();

		break;

	case STATE_DAMAGE:		// �_���[�W���

		// �`�揈��
		CCharacter::Draw(m_stateInfo.col);

		break;

	case STATE_INVINSIBLE:	// ���G���

		// �`�揈��
		CCharacter::Draw(m_stateInfo.col.a);

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		if (m_apHandGun[nCntGun] != nullptr)
		{ // ���e�̏�� NULL ����Ȃ��ꍇ

			// ���e�̕`�揈��
			m_apHandGun[nCntGun]->Draw();
		}
	}

	if (m_pDagger != nullptr)
	{ // ���e�̏�� NULL ����Ȃ��ꍇ

		// ���e�̕`�揈��
		m_pDagger->Draw();
	}

	if (m_pAim != nullptr)
	{ // �G�C���̏�� NULL ����Ȃ��ꍇ

		// �G�C���̕`�揈��
		m_pAim->Draw();
	}
}

//===========================================
// �q�b�g����
//===========================================
void CPlayer::Hit(const int nDamage, const float fRotSmash)
{
	// �̗͂����炷
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{ // �̗͂�0�ȉ��ɂȂ����ꍇ

		// �̗͂�0�ɂ���
		m_nLife = 0;

		// ���S��Ԃɂ���
		m_stateInfo.state = STATE_DEATH;

		// �ړ��ʂ�0�ɂ���
		m_move = NONE_D3DXVECTOR3;

		if (m_pMotion->GetType() != MOTIONTYPE_DEATH)
		{ // ���S���[�V��������Ȃ��ꍇ

			// ���S���[�V�������s��
			m_pMotion->Set(MOTIONTYPE_DEATH);
		}

		if (CManager::Get()->GetMode() == CScene::MODE_GAME)
		{ // �Q�[�����[�h�̏ꍇ

			// �Q�[���I�[�o�[�ɂ���
			CGame::SetState(CGame::STATE_GAMEOVER);

			// �Q�[���I�[�o�[�J�����ɂ���
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_GAMEOVER);
		}
	}
	else
	{ // ��L�ȊO

		// �_���[�W����炷
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_PLAYERDAMAGE);

		// �_���[�W��Ԃɂ���
		m_stateInfo.state = STATE_DAMAGE;

		if (m_pAction->GetAction() != CPlayerAction::ACTION_RELOAD)
		{ // �����[�h��ԈȊO

			// �_�K�[��\�����Ȃ�
			m_pDagger->SetEnableDisp(false);
			m_pDagger->SetEnableDispOrbit(false);

			// ���e��`�悷��
			m_apHandGun[0]->SetEnableDisp(true);
			m_apHandGun[1]->SetEnableDisp(true);

			// �ʏ�s���ɂ���
			m_pAction->SetAction(CPlayerAction::ACTION_NONE);
		}

		// �J�E���g��0�ɂ���
		m_stateInfo.nCount = 0;

		// �ړ��ʂ�ݒ肷��
		m_move.x = sinf(fRotSmash) * KNOCKBACK_MOVE;
		m_move.y = KNOCKBACK_JUMP;
		m_move.z = cosf(fRotSmash) * KNOCKBACK_MOVE;

		if (m_pMotion->GetType() != MOTIONTYPE_DAMAGE)
		{ // �_���[�W���[�V��������Ȃ������ꍇ

			// �_���[�W���[�V������ݒ�
			m_pMotion->Set(MOTIONTYPE_DAMAGE);
		}

		// �_���[�W����炷
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
	}
}

//===========================================
// �񕜏���
//===========================================
void CPlayer::Healing(const int nHealing)
{
	// �̗͂��񕜂���
	m_nLife += nHealing;

	if (m_nLife >= MAX_LIFE)
	{ // �̗͂��ő�l�ȏ�ɂȂ����ꍇ

		// �̗͂��ő�l�ɂ���
		m_nLife = MAX_LIFE;
	}
}

//===========================================
// ���[�V�����̏��̐ݒ菈��
//===========================================
CMotion* CPlayer::GetMotion(void) const
{
	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}

//===========================================
// �s���̏��̎擾����
//===========================================
CPlayerAction* CPlayer::GetAction(void) const
{
	// �s���̏���Ԃ�
	return m_pAction;
}

//===========================================
// ���e�̏��̎擾����
//===========================================
CHandgun* CPlayer::GetHandGun(const int nCount) const
{
	// ���e�̏���Ԃ�
	return m_apHandGun[nCount];
}

//===========================================
// �_�K�[�̏��̎擾����
//===========================================
CDagger* CPlayer::GetDagger(void) const
{
	// �_�K�[�̏���Ԃ�
	return m_pDagger;
}

//===========================================
// �G�C���̎擾����
//===========================================
CAim* CPlayer::GetAim(void) const
{
	// �G�C���̏���Ԃ�
	return m_pAim;
}

//===========================================
// �c�eUI�̏��̎擾����
//===========================================
CBulletUI* CPlayer::GetBulletUI(void) const
{
	// �c�eUI�̏���Ԃ�
	return m_pBulletUI;
}

//===========================================
// ���̍�UI�̏��̎擾����
//===========================================
CGoldBoneUI* CPlayer::GetGoldBoneUI(void) const
{
	// ���̍�UI�̏���Ԃ�
	return m_pGoldBoneUI;
}

//=======================================
// ����UI�̏��̎擾����
//=======================================
CLifeUI* CPlayer::GetLifeUI(void) const
{
	// ����UI�̏���Ԃ�
	return m_pLifeUI;
}

//=======================================
// �R���g���[���[�̏��̎擾����
//=======================================
CPlayerController* CPlayer::GetController(void) const
{
	// �R���g���[���[����Ԃ�
	return m_pController;
}

//=======================================
// ��s�@�̏��̎擾����
//=======================================
CAirplane* CPlayer::GetAirplane(void) const
{
	// ��s�@�̏���Ԃ�
	return m_pAirplane;
}

//=======================================
// ��s�@��NULL������
//=======================================
void CPlayer::DeleteAirplane()
{
	// ��s�@�� NULL �ɂ���
	m_pAirplane = nullptr;
}

//=======================================
// ��s�@�̓�������
//=======================================
void CPlayer::ArrivalAirplane(void)
{
	if (m_pAirplane != nullptr)
	{ // ��s�@�� NULL ����Ȃ��ꍇ

		// ��s�@�̈ʒu���擾
		D3DXVECTOR3 pos = m_pAirplane->GetPos();

		// �ړI�̈ʒu�ɕ␳����
		pos.x = m_pAirplane->GetPosDest().x;
		pos.z = m_pAirplane->GetPosDest().z;

		// �ʒu��K�p
		m_pAirplane->SetPos(pos);

		// �v���C���[�o���Ԃɂ���
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_PLAYERAPPEAR);

		// �J�����̎��_��ݒ肷��
		CManager::Get()->GetCamera()->SetPosV(pos + AIRPLANE_ARRIVAL_POSV);

		// �ʒu��ݒ肷��
		SetPos(pos);

		// �ړ��ʂ̐ݒ菈��
		SetMove(AIRPLANE_ARRIVAL_MOVE);

		// �ė���Ԃɂ���
		m_pAirplane->SetState(CAirplane::STATE_FALL);

		// ��ނ��s�@�ɕς���
		m_pAirplane->SetType(TYPE_AIRPLANE);

		// ��s�@�� NULL �ɂ���
		m_pAirplane = nullptr;

		if (m_pMotion->GetType() != MOTIONTYPE_AIRDIVE)
		{ // ��s�@�~���ԈȊO�̏ꍇ

			// ��s�@�~�胂�[�V�������s��
			m_pMotion->Set(MOTIONTYPE_AIRDIVE);
		}
	}
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CPlayer::SetData(const D3DXVECTOR3& pos)
{
	// �S�Ă̒l������������
	SetPos(pos);					// �ʒu
	SetPosOld(GetPos());			// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// ����������
		GetHierarchy(nCntData)->SetPos(pos);					// �ʒu
		GetHierarchy(nCntData)->SetPosOld(pos);					// �O��̈ʒu
		GetHierarchy(nCntData)->SetRot(NONE_D3DXVECTOR3);		// ����
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);			// �g�嗦
	}

	// ���[�V�����̐ݒ菈��
	m_pMotion->ResetMotion(MOTIONTYPE_NEUTRAL);

	for (int nCnt = 0; nCnt < NUM_HANDGUN; nCnt++)
	{
		if (m_apHandGun[nCnt] == nullptr)
		{ // ���e�� NULL �̏ꍇ

			// ���e�̏��𐶐�����
			m_apHandGun[nCnt] = CHandgun::Create(GUN_POS[nCnt], GUN_ROT[nCnt], GetHierarchy(INIT_GUN_IDX + nCnt)->GetMatrixP());
		}
	}

	if (m_pDagger == nullptr)
	{ // �_�K�[�� NULL �̏ꍇ

		// �_�K�[�𐶐�����
		m_pDagger = CDagger::Create(GetHierarchy(INIT_DAGGER_IDX)->GetMatrixP());
	}

	if (m_pAim == nullptr)
	{ // �G�C���� NULL �̏ꍇ

		// �G�C���𐶐�����
		m_pAim = CAim::Create(GetPos());
	}

	if (m_pAirplane == nullptr)
	{ // ��s�@�� NULL �̏ꍇ

		// ��s�@�𐶐�
		m_pAirplane = CAirplane::Create(pos);
	}

	// �ʒu���s�@�ɍ��킹��
	SetPos(m_pAirplane->GetPos());
	
	// �S�Ă̒l��ݒ肷��
	m_stateInfo.state = STATE_NONE;		// ���
	m_stateInfo.nCount = 0;				// ��ԃJ�E���g
	m_stateInfo.col = NONE_D3DXCOLOR;	// �̂̐F
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_move = NONE_D3DXVECTOR3;			// �ړ���
	m_nLife = MAX_LIFE;					// �̗�
	m_bMove = false;					// �ړ���
	m_bJump = false;					// �W�����v��

	// �敪�̐ݒ菈��
	m_nAreaIdx = area::SetFieldIdx(GetPos());

	if (m_pShadow == nullptr)
	{ // �ۉe�� NULL �̏ꍇ

		// �ۉe�𐶐�
		m_pShadow = CShadowCircle::Create(GetPos(), SHADOW_RADIUS, m_nAreaIdx);
	}
}

//===========================================
// ��������
//===========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos)
{
	// �v���C���[�̃C���X�^���X�𐶐�
	CPlayer* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���������m�ۂ���
		pPlayer = new CPlayer;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPlayer->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pPlayer;
}

//=======================================
// �ړI�̌����̐ݒ菈��
//=======================================
void CPlayer::SetRotDest(const D3DXVECTOR3& rot)
{
	// �ړI�̌�����ݒ肷��
	m_rotDest = rot;
}

//=======================================
// �ړI�̌����̎擾����
//=======================================
D3DXVECTOR3 CPlayer::GetRotDest(void) const
{
	// �ړI�̌�����Ԃ�
	return m_rotDest;
}

//=======================================
// �ړ��ʂ̐ݒ菈��
//=======================================
void CPlayer::SetMove(const D3DXVECTOR3& move)
{
	// �ړ��ʂ�ݒ肷��
	m_move = move;
}

//=======================================
// �ړ��ʂ̎擾����
//=======================================
D3DXVECTOR3 CPlayer::GetMove(void) const
{
	// �ړ��ʂ�Ԃ�
	return m_move;
}

//=======================================
// �����̐ݒ菈��
//======================================
void CPlayer::SetLife(const int nLife)
{
	// ���D��
	m_nLife = nLife;
}

//======================================
// �̗͂̎擾����
//======================================
int CPlayer::GetLife(void) const
{
	// �̗͂�Ԃ�
	return m_nLife;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CPlayer::SetState(const SState state)
{
	// ��Ԃ�ݒ肷��
	m_stateInfo = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CPlayer::SState CPlayer::GetState(void) const
{
	// ��Ԋ֘A�̏���Ԃ�
	return m_stateInfo;
}

//=======================================
// �W�����v�󋵂̐ݒ菈��
//=======================================
void CPlayer::SetEnableJump(const bool bJump)
{
	// �W�����v�󋵂�ݒ肷��
	m_bJump = bJump;
}

//=======================================
// �W�����v�󋵂̎擾����
//=======================================
bool CPlayer::IsJump(void) const
{
	//�W�����v�󋵂�Ԃ�
	return m_bJump;
}

//=======================================
// �敪�̔ԍ��̐ݒ菈��
//=======================================
void CPlayer::SetAreaIdx(const int nIdx)
{
	// �敪�̔ԍ���ݒ肷��
	m_nAreaIdx = nIdx;
}

//=======================================
// �敪�̔ԍ��̎擾����
//=======================================
int CPlayer::GetAreaIdx(void) const
{
	// �敪�̔ԍ���Ԃ�
	return m_nAreaIdx;
}

//=======================================
// �N���n�ʂ̓����蔻�菈��
//=======================================
void CPlayer::ElevationCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	float fHeight = 0.0f;			// ����
	bool bJump = true;				// �W�����v��
	bool bRange = false;			// �͈͓���
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// �擪�̏���
	CElevation* pElevEnd = nullptr;		// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pElev = list.GetTop();

		// �����̒l���擾����
		pElevEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �����蔻������
			fHeight = pElev->ElevationCollision(pos, &bRange);

			if (pos.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				pos.y = fHeight;

				// �d�͂�ݒ肷��
				m_move.y = LAND_GRAVITY;

				if (m_bJump == true)
				{ // �W�����v���̏ꍇ

					// ���n����炷
					CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_LANDING);
				}

				// �W�����v�󋵂� false �ɂ���
				bJump = false;
			}

			if (pElev == pElevEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pElev = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �W�����v�󋵂�������
	m_bJump = bJump;

	// �ʒu���X�V����
	SetPos(pos);
}

//=======================================
// �����蔻�菈��
//=======================================
void CPlayer::Collision(void)
{
	D3DXVECTOR3 pos = GetPos();									// �ʒu
	D3DXVECTOR3 posOld = GetPosOld();							// �O��̈ʒu
	D3DXVECTOR3 vtxMin = useful::VtxMinConv(COLLISION_SIZE);	// ���_�̍ŏ��l
	int nIdx = 0;

	for (int nCnt = 0; nCnt < area::NUM_COLL; nCnt++)
	{
		nIdx = m_nAreaIdx + area::COLL_ADD_IDX[nCnt];

		if (area::IndexCheck(nIdx) == true)
		{ // �敪���̏ꍇ

			// �����Ƃ̓����蔻��
			collision::CoinCollision(this, COLLISION_SIZE, nIdx);

			// �؂Ƃ̓����蔻��
			collision::TreeCollision(&pos, COLLISION_SIZE.x, nIdx);

			// �u���b�N�Ƃ̓����蔻��
			BlockCollision(&pos, posOld, COLLISION_SIZE, vtxMin, nIdx);

			// ��Ƃ̓����蔻��
			collision::RockCollision(&pos, posOld, COLLISION_SIZE.x, COLLISION_SIZE.y, nIdx, &m_move.y, &m_bJump);

			// ��Ƃ̓����蔻��
			collision::WallCollision(&pos, posOld, COLLISION_SIZE, vtxMin, nIdx);
		}
	}

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// ��ԊǗ�����
//=======================================
void CPlayer::StateManager(void)
{
	switch (m_stateInfo.state)
	{
	case CPlayer::STATE_NONE:

		if (m_pAction->GetAction() != CPlayerAction::ACTION_DODGE)
		{ // �����ԈȊO

			// �����蔻��
			if (collision::EnemyHitToPlayer(this, COLLISION_SIZE.x, COLLISION_SIZE.y) == true ||
				collision::ExplosionHitToPlayer(this, COLLISION_SIZE.x, COLLISION_SIZE.y) == true)
			{ // �����ɓ��������ꍇ

				// ���̐�̏������s��Ȃ�
				return;
			}

			// ������Ԍ���
			float fRotSmash = 0.0f;
			D3DXVECTOR3 pos = GetPos();

			if (collision::WindShotHit(pos, COLLISION_SIZE.x, COLLISION_SIZE.y, &fRotSmash) == true)
			{ // ���U���ɓ��������ꍇ

				// �q�b�g����
				Hit(WIND_SHOT_DAMAGE, fRotSmash);
			}
			else if (collision::FireShotHit(pos, COLLISION_SIZE.x, COLLISION_SIZE.y, &fRotSmash) == true)
			{ // ���U���ɓ��������ꍇ

				// �q�b�g����
				Hit(FIRE_SHOT_DAMAGE, fRotSmash);
			}
			else if (collision::BossAttack(D3DXVECTOR3(pos.x, pos.y + (COLLISION_SIZE.y * 0.5f), pos.z), COLLISION_SIZE.x, &fRotSmash) == true)
			{ // �{�X�̓ːi�ɓ��������ꍇ

				// �q�b�g����
				Hit(BOSS_RUSH_DAMAGE, fRotSmash);
			}
		}

		break;

	case CPlayer::STATE_DAMAGE:

		// �_���[�W��ԏ���
		DamageState();


		break;

	case CPlayer::STATE_INVINSIBLE:

		// ���G��ԏ���
		InvisibleState();

		break;

	case CPlayer::STATE_DEATH:

		if (m_pMotion->GetType() != MOTIONTYPE_DEATH)
		{ // ���S���[�V��������Ȃ��ꍇ

			// ���S���[�V�������s��
			m_pMotion->Set(MOTIONTYPE_DEATH);
		}

		if (CManager::Get()->GetMode() == CScene::MODE_GAME)
		{ // �Q�[�����[�h�̏ꍇ

			// �Q�[���I�[�o�[�ɂ���
			CGame::SetState(CGame::STATE_GAMEOVER);

			// �Q�[���I�[�o�[�J�����ɂ���
			CManager::Get()->GetCamera()->SetType(CCamera::TYPE_GAMEOVER);
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//=======================================
// �_���[�W��ԏ���
//=======================================
void CPlayer::DamageState()
{
	// ��ԃJ�E���g�����Z����
	m_stateInfo.nCount++;

	// �̂̐F��ς���
	m_stateInfo.col = DAMAGE_COLOR;

	if (m_stateInfo.nCount >= DAMAGE_COUNT)
	{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		m_stateInfo.nCount = 0;

		// �̂̐F�����ɖ߂�
		m_stateInfo.col = NONE_D3DXCOLOR;

		// ���G��Ԃɂ���
		m_stateInfo.state = STATE_INVINSIBLE;
	}
}

//=======================================
// ���G��ԏ���
//=======================================
void CPlayer::InvisibleState()
{
	// ��ԃJ�E���g�����Z����
	m_stateInfo.nCount++;

	if (m_stateInfo.nCount % INVINCIBLE_FLUSH_COUNT == 0)
	{ // �J�E���g����萔�ȏ�ɂȂ����ꍇ

		// �F��ς���
		m_stateInfo.col.a = (m_stateInfo.col.a >= 1.0f) ? 0.0f : 1.0f;
	}

	if (m_stateInfo.nCount >= INVINCIBLE_COUNT)
	{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

		// ��ԃJ�E���g��0�ɂ���
		m_stateInfo.nCount = 0;

		// �ʏ��Ԃɂ���
		m_stateInfo.state = STATE_NONE;
	}
}

//=======================================
// �ړ�����
//=======================================
void CPlayer::Move(void)
{
	// �ʒu�ƌ������擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	// �����̕␳����
	useful::RotCorrect(m_rotDest.y, &rot.y, ROT_CORRECT);

	if (m_pAction->IsRecoil() == false)
	{ // �����󋵂� false �̏ꍇ

		// �ʒu���ړ�������
		pos.x += m_move.x;
		pos.z += m_move.z;
	}

	// �d�͂�������
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// �ʒu�ƌ�����K�p����
	SetPos(pos);
	SetRot(rot);
}

//=======================================
// �ً}�̃����[�h����
//=======================================
void CPlayer::EmergentReload(void)
{
	if (m_pBulletUI != nullptr &&
		m_pBulletUI->GetNumBullet() <= 0 &&
		m_stateInfo.state == STATE_NONE)
	{ // �ʏ��ԂŒe�������[�h�ł��ĂȂ������ꍇ

		// �ً}�Œe�ۂ������[�h����
		m_pBulletUI->SetNumBullet(MAX_REMAINING_BULLET);
	}
}

//=======================================
// �e�̈ʒu�ݒ菈��
//=======================================
void CPlayer::ShadowPosSet(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// �e�̈ʒu�Ƌ敪�̔ԍ���ݒ肷��
	m_pShadow->SetPos(pos);
	m_pShadow->SetAreaIdx(m_nAreaIdx);

	if (m_bJump == true)
	{ // �W�����v���Ă���ꍇ

		// �������ŉ��w�ɂ���
		pos.y = FLT_MIN;

		// �e�̓����蔻��
		m_pShadow->Collision();
	}
}

//=======================================
// �u���b�N�Ƃ̓����蔻��
//=======================================
void CPlayer::BlockCollision(D3DXVECTOR3* pos, const D3DXVECTOR3& posOld, const D3DXVECTOR3& vtxMax, const D3DXVECTOR3& vtxMin, const int nAreaIdx)
{
	// ���[�J���ϐ��錾
	collision::SCollision coll = { false,false,false,false,false,false };				// �����蔻��̕ϐ�
	CListManager<CBlock*> list = CBlock::GetList(nAreaIdx);
	CBlock* pBlock = nullptr;		// �擪�̒l
	CBlock* pBlockEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pBlock = list.GetTop();

		// �����̒l���擾����
		pBlockEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �Z�ʑ̂̓����蔻��
			coll = collision::HexahedronClush
			(
				pos,
				pBlock->GetPos(),
				posOld,
				pBlock->GetPosOld(),
				vtxMin,
				pBlock->GetVtxMin(),
				vtxMax,
				pBlock->GetVtxMax()
			);

			if (coll.bTop == true)
			{ // ��ɏ�����ꍇ

				// �ړ��ʂ�ݒ肷��
				m_move.y = 0.0f;

				// �W�����v���Ă��Ȃ�
				m_bJump = false;
			}

			if (pBlock == pBlockEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pBlock = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}
}

//=======================================
// �Ւd�Ƃ̓����蔻��
//=======================================
void CPlayer::AlterCollision(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �Ւd�Ƃ̓����蔻��
	collision::AlterCollision
	(
		&pos,
		GetPosOld(),
		COLLISION_SIZE,
		D3DXVECTOR3(-COLLISION_SIZE.x, 0.0f, -COLLISION_SIZE.z)
	);

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//=======================================
// �X�e�[�W�Ƃ̓����蔻��
//=======================================
void CPlayer::StageCollision(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// �X�e�[�W�̓����蔻��
	collision::StageCollision(&pos, COLLISION_SIZE.x);

	// �ʒu��K�p
	SetPos(pos);
}