//============================================================
//
// �{�X���� [boss.cpp]
// Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "model.h"
#include "boss.h"
#include "manager.h"
#include "file.h"
#include "useful.h"

#include "motion.h"
#include "boss_appearstate.h"
#include "boss_destroystate.h"
#include "boss_stunstate.h"
#include "boss_lifeUI.h"
#include "objectElevation.h"
#include "boss_collision.h"
#include "fraction.h"

//------------------------------------------------------------
// �������O���
//------------------------------------------------------------
namespace
{
	const float LAND_GRAVITY = -50.0f;			// ���n���̏d��
	const int MAX_LIFE = 3000;					// �̗͂̍ő吔
	const int BARRIER_FRAC_MOVE_WIDTH = 40;		// �o���A�U�����̔j�Ђ̉��̈ړ���
	const int BARRIER_FRAC_MOVE_HEIGHT = 20;	// �o���A�U�����̔j�Ђ̏c�̈ړ���
	const int BARRIER_FRAC_MOVE_MIN = 10;		// �o���A�U�����̔j�Ђ̈ړ��ʂ̍Œ�l
	const int NUM_BARRIER_FRAC = 5;				// �o���A�U�����̔j�Ђ̐�
	const int BARRIER_FRAC_LIFE = 100;			// �o���A�U�����̔j�Ђ̎���
	const int WEAK_LIFE[CBoss::WEAK_MAX] =		// ��_�̃��C�t
	{
		40,
		10,
		10,
		10,
		10
	};
	const int WEAK_PART[CBoss::WEAK_MAX] =		// ��_�̃p�[�c�ԍ�
	{
		2,
		5,
		6,
		13,
		14
	};
	const int WEAK_MATERIAL_NUM = 0;			// ��_�̃}�e���A���ԍ�
}

// �ÓI�����o�ϐ�
CListManager<CBoss*> CBoss::m_list = {};				// ���X�g

//================================
// �R���X�g���N�^
//================================
CBoss::CBoss() : CCharacter(CObject::TYPE_BOSS, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pMotion = nullptr;			// ���[�V����
	m_pState = nullptr;				// ��Ԃ̏��
	m_pLifeUI = nullptr;			// �̗�UI�̏��
	memset(m_apColl, 0, sizeof(m_apColl));			// �����蔻��̋�
	memset(m_apMatCopy, 0, sizeof(m_apMatCopy));	// �}�e���A���̃R�s�[
	for (int nCnt = 0; nCnt < WEAK_MAX; nCnt++)
	{
		m_aWeakPointLife[nCnt] = WEAK_LIFE[nCnt];	// ��_�̃��C�t
	}
	m_nLife = MAX_LIFE;				// �̗�
	m_bDown = false;				// �_�E����

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//================================
// �f�X�g���N�^
//================================
CBoss::~CBoss()
{

}

//================================
// ����������
//================================
HRESULT CBoss::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���f���̑�����ݒ�
	SetNumModel(CMotion::GetNumModel(CMotion::STYLE_BOSS));

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // ���[�V������ NULL �������ꍇ

		if (m_pMotion == nullptr)
		{ // ���[�V������ NULL �̏ꍇ

			// ���[�V�����𐶐�����
			m_pMotion = CMotion::Create();
		}
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ���[�V�����̏����擾����
		m_pMotion->SetInfo(CMotion::STYLE_BOSS, GetHierarchy(), GetNumModel());
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);
	}

	// �l��Ԃ�
	return S_OK;
}

//================================
// �I������
//================================
void CBoss::Uninit(void)
{
	if (m_pMotion != nullptr)
	{ // ���[�V������ NULL ����Ȃ��ꍇ

		// ���[�V�����̏I������
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ��Ԃ̏I������
		m_pState->Delete();
		m_pState = nullptr;
	}

	if (m_pLifeUI != nullptr)
	{ // �̗�UI�� NULL ����Ȃ��ꍇ

		// �̗�UI�� NULL �ɂ���
		m_pLifeUI = nullptr;
	}

	for (int nCntColl = 0; nCntColl < GetNumModel(); nCntColl++)
	{
		if (m_apColl[nCntColl] != nullptr)
		{ // �����蔻��̏�� NULL ����Ȃ��ꍇ

			// �I������
			m_apColl[nCntColl]->Uninit();
			m_apColl[nCntColl] = nullptr;
		}
	}

	for (int nCntMat = 0; nCntMat < MAX_PARTS; nCntMat++)
	{
		if (m_apMatCopy[nCntMat] != nullptr)
		{ // �}�e���A���̃R�s�[�� NULL ����Ȃ��ꍇ

			// �}�e���A���̏I������
			delete[] m_apMatCopy[nCntMat];
			m_apMatCopy[nCntMat] = nullptr;
		}
	}

	// �I������
	CCharacter::Uninit();

	// ������������
	m_list.Pull(this);
}

