//===========================================
//
// ���e�̓��ΐ��̃��C������[bomb_fuse.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "bomb_fuse.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\BombFuse.x";		// ���f���̖��O
	const D3DXVECTOR3 FUSE_POS = D3DXVECTOR3(0.0f, 94.0f, 0.0f);	// �ʒu�̐ݒ菈��
	const float SUB_POS_Y = 0.03f;						// �ʒu�̌��Z��
}

//==============================
// �R���X�g���N�^
//==============================
CBombFuse::CBombFuse() : CModel(TYPE_NONE, PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pMtxParent = nullptr;		// �e�̃}�g���b�N�X
}

//==============================
// �f�X�g���N�^
//==============================
CBombFuse::~CBombFuse()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CBombFuse::Init(void)
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
void CBombFuse::Uninit(void)
{
	// �e�̃}�g���b�N�X�� NULL �ɂ���
	m_pMtxParent = nullptr;

	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CBombFuse::Update(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ʒu��������
	pos.y -= SUB_POS_Y;

	// �ʒu��K�p����
	SetPos(pos);
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CBombFuse::Draw(void)
{
	if (m_pMtxParent != nullptr)
	{ // �e�̃}�g���b�N�X�� NULL ����Ȃ��ꍇ

		// ���̃}�g���b�N�X�Ƃ̊|�����킹�`�揈��
		CModel::DrawMatrix(*m_pMtxParent);
	}
	else
	{ // ��L�ȊO

		// �`�揈��
		CModel::Draw();
	}

}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBombFuse::SetData(D3DXMATRIX* pMtx)
{
	// ���̐ݒ菈��
	SetPos(FUSE_POS);				// �ʒu
	SetPosOld(FUSE_POS);			// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_pMtxParent = pMtx;		// �e�̃}�g���b�N�X
}

//=======================================
// ��������
//=======================================
CBombFuse* CBombFuse::Create(D3DXMATRIX* pMtx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBombFuse* pFuse = nullptr;			// ���ΐ��𐶐�

	if (pFuse == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pFuse = new CBombFuse;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFuse != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pFuse->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pFuse->SetData(pMtx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���ΐ��̃|�C���^��Ԃ�
	return pFuse;
}