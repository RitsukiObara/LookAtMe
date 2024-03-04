//===========================================
//
// �Ւd�̃��C������[alter.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "alter.h"
#include "texture.h"
#include "useful.h"

#include "game.h"
#include "gold_bone.h"
#include "gold_bone_light.h"
#include "alter_pole.h"
#include "alter_flash.h"
#include "boss.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Alter.x";			// ���f���̖��O
	const char* BREAKALTER_MODEL = "data\\MODEL\\BreakAlter.x";			// ��ꂽ�Ւd�̃��f��
	const char* BREAKALTERPOLE_MODEL = "data\\MODEL\\BreakAlterPole.x";	// ��ꂽ�Ւd�̐Β��̃��f��
	const D3DXVECTOR3 POS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	const D3DXVECTOR3 POLE_POS[CAlter::NUM_POLE] =		// �Β��̈ʒu
	{
		D3DXVECTOR3(0.0f,0.0f,-450.0f),
		D3DXVECTOR3(450.0f,0.0f,0.0f),
		D3DXVECTOR3(-450.0f,0.0f,0.0f),
	};
	const float ALTER_LIGHT_HEIGHT = 900.0f;			// �Ւd�����o�鍂��
	const int CHARGE_COUNT = 120;						// �`���[�W��Ԃ̃J�E���g��
}

//==============================
// �R���X�g���N�^
//==============================
CAlter::CAlter() : CModel(TYPE_ALTER, PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
	{
		m_apPole[nCnt] = nullptr;		// �Β��̏��
	}
	m_state = STATE_NONE;				// ���
	m_bLightUp = false;					// ���C�g�_����
}

