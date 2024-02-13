//============================================
//
// ���nUI����[landUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "renderer.h"
#include "landUI.h"
#include "texture.h"

#include "locus2D.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define LANDUI_DEST_POS			(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))		// ���nUI�̈ʒu
#define LANDUI_INIT_POS			(D3DXVECTOR3(1600.0f, 500.0f, 0.0f))		// ���nUI�̈ʒu
#define LANDUI_SIZE				(D3DXVECTOR3(150.0f, 60.0f, 0.0f))			// ���nUI��BAD�̃T�C�Y
#define LANDUI_DISP_CNT			(10)										// �\����ԂɈڍs����J�E���g��
#define LANDUI_DELETE_CNT		(40)										// ������ԂɈڍs����J�E���g��
#define LANDUI_MOVE				((LANDUI_DEST_POS.x - LANDUI_INIT_POS.x) / LANDUI_DISP_CNT)		// �ړ���
#define LANDUI_LOCUS_ALPHA		(0.5f)										// �c���̓����x
#define LANDUI_LOCUS_LIFE		(7)											// �c���̎���
#define LANDUI_LOCUS_INTERVAL	(2)											// �c���̎���

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CLandUI::m_apTexturename[CLandUI::TYPE_MAX] =				// �e�N�X�`���̃p�X��
{
	"data/TEXTURE/BOO.png",												// BOO
	"data/TEXTURE/COOL.png",											// COOL
};

//============================
// �R���X�g���N�^
//============================
CLandUI::CLandUI() : CObject2D(CObject2D::TYPE_LANDUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_BAD;				// ���
	m_state = STATE_APPEAR;			// ���
	m_nStateCount = 0;				// ��ԃJ�E���g
}

//============================
// �f�X�g���N�^
//============================
CLandUI::~CLandUI()
{

}

//============================
// ����������
//============================
HRESULT CLandUI::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_type = TYPE_BAD;				// ���
	m_state = STATE_APPEAR;			// ���
	m_nStateCount = 0;				// ��ԃJ�E���g

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CLandUI::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CLandUI::Update(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	switch (m_state)
	{
	case STATE_APPEAR:		// �o�����

		// �ړ�����
		Move();

		// �\����Ԕ��菈��
		DispDecision();

		if (m_type == TYPE_GOOD && 
			m_nStateCount % LANDUI_LOCUS_INTERVAL == 0)
		{ // GOOD���A��莞�Ԃ��Ƃ�

			// �c���̐�������
			CLocus2D::Create(GetPos(), GetSize(), LANDUI_LOCUS_ALPHA, LANDUI_LOCUS_LIFE, GetTexIdx());
		}

		break;

	case STATE_DISP:		// �\������

		if (m_nStateCount >= LANDUI_DELETE_CNT)
		{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

			// ������Ԃɂ���
			m_state = STATE_DELETE;

			// ��ԃJ�E���g������������
			m_nStateCount = 0;
		}

		break;

	case STATE_DELETE:		// �������

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;

		break;

	default:				// ��L�ȊO

		// �x����
		MessageBox(NULL, "���nUI�ŗ\�����Ȃ���Ԃ����m�I", "�x���I", MB_ICONWARNING);

		break;
	}

	SetLength();							// �����ݒ�
	SetAngle();								// �����ݒ�

	// ���_���̏�����
	SetVertexRot();
}

//============================
// �`�揈��
//============================
void CLandUI::Draw(void)
{
	// �A���t�@�e�X�g��L���ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// �`�揈��
	CObject2D::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// �A���t�@�e�X�g�̎Q�ƒl�ݒ�
}

//============================
// ���̐ݒ菈��
//============================
void CLandUI::SetData(TYPE type)
{
	// ���[�J���ϐ��錾
	float fRot = (rand() % 101 - 50) * 0.01f;		// ����

	// �X�N���[���̐ݒ菈��
	SetPos(LANDUI_INIT_POS);						// �ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, fRot));	// �����ݒ�
	SetSize(LANDUI_SIZE);			// �T�C�Y�ݒ�

	SetLength();							// �����ݒ�
	SetAngle();								// �����ݒ�

	// ���_���̏�����
	SetVertexRot();

	// �S�Ă̒l���N���A����
	m_type = type;				// ���
	m_state = STATE_APPEAR;			// ���
	m_nStateCount = 0;				// ��ԃJ�E���g

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(m_apTexturename[m_type]));
}

//============================
//��������
//============================
CLandUI* CLandUI::Create(TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CLandUI* pLandUI = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pLandUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLandUI = new CLandUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLandUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLandUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���nUI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pLandUI->SetData(type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pLandUI;
}

//============================
// �ړ�����
//============================
void CLandUI::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	// �ړ��ʂ����Z����
	pos.x += LANDUI_MOVE;

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//============================
// �\����Ԕ��菈��
//============================
void CLandUI::DispDecision(void)
{
	if (m_nStateCount >= LANDUI_DISP_CNT)
	{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

		// �ʒu��ݒ肷��
		SetPos(LANDUI_DEST_POS);

		// �\����Ԃɂ���
		m_state = STATE_DISP;

		// ��ԃJ�E���g������������
		m_nStateCount = 0;
	}
}