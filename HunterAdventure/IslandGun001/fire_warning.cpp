//============================================
//
// �����ӂ̃��C������[fire_warning.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "fire_warning.h"
#include "renderer.h"
#include "manager.h"
#include "useful.h"

#include "fire_shot.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const int NUM_ANGLE = 8;			// �p�x�̐�
	const float RADIUS = 200.0f;		// ���a
	const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);		// �F�̏����l
	const int FIRE_CREATE_COUNT = 50;	// ���̐����J�E���g
}

//=========================================
// �R���X�g���N�^
//=========================================
CFireWarning::CFireWarning() : CObject3DFan(TYPE_FIREWARNING, PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_col = INIT_COL;		// �F
	m_nCount = 0;			// �o�߃J�E���g
}

//=========================================
// �f�X�g���N�^
//=========================================
CFireWarning::~CFireWarning()
{

}

//===========================================
// �����ӂ̏���������
//===========================================
HRESULT CFireWarning::Init(void)
{
	if (FAILED(CObject3DFan::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);

	// �l��Ԃ�
	return S_OK;
}

//===========================================
// �����ӂ̏I������
//===========================================
void CFireWarning::Uninit(void)
{
	// �I������
	CObject3DFan::Uninit();
}

//===========================================
// �����ӂ̍X�V����
//===========================================
void CFireWarning::Update(void)
{
	// �o�߃J�E���g�����Z����
	m_nCount++;

	// �F��ݒ肷��
	if (m_nCount >= FIRE_CREATE_COUNT)
	{ // �F�̌��������l�ȏ�ɂȂ����ꍇ
		 
		// �����o��
		CFireShot::Create(GetPos());

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_�̐ݒ菈��
	SetVertex();
}

//===========================================
// �����ӂ̕`�揈��
//===========================================
void CFireWarning::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

	// �`�揈��
	CObject3DFan::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�e�X�g�̗L��/�����ݒ�
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CFireWarning::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetNumAngle(NUM_ANGLE);			// �p�x�̑���
	SetRadius(RADIUS);				// ���a

	// �S�Ă̒l��ݒ肷��
	m_col = INIT_COL;		// �F
	m_nCount = 0;			// �o�߃J�E���g
}

//===========================================
// ��������
//===========================================
CFireWarning* CFireWarning::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CFireWarning* pWarning = nullptr;	// �����ӂ̃C���X�^���X�𐶐�

	if (pWarning == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pWarning = new CFireWarning;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pWarning != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pWarning->SetData(pos);

		// ����������
		if (FAILED(pWarning->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �����ӂ̃|�C���^��Ԃ�
	return pWarning;
}