//================================
// �X�V����
//================================
void CBoss::Update(void)
{
	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ��ԏ���
		m_pState->Process(this);
	}

	if (m_pMotion != nullptr)
	{ // ���[�V������ NULL ����Ȃ��ꍇ

		// ���[�V�����̍X�V����
		m_pMotion->Update();
	}
}

//================================
// �`�揈��
//================================
void CBoss::Draw(void)
{
	// �`�揈��
	CCharacter::Draw(m_apMatCopy);
}

//================================
// �q�b�g����
//================================
void CBoss::Hit(const int nDamage)
{
	if (m_nLife > 0)
	{ // �̗͂��܂�����ꍇ

		// �_���[�W���󂯂�
		m_nLife -= nDamage;

		// ������K�p����
		m_pLifeUI->SetLife(m_nLife);

		if (m_nLife <= 0)
		{ // �̗͂������Ȃ����ꍇ

			// ��Ԃ̏�������(���������[�N���N����\�������邽��)
			m_pState->Delete();
			m_pState = nullptr;

			// ���S��Ԃɂ���
			ChangeState(new CBossDestroyState);
		}
	}
}

//================================
// �o���A�j�󏈗�
//================================
void CBoss::BarrierBreak(const D3DXVECTOR3& pos, const int nPart)
{
	for (int nCntPart = 0; nCntPart < WEAK_MAX; nCntPart++)
	{
		if (m_aWeakPointLife[nCntPart] > 0 &&
			nPart == WEAK_PART[nCntPart])
		{
			// �o���A�̃q�b�g����
			BarrierHit(pos, nPart, nCntPart);

			// for�����甲����
			break;
		}
	}

	if (m_bDown == false &&
		m_aWeakPointLife[WEAK_RFSHIN] <= 0 &&
		m_aWeakPointLife[WEAK_LFSHIN] <= 0 &&
		m_aWeakPointLife[WEAK_RBSHIN] <= 0 &&
		m_aWeakPointLife[WEAK_LBSHIN] <= 0)
	{ // ��_���S�ĉ󂳂ꂽ�ꍇ

		// ��Ԃ̏�������(���������[�N���N����\�������邽��)
		m_pState->Delete();
		m_pState = nullptr;

		// �C���Ԃɂ���
		ChangeState(new CBossStunState);

		// �C���Ԃɂ���
		m_bDown = true;
	}
}

//================================
// �o���A�̃q�b�g����
//================================
void CBoss::BarrierHit(const D3DXVECTOR3& pos, const int nPart, const int nCntPart)
{
	// �ړ���
	int nMoveWidth;
	int nMoveHeight;

	for (int nCnt = 0; nCnt < NUM_BARRIER_FRAC; nCnt++)
	{
		// �ړ��ʂ������_���ŎZ�o
		nMoveWidth = (rand() % BARRIER_FRAC_MOVE_WIDTH + BARRIER_FRAC_MOVE_MIN);
		nMoveHeight = (rand() % BARRIER_FRAC_MOVE_HEIGHT + BARRIER_FRAC_MOVE_MIN);

		// �j�Ђ̐���
		CFraction::Create(pos, CFraction::TYPE::TYPE_BOSSROCK, BARRIER_FRAC_LIFE, nMoveWidth, nMoveHeight);
	}

	// ���������Z����
	m_aWeakPointLife[nCntPart]--;

	if (m_aWeakPointLife[nCntPart] <= 0)
	{ // ���C�t�������Ȃ����ꍇ

		// �F��ݒ肷��
		m_apMatCopy[nPart][WEAK_MATERIAL_NUM].MatD3D.Diffuse = D3DXCOLOR(0.1f, 0.0f, 0.1f, 1.0f);
		m_apMatCopy[nPart][WEAK_MATERIAL_NUM].MatD3D.Ambient = D3DXCOLOR(0.1f, 0.0f, 0.1f, 1.0f);
		m_apMatCopy[nPart][WEAK_MATERIAL_NUM].MatD3D.Emissive = D3DXCOLOR(0.1f, 0.0f, 0.1f, 1.0f);
	}
}

