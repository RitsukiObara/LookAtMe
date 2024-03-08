//===========================================
//
// �Ŕ̃��C������[signboard.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "signboard.h"
#include "sound.h"
#include "texture.h"
#include "useful.h"

#include "push_timing.h"
#include "signboard_explain.h"
#include "tutorial.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Signboard.x";		// ���f���̖��O
	const float BUTTON_SHIFT = 300.0f;					// �{�^���̂��炷��
	const float BUTTON_SIZE = 50.0f;					// �{�^���̃T�C�Y
	const int BUTTON_INTERVAL = 10;						// �{�^���̃A�j���[�V�����̃C���^�[�o��
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CSignboard*> CSignboard::m_list = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CSignboard::CSignboard() : CModel(TYPE_SIGNBOARD, PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pButton = nullptr;	// �{�^���̏��
	m_pExplain = nullptr;	// �����̏��
	m_type = TYPE_JUMP;		// ���

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CSignboard::~CSignboard()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CSignboard::Init(void)
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
void CSignboard::Uninit(void)
{
	if (m_pButton != nullptr)
	{ // �{�^���� NULL ����Ȃ��ꍇ

		// �{�^���̏I������
		m_pButton = nullptr;
	}

	// �I������
	CModel::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CSignboard::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CSignboard::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CSignboard::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(rot);					// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_pButton = CPushTiming::Create(D3DXVECTOR3(pos.x, pos.y + BUTTON_SHIFT, pos.z), BUTTON_SIZE, CPushTiming::TYPE_PAD_A, BUTTON_INTERVAL);	// �{�^���̏��
	m_pButton->SetEnableDisp(false);
	m_type = type;		// ���
}

//=======================================
// �����ڍs����
//=======================================
void CSignboard::Explain(void)
{
	if (m_pExplain == nullptr)
	{ // �����󋵂� NULL �̏ꍇ

		// �����𐶐�
		m_pExplain = CSignboardExpl::Create(m_type);
	}

	// ������Ԃɂ���
	CTutorial::SetState(CTutorial::STATE_EXPL);

	// �Ŕ̏���ݒ肷��
	CTutorial::SetLookSign(this);

	// �Ŕ���炷
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNBOARD);
}

//=======================================
// ��������
//=======================================
CSignboard* CSignboard::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSignboard* pSignboard = nullptr;		// �C���X�^���X�𐶐�

	if (pSignboard == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pSignboard = new CSignboard;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pSignboard != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pSignboard->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pSignboard->SetData(pos, rot, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �Ŕ̃|�C���^��Ԃ�
	return pSignboard;
}

//=======================================
// �����̐ݒ菈��
//=======================================
void CSignboard::SetExplain(CSignboardExpl* pExpl)
{
	// �����̏���ݒ肷��
	m_pExplain = pExpl;
}

//=======================================
// �{�^���\���̎擾����
//=======================================
CPushTiming* CSignboard::GetButton(void) const
{
	// �{�^���\���̏���Ԃ�
	return m_pButton;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CSignboard*> CSignboard::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}