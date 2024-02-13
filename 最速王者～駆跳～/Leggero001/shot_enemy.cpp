//============================================
//
// �e�����G�̃��C������[shotenemy.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"
#include "shot_enemy.h"
#include "manager.h"
#include "game.h"
#include "collision.h"
#include "renderer.h"
#include "motion.h"
#include "useful.h"

#include "bullet.h"
#include "player.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SHOTENEMY_SHOT_COUNT		(60)		// �G���e��łC���^�[�o��
#define SHOTENEMY_ACT_CHANGE_ROT	(0.01f)		// �G���s����Ԃ�ς����ƂȂ����
#define SHOTENEMY_ROT_MAGNI			(0.5f)		// �G��������ς���{��

//===========================================
// �R���X�g���N�^
//===========================================
CShotEnemy::CShotEnemy() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_action = ACT_SHOT;		// �s�����
	m_nShotCount = 0;			// ���˃J�E���g
	m_fGravity = 0.0f;			// �d��
	m_fRotDest = 0.0f;			// �ړI�̌���
	m_fRotDiff = 0.0f;			// �����̍���
}

//===========================================
// �f�X�g���N�^
//===========================================
CShotEnemy::~CShotEnemy()
{

}

//===========================================
// ����������
//===========================================
HRESULT CShotEnemy::Init(void)
{
	// �S�Ă̒l���N���A����
	m_action = ACT_SHOT;		// �s�����
	m_nShotCount = 0;			// ���˃J�E���g
	m_fGravity = 0.0f;			// �d��
	m_fRotDest = 0.0f;			// �ړI�̌���
	m_fRotDiff = 0.0f;			// �����̍���

	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �x�^�ł�
	SetNumModel(14);

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();

	if (GetMotion() == nullptr)
	{ // ���[�V������ NULL �������ꍇ

		// ���[�V�����̐ݒ菈��
		SetMotion();
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �x����
		MessageBox(NULL, "�ˌ��G�̃��[�V�����̃|�C���^���g���Ă��܂��I", "�x���I", MB_ICONWARNING);
	}

	if (GetMotion() != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�V�����̏����擾����
		GetMotion()->SetModel(GetHierarchy(), GetNumModel());

		// ���[�h����
		GetMotion()->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �x����
		MessageBox(NULL, "�ˌ��G�̃��[�V�����̃��������m�ۂł��܂���ł����I", "�x���I", MB_ICONWARNING);
	}

	// ���[�V�����̐ݒ菈��
	GetMotion()->Set(MOTIONTYPE_NEUTRAL);

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CShotEnemy::Uninit(void)
{
	// �I��
	CEnemy::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CShotEnemy::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	// �U���������
	Turn();

	switch (m_action)
	{
	case ACT_SHOT:

		// �e������Ԃł̏���
		ActionShot();

		break;

	case ACT_TURN:

		// �U�������Ԃł̏���
		ActionTurn();

		break;
	}

	// �d�͏���
	Gravity(&m_fGravity, pos, ENEMY_GRAVITY);

	// �ʒu��ݒ肷��
	SetPos(pos);

	if (collision::EnemyBlockCollision(this) == true)
	{ // �u���b�N�Ƃ̓����蔻�肪 true �̏ꍇ

		// �d�͂� 0.0f �ɂ���
		m_fGravity = 0.0f;
	}

	// ���[�V�����̍X�V����
	GetMotion()->Update();
}

//===========================================
// �`�揈��
//===========================================
void CShotEnemy::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CShotEnemy::SetData(const D3DXVECTOR3& pos)
{
	// �S�Ă̒l������������
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));			// ����
	SetScale(NONE_SCALE);	// �g�嗦

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// ����������
		GetHierarchy(nCntData)->SetPos(pos);										// �ʒu
		GetHierarchy(nCntData)->SetPosOld(pos);										// �O��̈ʒu
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// ����
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// �g�嗦
		GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(nCntData));	// �f�[�^�̐ݒ菈��
	}
}

//===========================================
// �����̌����̐ݒ菈��
//===========================================
void CShotEnemy::InitRot(void)
{
	// ���[�J���|�C���^�錾
	CPlayer* pPlayer = CScene::GetPlayer();		// �v���C���[�̏����擾����

	if (pPlayer != nullptr)
	{ // �v���C���[�̏�񂪂���ꍇ

		if (pPlayer != nullptr)
		{ // �v���C���[�� NULL ����Ȃ��ꍇ

			if (pPlayer->GetPos().x > GetPos().x)
			{ // �v���C���[���G���E�ɂ����ꍇ

				// �ړI�̌�����ݒ肷��
				m_fRotDest = -D3DX_PI * 0.5f;

				// �U�������Ԃɂ���
				m_action = ACT_TURN;
			}
			else
			{ // �v���C���[���G��荶�ɂ����ꍇ

				// �ړI�̌�����ݒ肷��
				m_fRotDest = D3DX_PI * 0.5f;

				// �U�������Ԃɂ���
				m_action = ACT_TURN;
			}
		}
	}
	else
	{ // �v���C���[�̏�񂪂Ȃ��ꍇ

		// �����̐ݒ菈��
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
}

//===========================================
// �ˌ�����
//===========================================
void CShotEnemy::Shot(void)
{
	// ���˃J�E���g�����Z����
	m_nShotCount++;

	if (m_nShotCount % SHOTENEMY_SHOT_COUNT == 0)
	{ // ���˃J�E���g����萔�ɂȂ����ꍇ

		// �e�̐�������
		CBullet::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 50.0f, 0.0f), BULLET_SIZE, -GetRot().y, CBullet::TYPE_ENEMY);
	}
}

//===========================================
// �U���������
//===========================================
void CShotEnemy::Turn(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// ���[�J���|�C���^�錾
	CPlayer* pPlayer = CScene::GetPlayer();		// �v���C���[�̏����擾����

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		if (pPlayer->GetPosOld().x < pos.x &&
			pPlayer->GetPos().x > pos.x)
		{ // �v���C���[���G���E�Ɉړ������ꍇ

			// �ړI�̌�����ݒ肷��
			m_fRotDest = -D3DX_PI * 0.5f;

			// �U�������Ԃɂ���
			m_action = ACT_TURN;
		}
		else if(pPlayer->GetPosOld().x > pos.x &&
			pPlayer->GetPos().x < pos.x)
		{ // �v���C���[���G��荶�Ɉړ������ꍇ

			// �ړI�̌�����ݒ肷��
			m_fRotDest = D3DX_PI * 0.5f;

			// �U�������Ԃɂ���
			m_action = ACT_TURN;
		}
	}
}

//===========================================
// �e������Ԃł̏���
//===========================================
void CShotEnemy::ActionShot(void)
{
	// �ˌ�����
	Shot();
}

//===========================================
// �U�������Ԃł̏���
//===========================================
void CShotEnemy::ActionTurn(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �����̍������v�Z����
	m_fRotDiff = m_fRotDest - rot.y;

	// �����ɍ��������Z����
	rot.y += m_fRotDiff * SHOTENEMY_ROT_MAGNI;

	// �����̐��K��
	RotNormalize(&rot.y);

	if (rot.y >= m_fRotDest - SHOTENEMY_ACT_CHANGE_ROT &&
		rot.y <= m_fRotDest + SHOTENEMY_ACT_CHANGE_ROT)
	{ // ������������x�������ꍇ

		// ������ݒ肷��
		rot.y = m_fRotDest;

		// �e������Ԃɂ���
		m_action = ACT_SHOT;

		// �e�����J�E���g������������
		m_nShotCount = 0;
	}

	// �������X�V����
	SetRot(rot);
}