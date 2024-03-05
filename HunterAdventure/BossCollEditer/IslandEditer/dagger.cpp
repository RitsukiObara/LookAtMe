//===========================================
//
// ���e�̃��C������[handgun.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "dagger.h"
#include "useful.h"

#include "orbit.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Dagger.x";		// ���f���̖��O
	const D3DXVECTOR3 DAGGER_POS = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);		// �_�K�[�̈ʒu
	const D3DXVECTOR3 DAGGER_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// �_�K�[�̌���
}

//==============================
// �R���X�g���N�^
//==============================
CDagger::CDagger() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_pOrbit = nullptr;			// �O�Ղ̏��
	m_pMtxParent = nullptr;		// �e�̃}�g���b�N�X
	m_bDisp = false;			// �`���
	m_bDispOld = m_bDisp;		// �O��̕`���
}

//==============================
// �f�X�g���N�^
//==============================
CDagger::~CDagger()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CDagger::Init(void)
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
void CDagger::Uninit(void)
{
	if (m_pOrbit != nullptr)
	{ // �O�Ղ� NULL ����Ȃ��ꍇ

		// �O�Ղ̏I������
		m_pOrbit->Uninit();
		m_pOrbit = nullptr;
	}

	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CDagger::Update(void)
{

}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CDagger::Draw(void)
{
	if (m_bDisp == true)
	{ // �`��󋵂� true �̏ꍇ

		// �`�揈��
		CModel::DrawMatrix(*m_pMtxParent);

		if (m_bDisp == true &&
			m_bDispOld == false)
		{ // �`��󋵂� true �ɂȂ����u��

			// �ʒu�̃��Z�b�g����
			m_pOrbit->PosReset();
		}

		if (m_pOrbit != nullptr)
		{ // �O�Ղ� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_pOrbit->Draw();
		}
	}

	// �O��̕`��󋵂�������
	m_bDispOld = m_bDisp;
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CDagger::SetData(D3DXMATRIX* mtx)
{
	// ���̐ݒ菈��
	SetPos(DAGGER_POS);					// �ʒu
	SetPosOld(DAGGER_POS);				// �O��̈ʒu
	SetRot(DAGGER_ROT);					// ����
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f�����

	// �S�Ă̒l���N���A����
	m_pMtxParent = mtx;		// �e�̃}�g���b�N�X
	m_bDisp = false;		// �`���
	m_bDispOld = m_bDisp;		// �O��̕`���

	D3DXVECTOR3 OffSet[2] = {};

	OffSet[0] = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	OffSet[1] = D3DXVECTOR3(0.0f, 0.0f, -50.0f);

	// �O�Ղ𐶐�����
	m_pOrbit = COrbit::Create(GetMatrixPoint(), GetPos(), OffSet[0], OffSet[1], "data\\TEXTURE\\SilverGloss.png", TYPE_NONE);
}

//=======================================
// ��������
//=======================================
CDagger* CDagger::Create(D3DXMATRIX* mtx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CDagger* pGun = nullptr;	// �C���X�^���X�𐶐�

	if (pGun == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pGun = new CDagger;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pGun != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pGun->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pGun->SetData(mtx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���e�̃|�C���^��Ԃ�
	return pGun;
}

//=======================================
// �`��󋵂̐ݒ菈��
//=======================================
void CDagger::SetEnableDisp(const bool bDisp)
{
	// �`��󋵂�ݒ肷��
	m_bDisp = bDisp;
}

//=======================================
// �`��󋵂̎擾����
//=======================================
bool CDagger::IsDisp(void) const
{
	// �`��󋵂�Ԃ�
	return m_bDisp;
}