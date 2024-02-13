//============================================
//
// �^�C�g������[title_name.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "locus2D.h"
#include "renderer.h"
#include "title_name.h"
#include "title_select.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define TITLE_NAME_TEXTURE			"data/TEXTURE/KariTitle.png"			// �^�C�g���̃e�N�X�`��
#define TITLE_NAME_SIZE				(D3DXVECTOR3(287.0f, 175.0f, 0.0f))		// �^�C�g���̃T�C�Y
#define TITLE_NAME_POS				(D3DXVECTOR3(-700.0f, 250.0f, 0.0f))	// �^�C�g���̈ʒu
#define TITLE_NAME_LOCUS_COUNT		(5)										// �^�C�g���̎c�����o���J�E���g
#define TITLE_NAME_MOVE				(40.0f)									// �^�C�g���̈ړ���
#define TITLE_NAME_LOCUS_ALPHA		(0.75f)									// �c���̓����x
#define TITLE_NAME_LOCUS_LIFE		(15)									// �c���̎���

//============================
// �R���X�g���N�^
//============================
CTitleName::CTitleName() : CObject2D(CObject2D::TYPE_LANDUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_MOVE;		// ���
	m_nStateCount = 0;			// ��ԃJ�E���g
}

//============================
// �f�X�g���N�^
//============================
CTitleName::~CTitleName()
{

}

//============================
// ����������
//============================
HRESULT CTitleName::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_state = STATE_MOVE;			// ���
	m_nStateCount = 0;			// ��ԃJ�E���g

	// �S�Ă̒l��ݒ肷��
	SetPos(TITLE_NAME_POS);					// �ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ����
	SetSize(TITLE_NAME_SIZE);				// �T�C�Y�ݒ�
	SetLength();							// �����ݒ�
	SetAngle();								// �����ݒ�

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(TITLE_NAME_TEXTURE));

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CTitleName::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CTitleName::Update(void)
{
	switch (m_state)
	{
	case STATE_MOVE:		// �ړ����

		// �ړ���ԏ���
		MoveState();

		break;

	case STATE_BRAKE:

		// �u���[�L��ԏ���
		BrakeState();

		break;
	}
}

//============================
// �`�揈��
//============================
void CTitleName::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//============================
//��������
//============================
CTitleName* CTitleName::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CTitleName* pTitleName = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTitleName == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pTitleName = new CTitleName;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pTitleName != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pTitleName->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�^�C�g���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pTitleName;
}

//============================
// �ړ���ԏ���
//============================
void CTitleName::MoveState(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �ʒu�����Z����
	pos.x += TITLE_NAME_MOVE;

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % TITLE_NAME_LOCUS_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

		// �c���̐�������
		CLocus2D::Create(pos, GetSize(), TITLE_NAME_LOCUS_ALPHA, TITLE_NAME_LOCUS_LIFE, GetTexIdx());
	}

	if (pos.x >= SCREEN_WIDTH * 0.5f)
	{ // �ʒu�����S�𒴂����ꍇ

		// X���W��␳����
		pos.x = SCREEN_WIDTH * 0.5f;

		// ��Ԃ�ݒ肷��
		m_state = STATE_BRAKE;

		// �w���𐶐�����
		CTitleSelect::Create();
	}

	// �ʒu���X�V����
	SetPos(pos);

	// ���_���̏�����
	SetVertex();
}

//============================
// �u���[�L��ԏ���
//============================
void CTitleName::BrakeState(void)
{

}