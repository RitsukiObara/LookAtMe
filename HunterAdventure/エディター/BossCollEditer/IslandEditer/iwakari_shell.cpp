//===========================================
//
// �C���J���̊k�̃��C������[iwakari_shell.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "iwakari_shell.h"
#include "renderer.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\IwakariShell.x";	// �C���J���̊k�̖��O
	const int LIFE = 80;								// �k�̎���
}

//==============================
// �R���X�g���N�^
//==============================
CIwakariShell::CIwakariShell() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_nLife = 0;		// �̗�
}

//==============================
// �f�X�g���N�^
//==============================
CIwakariShell::~CIwakariShell()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CIwakariShell::Init(void)
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
void CIwakariShell::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CIwakariShell::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CIwakariShell::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CIwakariShell::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���̐ݒ菈��
	SetPos(pos);						// �ʒu
	SetPosOld(pos);						// �O��̈ʒu
	SetRot(rot);						// ����
	SetScale(NONE_SCALE);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));		// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_nLife = LIFE;			// �̗�
}

//=======================================
// ��������
//=======================================
CIwakariShell* CIwakariShell::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CIwakariShell* pShell = nullptr;	// �C���X�^���X�𐶐�

	if (pShell == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pShell = new CIwakariShell;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pShell != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pShell->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pShell->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �k�̃|�C���^��Ԃ�
	return pShell;
}

//=======================================
// �̗͂̐ݒ菈��
//=======================================
void CIwakariShell::SetLife(const int nLife)
{
	// �̗͂�ݒ肷��
	m_nLife = nLife;
}

//=======================================
// �̗͂̎擾����
//=======================================
int CIwakariShell::GetLife(void) const
{
	// �̗͂�Ԃ�
	return m_nLife;
}