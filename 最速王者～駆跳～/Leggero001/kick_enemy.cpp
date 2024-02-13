//============================================
//
// ��Q���R��G�̃��C������[kick_enemy.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"
#include "kick_enemy.h"
#include "manager.h"
#include "game.h"
#include "collision.h"
#include "motion.h"
#include "renderer.h"
#include "useful.h"
#include "debugproc.h"

#include "drum.h"
#include "player.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define WAITING_COUNT		(120)		// �ҋ@��Ԃ̃J�E���g

//===========================================
// �R���X�g���N�^
//===========================================
CKickEnemy::CKickEnemy() : CEnemy(CObject::TYPE_ENEMY, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pDrum = nullptr;								// ��Q���̃|�C���^
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��Q���̖ړI�̈ʒu
	m_drumMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��Q���̈ړ���
	m_action = ACT_WAITING;							// �s�����
	m_nStateCount = 0;								// ��ԃJ�E���g
	m_fGravity = 0.0f;								// �d��
	m_bRight = false;								// �E������
}

//===========================================
// �f�X�g���N�^
//===========================================
CKickEnemy::~CKickEnemy()
{

}

//===========================================
// ����������
//===========================================
HRESULT CKickEnemy::Init(void)
{
	// �S�Ă̒l���N���A����
	m_pDrum = nullptr;								// ��Q���̃|�C���^
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��Q���̖ړI�̈ʒu
	m_drumMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��Q���̈ړ���
	m_action = ACT_WAITING;							// �s�����
	m_nStateCount = 0;								// ��ԃJ�E���g
	m_fGravity = 0.0f;								// �d��
	m_bRight = false;								// �E������

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
		MessageBox(NULL, "��Q���L�b�N�G�̃��[�V�����̃|�C���^���g���Ă��܂��I", "�x���I", MB_ICONWARNING);
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
		MessageBox(NULL, "��Q���L�b�N�G�̃��[�V�����̃��������m�ۂł��܂���ł����I", "�x���I", MB_ICONWARNING);
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CKickEnemy::Uninit(void)
{
	// �I��
	CEnemy::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CKickEnemy::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	// ��Q���̑{������
	if (SearchObstacle() == false)
	{ // �|�C���^����v���Ȃ������ꍇ

		// �|�C���^�� NULL �ɂ���
		m_pDrum = nullptr;
	}

	switch (m_action)
	{
	case ACT_WAITING:		// �ҋ@���

		// �U���������
		Turn();

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % WAITING_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ��ԃJ�E���g������������
			m_nStateCount = 0;

			// �\����Ԃɂ���
			m_action = ACT_BRING;

			// ��Q���̖ړI�̌�����ݒ肷��
			m_posDest = D3DXVECTOR3
			(
				pos.x + sinf(GetRot().y) * 50.0f,
				pos.y,
				0.0f
			);

			// �ړ��ʂ�ݒ肷��
			m_drumMove = (m_posDest - m_pDrum->GetPos()) * 0.05f;
		}

		break;

	case ACT_BRING:			// �\�����

		if (m_pDrum != nullptr)
		{ // ��Q�������݂����ꍇ

			// ���[�J���ϐ��錾
			D3DXVECTOR3 ObstaPos;		// ��Q���̈ʒu

			// ��Q���̈ʒu���擾����
			ObstaPos = m_pDrum->GetPos();

			// ��Q���̈ʒu���ړ�����
			ObstaPos.x += m_drumMove.x;
			ObstaPos.z += m_drumMove.z;

			if (ObstaPos.z <= 0.0f)
			{ // Z���� 0.0f �ȏ�ɂȂ����ꍇ

				// ��Q���̈ʒu��ݒ肷��
				ObstaPos.x = m_posDest.x;
				ObstaPos.z = m_posDest.z;

				// ��ԃJ�E���g�����Z����
				m_nStateCount++;
			}

			// ��Q���̈ʒu��ݒ肷��
			m_pDrum->SetPos(ObstaPos);

			if (m_nStateCount >= 90)
			{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

				// �V���[�g��Ԃɂ���
				m_action = ACT_SHOOT;

				if (m_bRight == true)
				{ // �E�������Ă����ꍇ

					// �q�b�g����
					m_pDrum->Hit();
				}
				else
				{ // ���������Ă����ꍇ

					// �q�b�g����
					m_pDrum->HitLeft();
				}

				// ��Q���̃|�C���^�� NULL �ɂ���
				m_pDrum = nullptr;
			}
		}

		break;

	case ACT_SHOOT:			// �V���[�g���

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % 50 == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ��Q���̐�������
			m_pDrum = dynamic_cast<CDrum*>(CObstacle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 1.0f, 100.0f), CObstacle::TYPE::TYPE_DRUM));

			// �A�N�V��������̐ݒ菈��
			m_pDrum->SetCollision(false);

			// �A�N�V��������̐ݒ菈��
			m_pDrum->SetAttack(false);

			// �ҋ@��Ԃɂ���
			m_action = ACT_WAITING;
		}

		break;
	}

	// �����蔻�菈��
	Collision();

	// ���[�V�����̍X�V����
	GetMotion()->Update();

	CManager::GetDebugProc()->Print("��Q���̃|�C���^�F%s\n", m_pDrum);
}

