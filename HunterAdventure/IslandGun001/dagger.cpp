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
	const char* ORBIT_TEXTURE = "data\\TEXTURE\\DaggerOrbit.png";		// �O�Ղ̃e�N�X�`��
	const D3DXVECTOR3 DAGGER_POS = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);		// �_�K�[�̈ʒu
	const D3DXVECTOR3 DAGGER_ROT = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);	// �_�K�[�̌���
	const D3DXVECTOR3 ORBIT_OFFSET[2] =					// �O�Ղ̃I�t�Z�b�g
	{
		D3DXVECTOR3(0.0f, 0.0f, -10.0f),
		D3DXVECTOR3(0.0f, 0.0f, -50.0f)
	};
}

//==============================
// �R���X�g���N�^
//==============================
CDagger::CDagger() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_PLAYER)
{
	// �S�Ă̒l���N���A����
	m_pOrbit = nullptr;				// �O�Ղ̏��
	m_pMtxParent = nullptr;			// �e�̃}�g���b�N�X
	m_bDisp = false;				// �`���
	m_bDispOrbit = false;			// �O�Ղ̕`���
	m_bDispOrbitOld = m_bDispOrbit;	// �O�Ղ̑O��̕`���
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
	}

	if (m_bDispOrbit == true &&
		m_pOrbit != nullptr)
	{ // �O�Ղ� NULL ����Ȃ��ꍇ

		if (m_bDispOrbit == true &&
			m_bDispOrbitOld == false)
		{ // �`��󋵂� true �ɂȂ����u��

			// �ʒu�̃��Z�b�g����
			m_pOrbit->PosReset();
		}

		// �`�揈��
		m_pOrbit->Draw();
	}

	// �O��̕`��󋵂�������
	m_bDispOrbitOld = m_bDispOrbit;
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
	m_pMtxParent = mtx;				// �e�̃}�g���b�N�X
	m_bDisp = false;				// �`���
	m_bDispOrbit = false;			// �O�Ղ̕`���
	m_bDispOrbitOld = m_bDispOrbit;	// �O�Ղ̑O��̕`���

	// �O�Ղ𐶐�����
	m_pOrbit = COrbit::Create(GetMatrixPoint(), GetPos(), ORBIT_OFFSET[0], ORBIT_OFFSET[1], ORBIT_TEXTURE, TYPE_NONE);
}

//=======================================
// ��������
//=======================================
CDagger* CDagger::Create(D3DXMATRIX* mtx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CDagger* pDagger = nullptr;	// �C���X�^���X�𐶐�

	if (pDagger == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pDagger = new CDagger;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pDagger != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pDagger->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pDagger->SetData(mtx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �_�K�[�̃|�C���^��Ԃ�
	return pDagger;
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

//=======================================
// �O�Ղ̕`��󋵂̐ݒ菈��
//=======================================
void CDagger::SetEnableDispOrbit(const bool bDisp)
{
	// �O�Ղ̕`��󋵂�ݒ肷��
	m_bDispOrbit = bDisp;
}

//=======================================
// �O�Ղ̕`��󋵂̎擾����
//=======================================
bool CDagger::IsDispOrbit(void) const
{
	// �O�Ղ̕`��󋵂�Ԃ�
	return m_bDispOrbit;
}