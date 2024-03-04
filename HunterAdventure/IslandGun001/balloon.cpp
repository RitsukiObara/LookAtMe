//===========================================
//
// ���D�̃��C������[balloon.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "balloon.h"
#include "useful.h"

#include "balloon_rope.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Balloon.x";		// ���D���f���̖��O
	const float HEIGHT = 40.0f;			// �����̕ω���
	const float ADD_ROT = 0.02f;		// �����̉��Z��
}

//==============================
// �R���X�g���N�^
//==============================
CBalloon::CBalloon() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pRope = nullptr;			// �R�̃��f��
	m_fPosYInit = 0.0f;			// �����̍���
	m_fHeightRot = 0.0f;		// ���������߂����
}

//==============================
// �f�X�g���N�^
//==============================
CBalloon::~CBalloon()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CBalloon::Init(void)
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
// �j�Ђ̏I������
//========================================
void CBalloon::Uninit(void)
{
	if (m_pRope != nullptr)
	{ // �R�� NULL ����Ȃ��ꍇ

		// �R�̏I������
		m_pRope->Uninit();
		m_pRope = nullptr;
	}

	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CBalloon::Update(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();

	// �ʒu��ݒ肷��
	pos.y = m_fPosYInit + sinf(m_fHeightRot) * HEIGHT;

	// ���������Z����
	m_fHeightRot += ADD_ROT;

	// �����̐��K��
	useful::RotNormalize(&m_fHeightRot);

	// �ʒu��K�p
	SetPos(pos);
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CBalloon::Draw(void)
{
	// �`�揈��
	CModel::Draw();

	if (m_pRope != nullptr)
	{ // �R�� NULL ����Ȃ��ꍇ

		// �R�̕`�揈��
		m_pRope->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBalloon::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l��ݒ肷��
	m_fPosYInit = pos.y;			// �����̍���
	m_fHeightRot = 0.0f;			// ���������߂����

	if (m_pRope == nullptr)
	{ // �R�� NULL ����Ȃ��ꍇ

		// �R�̐���
		m_pRope = CBalloonRope::Create(GetMatrixPoint());
	}
}

//=======================================
// ��������
//=======================================
CBalloon* CBalloon::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBalloon* pBalloon = nullptr;		// �C���X�^���X�𐶐�

	if (pBalloon == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pBalloon = new CBalloon;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBalloon != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBalloon->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBalloon->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���D�̃|�C���^��Ԃ�
	return pBalloon;
}