//===========================================
// �`�揈��
//===========================================
void CKickEnemy::Draw(void)
{
	// �`�揈��
	CEnemy::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CKickEnemy::SetData(const D3DXVECTOR3& pos)
{
	// �S�Ă̒l������������
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));			// ����
	SetScale(NONE_SCALE);	// �g�嗦

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// ����������
		GetHierarchy(nCntData)->SetPos(pos);								// �ʒu
		GetHierarchy(nCntData)->SetPosOld(pos);								// �O��̈ʒu
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);						// �g�嗦
		GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(INIT_PLAYER + nCntData));		// �f�[�^�̐ݒ菈��
	}

	// ���[�V�����̐ݒ菈��
	GetMotion()->Set(MOTIONTYPE_NEUTRAL);

	// ��Q���̐�������
	m_pDrum = dynamic_cast<CDrum*>(CObstacle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 1.0f, 100.0f), CObstacle::TYPE::TYPE_DRUM));

	// �A�N�V��������̐ݒ菈��
	m_pDrum->SetCollision(false);

	// �A�N�V��������̐ݒ菈��
	m_pDrum->SetAttack(false);
}

//===========================================
// ��Q���̑{������
//===========================================
bool CKickEnemy::SearchObstacle(void)
{
	if (m_pDrum != nullptr)
	{ // ��Q���̃|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�J���ϐ��錾
		CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
		CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g�ւ̃|�C���^

		// �I�u�W�F�N�g�̏����擾����
		pObj = CObject::GetTop(PRIORITY_BLOCK);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�ւ̃|�C���^�� NULL ����Ȃ������

			// ���̃I�u�W�F�N�g��������
			pObjNext = pObj->GetNext();

			if (pObj->GetType() == TYPE_OBSTACLE &&
				pObj == m_pDrum)
			{ // �I�u�W�F�N�g�̎�ނ��A�C�e�����A�A�h���X����v�����ꍇ

				// true ��Ԃ�
				return true;
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObjNext;
		}

		// false ��Ԃ�
		return false;
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

//===========================================
// �U���������
//===========================================
void CKickEnemy::Turn(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// ���[�J���|�C���^�錾
	CPlayer* pPlayer = CScene::GetPlayer();		// �v���C���[�̏����擾����

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		if (pPlayer->GetPos().x > pos.x)
		{ // �v���C���[���G���E�ɂ����ꍇ

			// �E�����ɂ���
			m_bRight = true;

			// ������ݒ肷��
			rot.y = D3DX_PI * 0.5f;
		}
		else if (pPlayer->GetPos().x < pos.x)
		{ // �v���C���[���G��荶�ɂ����ꍇ

			// �������ɂ���
			m_bRight = false;

			// ������ݒ肷��
			rot.y = -D3DX_PI * 0.5f;
		}
	}

	// ������ݒ肷��
	SetRot(rot);
}

//===========================================
// �����蔻�菈��
//===========================================
void CKickEnemy::Collision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �d�͏���
	Gravity(&m_fGravity, pos, ENEMY_GRAVITY);

	// �ʒu��ݒ肷��
	SetPos(pos);

	if (collision::EnemyBlockCollision(this) == true)
	{ // �u���b�N�Ƃ̓����蔻�肪 true �̏ꍇ

		// �d�͂� 0.0f �ɂ���
		m_fGravity = 0.0f;
	}
}