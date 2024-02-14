//============================================
//
// �L�����N�^�[�u���[�̃��C������[chara_blur.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "chara_blur.h"
#include "model.h"
#include "renderer.h"
#include "useful.h"

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CCharaBlur::CCharaBlur(PRIORITY priority) : CCharacter(CObject::TYPE_LOCUSMODEL, priority)
{
	// �S�Ă̒l���N���A����
	m_col = NONE_D3DXCOLOR;		// �F
	m_nLife = 0;				// ����
	m_fSub = 0.0f;				// ���Z��
}

//=========================================
// �f�X�g���N�^
//=========================================
CCharaBlur::~CCharaBlur()
{

}

//===========================================
// �v���C���[�̏���������
//===========================================
HRESULT CCharaBlur::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ��~
		assert(false);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_col = NONE_D3DXCOLOR;		// �F
	m_nLife = 0;				// ����
	m_fSub = 0.0f;				// ���Z��

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �v���C���[�̏I������
//===========================================
void CCharaBlur::Uninit(void)
{
	// �I������
	CCharacter::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CCharaBlur::Update(void)
{
	// �����x�����Z����
	m_col.a -= m_fSub;

	if (m_col.a <= 0.0f)
	{ // �����x�� 0.0f �ȉ��ɂȂ����ꍇ

		// �����x��␳����
		m_col.a = 0.0f;
	}

	// ���������炵�Ă���
	m_nLife--;

	if (m_nLife <= 0)
	{ // �G�t�F�N�g�̎������s������

		// �j������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//===========================================
// �v���C���[�̕`�揈��
//===========================================
void CCharaBlur::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`�揈��
	CCharacter::Draw(m_col);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================================
// ��������
//===========================================
CCharaBlur* CCharaBlur::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife, const PRIORITY priority)
{
	CCharaBlur* pPlayer = nullptr;

	if (pPlayer == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���������m�ۂ���
		pPlayer = new CCharaBlur(priority);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pPlayer;
	}

	if (pPlayer != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPlayer->SetData(pos, rot, scale, nNumModel, col, nLife);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pPlayer;
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CCharaBlur::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const int nNumModel, const D3DXCOLOR& col, const int nLife)
{
	// �S�Ă̒l������������
	SetPos(pos);				// �ʒu
	SetPosOld(GetPos());		// �O��̈ʒu
	SetRot(rot);				// ����
	SetScale(scale);			// �g�嗦

	// �x�^�ł�
	SetNumModel(nNumModel);

	// �S�Ă̒l��ݒ肷��
	m_col = col;				// �F
	m_nLife = nLife;			// ����
	m_fSub = m_col.a / nLife;	// ���Z��

	// �f�[�^�̐ݒ菈��
	CCharacter::SetData();
}

//=======================================
// ���f���̏��ݒ菈��
//=======================================
void CCharaBlur::SetModelData(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile& filedata, const int nParentIdx)
{
	// ����������
	GetHierarchy(nIdx)->SetPos(pos);				// �ʒu
	GetHierarchy(nIdx)->SetPosOld(pos);				// �O��̈ʒu
	GetHierarchy(nIdx)->SetRot(rot);				// ����
	GetHierarchy(nIdx)->SetScale(scale);			// �g�嗦
	GetHierarchy(nIdx)->SetFileData(filedata);		// �f�[�^�̐ݒ菈��

	if (nParentIdx != NONE_PARENT)
	{ // �e������ꍇ

		// �e�̃|�C���^
		GetHierarchy(nIdx)->SetParent(GetHierarchy(nParentIdx));
	}
}