//==============================
// �f�X�g���N�^
//==============================
CAlter::~CAlter()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CAlter::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CAlter::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
	{
		if (m_apPole[nCnt] != nullptr)
		{ // �Β��̏�� NULL ����Ȃ��ꍇ

			// �I������
			m_apPole[nCnt]->Uninit();
			m_apPole[nCnt] = nullptr;
		}
	}

	// �I������
	CModel::Uninit();

	// �Ւd��NULL������
	CGame::DeleteAlter();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CAlter::Update(void)
{
	switch (m_state)
	{
	case CAlter::STATE_NONE:

		// ���̍��̌�����
		GoldBoneLight();

		if (m_apPole[0]->IsArrival() == true &&
			m_apPole[1]->IsArrival() == true &&
			m_apPole[2]->IsArrival() == true)
		{ // �Β��ɑS�ċ��̍����������Ă����ꍇ

			// �`���[�W��Ԃւ̑J�ڏ���
			ChargeTrans();
		}

		break;

	case CAlter::STATE_CHARGE:

		for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
		{
			if (m_apPole[nCnt] != nullptr)
			{ // �|�[���� NULL ����Ȃ��ꍇ

				// �`���[�W����
				m_apPole[nCnt]->Charge();

				// ���̍��̈ʒu�ݒ菈��wwwwwwwwwwwwwwwwwwwwwwwwww
				m_apPole[nCnt]->GoldBonePosSet(GetPos());
			}
		}

		if (m_apPole[0]->GetBoneDist() <= 0.0f &&
			m_apPole[1]->GetBoneDist() <= 0.0f &&
			m_apPole[2]->GetBoneDist() <= 0.0f)
		{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

			for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
			{
				// ���̍�������
				m_apPole[nCnt]->GetGoldBone()->Uninit();
				m_apPole[nCnt]->SetGoldBone(nullptr);
			}

			// �{�X�o����Ԃɂ���
			m_state = STATE_BOSSAPPEAR;

			// �Ւd�̑M���𐶐�
			CAlterFlash::Create();

			// �{�X�𐶐�����
			CBoss::Create(NONE_D3DXVECTOR3, NONE_D3DXVECTOR3);
		}

		break;

	case CAlter::STATE_BOSSAPPEAR:
		break;

	case CAlter::STATE_WAIT:
		break;

	case CAlter::STATE_BREAK:
		break;

	default:

		// ��~
		assert(false);

		break;
	}

	for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
	{
		if (m_apPole[nCnt] != nullptr)
		{ // �Β��̏�� NULL ����Ȃ��ꍇ

			// �X�V����
			m_apPole[nCnt]->Update();
		}
	}
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CAlter::Draw(void)
{
	// �`�揈��
	CModel::Draw();

	for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
	{
		if (m_apPole[nCnt] != nullptr)
		{ // �Β��̏�� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apPole[nCnt]->Draw();
		}
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CAlter::SetData(void)
{
	// ���̐ݒ菈��
	SetPos(POS);					// �ʒu
	SetPosOld(POS);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
	{
		if (m_apPole[nCnt] == nullptr)
		{ // �Β��� NULL �������ꍇ

			// �Β��𐶐�
			m_apPole[nCnt] = CAlterPole::Create(POLE_POS[nCnt]);
		}
	}
	m_state = STATE_NONE;				// ���
	m_bLightUp = false;					// ���C�g�_����
}

//=======================================
// ��������
//=======================================
CAlter* CAlter::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAlter* pAlter = nullptr;		// �C���X�^���X�𐶐�

	if (pAlter == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pAlter = new CAlter;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pAlter != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pAlter->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pAlter->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �Ւd�̃|�C���^��Ԃ�
	return pAlter;
}

//=======================================
// �Β��̎擾����
//=======================================
CAlterPole* CAlter::GetPole(const int nIdx) const
{
	if (nIdx >= NUM_POLE)
	{ // �C���f�b�N�X���ő吔�ȏ�̏ꍇ

		// ��~
		assert(false);
	}

	// �Β��̏���Ԃ�
	return m_apPole[nIdx];
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CAlter::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CAlter::STATE CAlter::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// ���C�g�_���󋵂̐ݒ菈��
//=======================================
void CAlter::SetEnableLightUp(const bool bLight)
{
	// ���C�g�_���󋵂�ݒ肷��
	m_bLightUp = bLight;
}

//=======================================
// ���C�g�_���󋵂̎擾����
//=======================================
bool CAlter::IsLightUp(void) const
{
	// ���C�g�_���󋵂�Ԃ�
	return m_bLightUp;
}

//=======================================
// �j�󏈗�
//=======================================
void CAlter::Break(void)
{
	// �Ւd�̃��f����ς���
	SetFileData(CManager::Get()->GetXFile()->Regist(BREAKALTER_MODEL));

	for (int nCntPole = 0; nCntPole < CAlter::NUM_POLE; nCntPole++)
	{
		if (m_apPole[nCntPole] != nullptr)
		{ // �Β��� NULL ����Ȃ��ꍇ

			// �Β��̃��f����ς���
			m_apPole[nCntPole]->SetFileData(CManager::Get()->GetXFile()->Regist(BREAKALTERPOLE_MODEL));
		}
	}

	// �j���Ԃɂ���
	m_state = STATE_BREAK;
}

//=======================================
// �`���[�W��Ԃւ̑J�ڏ���
//=======================================
void CAlter::ChargeTrans(void)
{
	// �`���[�W��Ԃɂ���
	m_state = STATE_CHARGE;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 posBone;

	for (int nCnt = 0; nCnt < NUM_POLE; nCnt++)
	{
		if (m_apPole[nCnt] != nullptr &&
			m_apPole[nCnt]->GetGoldBone() != nullptr)
		{ // ���̍�������ɂ������ꍇ

			// �ʒu���擾
			pos = GetPos();
			posBone = m_apPole[nCnt]->GetGoldBone()->GetPos();

			// ���̍��ւ̌������Z�o
			m_apPole[nCnt]->SetBoneRot(atan2f(posBone.x - pos.x, posBone.z - pos.z));

			// ���̍��ւ̋������Z�o
			m_apPole[nCnt]->SetBoneDist(sqrtf((posBone.x - pos.x) * (posBone.x - pos.x) + (posBone.z - pos.z) * (posBone.z - pos.z)));
		}
	}
}

//=======================================
// ���̍��̌�����
//=======================================
void CAlter::GoldBoneLight(void)
{
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pBone = nullptr;			// ���̃|�C���^

	for (int nCnt = 0; nCnt < list.GetNumData(); nCnt++)
	{
		// ���̍����擾
		pBone = list.GetData(nCnt);

		if (m_bLightUp == true)
		{ // ���C�g�_���󋵂� true �̏ꍇ

			if (pBone->GetLight() == nullptr &&
				pBone->GetState() == CGoldBone::STATE_NONE)
			{ // �ʏ��Ԃ̏ꍇ

				// ���C�g�̐ݒ菈��
				pBone->SetLight();
			}
		}
		else
		{ // ��L�ȊO

			if (pBone->GetLight() != nullptr &&
				pBone->GetLight()->GetState() == CGoldBoneLight::STATE_EXTEND)
			{ // �����g���Ԃ̏ꍇ

				// ���C�g���k����Ԃɂ���
				pBone->GetLight()->SetState(CGoldBoneLight::STATE_SHRINK);
			}
		}
	}
}