//================================
// �C��񕜏���
//================================
void CBoss::StunRecovery(void)
{
	int nNumMat = 0;

	// �C��𒼂�
	m_bDown = false;

	for (int nCnt = WEAK_RFSHIN; nCnt < WEAK_MAX; nCnt++)
	{
		// �̗͂��񕜂���
		m_aWeakPointLife[nCnt] = WEAK_LIFE[nCnt];

		// �}�e���A���̑������擾
		nNumMat = (int)GetHierarchy(WEAK_PART[nCnt])->GetFileData().dwNumMat;

		// �}�e���A���̃R�s�[���擾
		m_apMatCopy[WEAK_PART[nCnt]][WEAK_MATERIAL_NUM] = GetHierarchy(WEAK_PART[nCnt])->GetMaterial(WEAK_MATERIAL_NUM);
	}
}

//================================
// ���̐ݒ菈��
//================================
void CBoss::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	int nNumMat;

	// �S�Ă̒l������������
	SetPos(pos);				// �ʒu
	SetPosOld(GetPos());		// �O��̈ʒu
	SetRot(rot);				// ����
	SetScale(NONE_SCALE);		// �g�嗦

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// ����������
		GetHierarchy(nCntData)->SetPos(pos);										// �ʒu
		GetHierarchy(nCntData)->SetPosOld(pos);										// �O��̈ʒu
		GetHierarchy(nCntData)->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// ����
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// �g�嗦

		// �}�e���A���̐����擾����
		nNumMat = (int)GetHierarchy(nCntData)->GetFileData().dwNumMat;

		// �}�e���A���̃R�s�[�𐶐�����
		m_apMatCopy[nCntData] = new D3DXMATERIAL[nNumMat];

		for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
		{
			// �}�e���A���̃R�s�[���擾
			m_apMatCopy[nCntData][nCntMat] = GetHierarchy(nCntData)->GetMaterial(nCntMat);
		}
	}

	// ���[�V�����̃��Z�b�g����
	m_pMotion->ResetMotion(MOTIONTYPE_FLY);

	// ��Ԃ̐؂�ւ�����
	ChangeState(new CBossAppearState);

	// �S�Ă̒l��ݒ肷��
	m_pLifeUI = CBossLifeUI::Create(MAX_LIFE);		// �̗�UI
	m_nLife = MAX_LIFE;		// �̗�
	m_bDown = false;		// �_�E����

	// �����蔻���ݒ肷��
	CManager::Get()->GetFile()->SetBossColl(&m_apColl[0]);
}

//===========================================
// ��������
//===========================================
CBoss* CBoss::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_list.IsEmpty() == false)
	{ // �󔒏�Ԃ̏ꍇ

		// �������Ȃ�(���}���u)
		return nullptr;
	}

	// �{�X�̃C���X�^���X
	CBoss* pBoss = nullptr;

	if (pBoss == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �{�X�𐶐�����
		pBoss = new CBoss;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pBoss;
	}

	if (pBoss != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBoss->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBoss->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �{�X�̃|�C���^��Ԃ�
	return pBoss;
}

//===========================================
// ���X�g�̎擾����
//===========================================
CListManager<CBoss*> CBoss::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}

//===========================================
// �N���n�ʂƂ̓����蔻��
//===========================================
bool CBoss::ElevationCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	float fHeight = 0.0f;			// ����
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;	// �擪�̏���
	CElevation* pElevEnd = nullptr;	// �����̒l
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
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				pos.y = fHeight;

				// �ʒu���X�V����
				SetPos(pos);

				// true ��Ԃ�
				return true;
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

	// �ʒu���X�V����
	SetPos(pos);

	// false ��Ԃ�
	return false;
}

//===========================================
// ��Ԃ̑J�ڏ���
//===========================================
void CBoss::ChangeState(CBossState* pNext)
{
	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ��Ԃ̏I������
		m_pState->Uninit();
		m_pState = nullptr;
	}

	// ���̏�Ԃ�ݒ肷��
	m_pState = pNext;

	if (m_pState != nullptr)
	{ // ��Ԃ� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		m_pState->SetData(this);
	}
}

//===========================================
// ���[�V�����̎擾����
//===========================================
CMotion* CBoss::GetMotion(void)
{
	// ���[�V�����̏���Ԃ�
	return m_pMotion;
}

//===========================================
// �����蔻��̎擾����
//===========================================
CBossCollision* CBoss::GetColl(const int nIdx)
{
	// �����蔻��̏���Ԃ�
	return m_apColl[nIdx];
}