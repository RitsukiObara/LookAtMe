//===========================================
//
// �����̃��C������[coin.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "coin.h"
#include "sound.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Coin.x";	// ���f���̖��O
	const float INIT_CYCLESPEED = 0.08f;		// �����̉�]���x
	const float GET_CYCLESPEED = 0.2f;			// �擾���̉�]���x
	const float GET_HEIGHTDEST = 150.0f;		// �擾���̖ړI�̍���
	const float GET_POS_CORRECT = 10.0f;		// �擾���̈ʒu�̕␳��
	const int DEATH_COUNT = 70;					// ���S�܂ł̃J�E���g
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CCoin*> CCoin::m_aList[area::NUM_AREA] = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CCoin::CCoin() : CModel(CObject::TYPE_COIN, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_NONE;		// ���
	m_nAreaIdx = 0;				// �敪�̔ԍ�
	m_nDeathCount = 0;			// ���S�J�E���g
	m_fCycleSpeed = 0.0f;		// ��]���x
	m_fHeightDest = 0.0f;		// �ڕW�̍���
}

//==============================
// �f�X�g���N�^
//==============================
CCoin::~CCoin()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CCoin::Init(void)
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
void CCoin::Uninit(void)
{
	// ������������
	m_aList[m_nAreaIdx].Pull(this);

	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CCoin::Update(void)
{
	switch (m_state)
	{
	case CCoin::STATE_NONE:		// �ʏ���

		// ���ɂȂ�

		break;

	case CCoin::STATE_GET:		// �擾���

		// �擾����
		Get();

		break;

	case STATE_DEATH:			// ���S���

		// ���S�J�E���g�����Z����
		m_nDeathCount++;

		if (m_nDeathCount >= DEATH_COUNT)
		{ //���S�J�E���g����萔�ȏ�ɂȂ����ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ��]����
	Cycle();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CCoin::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CCoin::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);						// �ʒu
	SetPosOld(pos);						// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);			// ����
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// ���f���̏��

	// �S�Ă̒l������������
	m_state = STATE_NONE;						// ���
	m_nDeathCount = 0;							// ���S�J�E���g
	m_fCycleSpeed = INIT_CYCLESPEED;			// ��]���x
	m_fHeightDest = pos.y + GET_HEIGHTDEST;		// �ڕW�̍���

	// �敪�̔ԍ��̐ݒ菈��
	m_nAreaIdx = area::SetFieldIdx(GetPos());

	// ���X�g�ɒǉ�����
	m_aList[m_nAreaIdx].Regist(this);
}

//=======================================
// ��������
//=======================================
CCoin* CCoin::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCoin* pCoin = nullptr;	// �C���X�^���X�𐶐�

	if (pCoin == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pCoin = new CCoin;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCoin != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCoin->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pCoin->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �����̃|�C���^��Ԃ�
	return pCoin;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CCoin*> CCoin::GetList(const int nIdx)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_aList[nIdx];
}

//=======================================
// �q�b�g����
//=======================================
void CCoin::Hit(void)
{
	// �擾��Ԃɂ���
	m_state = STATE_GET;

	// ��]���x�𑁂�����
	m_fCycleSpeed = GET_CYCLESPEED;

	// �R�C���Q�b�g����炷
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_COINGET);
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CCoin::STATE CCoin::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �擾��ԏ���
//=======================================
void CCoin::Get(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	if (useful::FrameCorrect(m_fHeightDest, &pos.y, 10.0f) == true)
	{ // �ړI�̒l�ɒB�����ꍇ

		// ���S��Ԃɂ���
		m_state = STATE_DEATH;
	}

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// ��]����
//=======================================
void CCoin::Cycle(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ���������Z����
	rot.y += m_fCycleSpeed;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// ������K�p����
	SetRot(rot);
}