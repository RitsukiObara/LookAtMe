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
#include "useful.h"

#include "player.h"
#include "player_action.h"
#include "handgun.h"
#include "dagger.h"
#include "aim.h"
#include "bulletUI.h"
#include "gold_bone_UI.h"
#include "player_controller.h"

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
	const float INIT_POSV_CAMERA_Y = 250.0f;		// �J�����̎��_��Y���W
	const float ROT_CORRECT = 0.2f;					// �����̕␳�{��
	const D3DXVECTOR3 COLLISION_SIZE = D3DXVECTOR3(40.0f, 190.0f, 40.0f);		// �����蔻�莞�̃T�C�Y
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
	const int DAMAGE_COUNT = 7;						// �_���[�W��Ԃ̃J�E���g��
	const D3DXCOLOR DAMAGE_COLOR = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// �_���[�W��Ԃ̐F
	const int INVINCIBLE_COUNT = 90;				// ���G��Ԃ̃J�E���g��
	const int INVINCIBLE_FLUSH_COUNT = 10;			// ���G��Ԃ̓_�ł̃J�E���g
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
	m_pController = nullptr;				// �v���C���[�̃R���g���[���[�̏��

	m_stateInfo.state = STATE_NONE;			// ���
	m_stateInfo.nCount = 0;					// ��ԃJ�E���g
	m_stateInfo.col = NONE_D3DXCOLOR;		// �̂̐F
	m_rotDest = NONE_D3DXVECTOR3;			// �ڕW�̌���
	m_move = NONE_D3DXVECTOR3;				// �ړ���
	m_nLife = MAX_LIFE;						// �̗�
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

	// �S�Ă̒l������������
	for (int nCntGun = 0; nCntGun < NUM_HANDGUN; nCntGun++)
	{
		m_apHandGun[nCntGun] = nullptr;	// ���e�̏��
	}
	m_pAim = nullptr;					// �G�C���̏��
	m_pDagger = nullptr;				// �_�K�[�̏��

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

	if (m_pController != nullptr)
	{ // �R���g���[���[�̏�� NULL ����Ȃ��ꍇ

		// �I������
		m_pController->Uninit();
		m_pController = nullptr;
	}

	// �I������
	CCharacter::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CPlayer::Update(void)
{
	// �O��̈ʒu�̐ݒ菈��
	SetPosOld(GetPos());

	// ��ԊǗ�����
	StateManager();

	// ���쏈��
	m_pController->Control(this);

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

	if (m_pAim != nullptr)
	{ // �G�C���� NULL ����Ȃ��ꍇ

		// �G�C���̍X�V����
		m_pAim->Update();
	}

	// �����Ƃ̓����蔻��
	collision::CoinCollision(GetPos(), COLLISION_SIZE);

	// �؂Ƃ̓����蔻��
	TreeCollision();

	// �N���n�ʂƂ̓����蔻�菈��
	ElevationCollision();

	// �u���b�N�Ƃ̓����蔻�菈��
	BlockCollision();

	// ��Ƃ̓����蔻��
	RockCollision();

	// ���̍��Ƃ̓����蔻��
	collision::GoldBoneCollision(*this, COLLISION_SIZE);

	CManager::Get()->GetDebugProc()->Print("�ʒu�F%f %f %f", GetPos().x, GetPos().y, GetPos().z);
}

//===========================================
// �v���C���[�̕`�揈��
//===========================================
void CPlayer::Draw(void)
{
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
void CPlayer::Hit(const int nDamage)
{
	// �̗͂����炷
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{ // �̗͂�0�ȉ��ɂȂ����ꍇ

		// �̗͂�0�ɂ���
		m_nLife = 0;

		// ���S��Ԃɂ���
		m_stateInfo.state = STATE_DEATH;
	}
	else
	{ // ��L�ȊO

		// �_���[�W��Ԃɂ���
		m_stateInfo.state = STATE_DAMAGE;

		// �J�E���g��0�ɂ���
		m_stateInfo.nCount = 0;
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
		// ���e�̏��𐶐�����
		m_apHandGun[nCnt] = CHandgun::Create(GUN_POS[nCnt], GUN_ROT[nCnt], GetHierarchy(INIT_GUN_IDX + nCnt)->GetMatrixP());
	}

	// �_�K�[�𐶐�����
	m_pDagger = CDagger::Create(GetHierarchy(INIT_DAGGER_IDX)->GetMatrixP());

	// �G�C���𐶐�����
	m_pAim = CAim::Create(GetPos());
	
	// �S�Ă̒l��ݒ肷��
	m_stateInfo.state = STATE_NONE;		// ���
	m_stateInfo.nCount = 0;				// ��ԃJ�E���g
	m_stateInfo.col = NONE_D3DXCOLOR;	// �̂̐F
	m_rotDest = NONE_D3DXVECTOR3;		// �ڕW�̌���
	m_move = NONE_D3DXVECTOR3;			// �ړ���
	m_nLife = MAX_LIFE;					// �̗�
	m_bMove = false;					// �ړ���
	m_bJump = false;					// �W�����v��
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
// ��ԊǗ�����
//=======================================
void CPlayer::StateManager(void)
{
	switch (m_stateInfo.state)
	{
	case CPlayer::STATE_NONE:


		break;

	case CPlayer::STATE_DAMAGE:

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

		break;

	case CPlayer::STATE_INVINSIBLE:

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

		break;

	case CPlayer::STATE_DEATH:


		break;

	default:

		// ��~
		assert(false);

		break;
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
// �؂Ƃ̓����蔻��
//=======================================
void CPlayer::TreeCollision(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �؂Ƃ̓����蔻��
	collision::TreeCollision(&pos, COLLISION_SIZE.x);

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �u���b�N�Ƃ̓����蔻��
//=======================================
void CPlayer::BlockCollision(void)
{
	// ���[�J���ϐ��錾
	collision::SCollision coll = { false,false,false,false,false,false };				// �����蔻��̕ϐ�
	D3DXVECTOR3 pos = GetPos();							// �ʒu���擾����
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-COLLISION_SIZE.x, 0.0f, -COLLISION_SIZE.z);		// �ŏ��l���擾����
	D3DXVECTOR3 vtxMax = COLLISION_SIZE;				// �ő�l���擾����
	CListManager<CBlock*> list = CBlock::GetList();
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
				&pos,
				pBlock->GetPos(),
				GetPosOld(),
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

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// ��Ƃ̓����蔻��
//=======================================
void CPlayer::RockCollision(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// ��Ƃ̓����蔻��
	collision::RockCollision(&pos, COLLISION_SIZE.x, COLLISION_SIZE.y);

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}