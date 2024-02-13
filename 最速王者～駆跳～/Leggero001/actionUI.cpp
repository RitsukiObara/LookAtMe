//============================================
//
// �A�N�V����UI����[actionUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"
#include "manager.h"
#include "renderer.h"
#include "actionUI.h"
#include "texture.h"
#include "useful.h"

#include "praiseUI.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define ACTIONUI_GROUND_TEXTURE		"data/TEXTURE/ActUI_Back.png"					// �w�i�̃e�N�X�`��
#define ACTIONUI_POS				(D3DXVECTOR3(1080.0f, 150.0f, 0.0f))			// �A�N�V����UI�̈ʒu

#define ACTIONUI_WK_SIZE			(D3DXVECTOR3(175.0f, 50.0f, 0.0f))				// �ǃL�b�NUI�̃T�C�Y
#define ACTIONUI_LT_SIZE			(D3DXVECTOR3(200.0f, 50.0f, 0.0f))				// ���n�^�[�{UI�̃T�C�Y
#define ACTIONUI_SJ_SIZE			(D3DXVECTOR3(200.0f, 50.0f, 0.0f))				// ���W�����vUI�̃T�C�Y
#define ACTIONUI_NJ_SIZE			(D3DXVECTOR3(240.0f, 40.0f, 0.0f))				// �X���W�����v�̃T�C�Y
#define ACTIONUI_NA_SIZE			(D3DXVECTOR3(200.0f, 40.0f, 0.0f))				// �U���̃T�C�Y

#define ACTIONUI_SIZE_SUB			(0.5f)											// �A�N�V����UI�̃T�C�Y�̏k����
#define ACTIONUI_ADD_ROT			(0.1f)											// �A�N�V����UI�̌����̉��Z��
#define ACTIONUI_SIZE_DELETE		(0.01f)											// �A�N�V����UI�̏����Ώۂ̃T�C�Y
#define ACTIONUI_GROUND_SIZE_MOVE	(D3DXVECTOR3(-10.0f, -10.0f, 0.0f))				// �A�N�V����UI�̔w�i�̃T�C�Y�̈ړ���

#define ACTIONUI_APPEAR_COUNT		(30)											// �A�N�V����UI�̏o����Ԃ̃J�E���g��
#define ACTIONUI_DISP_COUNT			(10)											// �A�N�V����UI�̕\����Ԃ̃J�E���g��

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CActionUI::m_apTexturename[CActionUI::TYPE_MAX] =				// �e�N�X�`���̃p�X��
{
	"data/TEXTURE/ActUI_WK.png",		// �ǃL�b�N
	"data/TEXTURE/ActUI_LT.png",		// ���n�^�[�{
	"data/TEXTURE/ActUI_SJ.png",		// ���W�����v
	"data/TEXTURE/ActUI_NJ.png",		// �X���W�����v
	"data/TEXTURE/ActUI_NA.png",		// �U��
};

const D3DXVECTOR3 CActionUI::m_aSize[CActionUI::TYPE_MAX] =		// ���ꂼ��̑傫��
{
	ACTIONUI_WK_SIZE,		// �ǃL�b�N
	ACTIONUI_LT_SIZE,		// ���n�^�[�{
	ACTIONUI_SJ_SIZE,		// ���W�����v
	ACTIONUI_NJ_SIZE,		// �X���W�����v
	ACTIONUI_NA_SIZE,		// �U��
};

//============================
// �R���X�g���N�^
//============================
CActionUI::CActionUI() : CObject(TYPE_ACTIONUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		m_apActUI[nCntAct] = nullptr;				// �A�N�V����UI�̏��
	}

	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�̃T�C�Y
	m_GroundMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �w�i�̃T�C�Y�̈ړ���
	m_nStateCount = 0;								// ��ԃJ�E���g
	m_state = STATE_APPEAR;							// ���
	m_nScore = 0;									// ���_
}

//============================
// �f�X�g���N�^
//============================
CActionUI::~CActionUI()
{

}

//============================
// ����������
//============================
HRESULT CActionUI::Init(void)
{
	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		if (m_apActUI[nCntAct] == nullptr)
		{ // ��� NULL �̏ꍇ

			// �A�N�V����UI�̐�������
			m_apActUI[nCntAct] = m_apActUI[nCntAct]->Create(CObject2D::TYPE_NONE, TYPE_ACTIONUI, PRIORITY_UI);
		}

		if (m_apActUI[nCntAct] != nullptr)
		{ // ��� NULL ����Ȃ��ꍇ

			if (FAILED(m_apActUI[nCntAct]->Init()))
			{ // �������Ɏ��s�����ꍇ

				// ���s��Ԃ�
				return E_FAIL;
			}
		}
	}

	// �S�Ă̒l���N���A����
	m_nStateCount = 0;				// ��ԃJ�E���g
	m_state = STATE_APPEAR;			// ���
	m_nScore = 0;					// ���_

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CActionUI::Uninit(void)
{
	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		// ���̏I��
		m_apActUI[nCntAct]->Uninit();
		m_apActUI[nCntAct] = nullptr;
	}

	// �{�̂̏I��
	Release();
}

//============================
// �X�V����
//============================
void CActionUI::Update(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	switch (m_state)
	{
	case STATE_APPEAR:		// �o�����

		// �T�C�Y�̕ω�����
		SizeChange(POLY_ACT);

		// �w�i�̃o�E���h����
		GroundBound();

		if (m_nStateCount % ACTIONUI_APPEAR_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

			// �o����ԕω�����
			AppearChange(POLY_ACT);

			// �w�i�̕ω�����
			GroundChange();
		}

		break;

	case STATE_DISP:		// �\�����

		// �w�i�̕\����Ԏ��̏���
		GroundDispProcess();

		if (m_nStateCount % ACTIONUI_DISP_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

			// �\������
			DispChange();
		}

		break;

	case STATE_SMALLER:		// �k�����

		for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
		{
			// �T�C�Y�ω�����
			SizeChange(nCnt);

			// �����ω�����
			Rotation(nCnt);
		}

		if (m_apActUI[POLY_ACT]->GetSize().x <= ACTIONUI_SIZE_DELETE)
		{ // �T�C�Y����萔�����ɂȂ����ꍇ

			// �I������
			Uninit();

			// �J�ߌ��t�̐�������
			CPraiseUI::Create(m_nScore);

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;
	}

	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		// �����ݒ�
		m_apActUI[nCntAct]->SetLength();

		// �����ݒ�
		m_apActUI[nCntAct]->SetAngle();

		// ���_���̏�����
		m_apActUI[nCntAct]->SetVertexRot();
	}
}

//============================
// �`�揈��
//============================
void CActionUI::Draw(void)
{
	// �A���t�@�e�X�g��L���ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		// �`�揈��
		m_apActUI[nCntAct]->Draw();
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// �A���t�@�e�X�g�̎Q�ƒl�ݒ�
}

//============================
// ���̐ݒ菈��
//============================
void CActionUI::SetData(const TYPE type, const int nScore)
{
	// ���[�J���ϐ��錾
	float fRot = (rand() % 157 - 78) * 0.01f;		// ����

	for (int nCntAct = 0; nCntAct < POLY_MAX; nCntAct++)
	{
		if (m_apActUI[nCntAct] != nullptr)
		{ // ��� NULL ����Ȃ��ꍇ

			// �X�N���[���̐ݒ菈��
			m_apActUI[nCntAct]->SetPos(ACTIONUI_POS);					// �ʒu�ݒ�
			m_apActUI[nCntAct]->SetRot(D3DXVECTOR3(0.0f, 0.0f, fRot));	// �����ݒ�
			m_apActUI[nCntAct]->SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �T�C�Y�ݒ�

			m_apActUI[nCntAct]->SetLength();			// �����ݒ�
			m_apActUI[nCntAct]->SetAngle();				// �����ݒ�

			// ���_���̏�����
			m_apActUI[nCntAct]->SetVertexRot();

			if (nCntAct == POLY_GROUND)
			{ // �w�i�̏ꍇ

				// �e�N�X�`���̓ǂݍ��ݏ���
				m_apActUI[nCntAct]->BindTexture(CManager::GetTexture()->Regist(ACTIONUI_GROUND_TEXTURE));
			}
			else if (nCntAct == POLY_ACT)
			{ // �A�N�V�������̏ꍇ

				// �e�N�X�`���̓ǂݍ��ݏ���
				m_apActUI[nCntAct]->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[type]));
			}
		}
	}

	// �S�Ă̒l��ݒ肷��
	m_nStateCount = 0;			// ��ԃJ�E���g
	m_state = STATE_APPEAR;		// ���
	m_nScore = nScore;			// ���_

	// �ڕW�̃T�C�Y��ݒ肷��
	m_sizeDest = m_aSize[type];

	// �w�i�̃T�C�Y�̈ړ���
	m_GroundMove = ACTIONUI_GROUND_SIZE_MOVE;
}

//============================
// �T�C�Y�ω�����
//============================
void CActionUI::SizeChange(const int nCnt)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apActUI[nCnt]->GetSize();	// �T�C�Y���擾����
	
	if (m_nStateCount <= (ACTIONUI_APPEAR_COUNT / 2))
	{ // ��ԃJ�E���g����萔�ȉ��������ꍇ

		// �␳����
		Correct(m_sizeDest.x * 1.2f, &size.x, ACTIONUI_SIZE_SUB);
		Correct(m_sizeDest.y * 1.2f, &size.y, ACTIONUI_SIZE_SUB);
	}
	else
	{ // ��L�ȊO

		// �␳����
		Correct(m_sizeDest.x, &size.x, ACTIONUI_SIZE_SUB);
		Correct(m_sizeDest.y, &size.y, ACTIONUI_SIZE_SUB);
	}

	// �T�C�Y�̐ݒ菈��
	m_apActUI[nCnt]->SetSize(size);
}

//============================
// �o����ԕω�����
//============================
void CActionUI::AppearChange(const int nCnt)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apActUI[nCnt]->GetSize();	// �T�C�Y���擾����

	// ��ԃJ�E���g������������
	m_nStateCount = 0;

	// �\����Ԃɂ���
	m_state = STATE_DISP;

	// �T�C�Y��ݒ�(�␳)����
	size = m_sizeDest;

	// �T�C�Y�̐ݒ菈��
	m_apActUI[nCnt]->SetSize(size);
}

//============================
// �\����ԕω�����
//============================
void CActionUI::DispChange(void)
{
	// �ڕW�̃T�C�Y��ݒ肷��
	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ��ԃJ�E���g������������
	m_nStateCount = 0;

	// �k����Ԃɂ���
	m_state = STATE_SMALLER;
}

//============================
// �����ω�����
//============================
void CActionUI::Rotation(const int nCnt)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = m_apActUI[nCnt]->GetRot();		// �������擾����

	// �����ɉ��Z����
	rot.z += ACTIONUI_ADD_ROT;

	// �����̐��K��
	RotNormalize(&rot.z);

	// ������ݒ肷��
	m_apActUI[nCnt]->SetRot(rot);
}

//============================
// �w�i�̃o�E���h����
//============================
void CActionUI::GroundBound(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apActUI[POLY_GROUND]->GetSize();	// �T�C�Y���擾����

	if (size.x >= m_sizeDest.x)
	{ // �T�C�Y���ڕW�̃T�C�Y�ȏ�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size.x = m_sizeDest.x;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.x *= -1;
	}

	if (size.x <= 0.0f)
	{ // �T�C�Y��0.0f�ȉ��ɂȂ����ꍇ

		// �T�C�Y��␳����
		size.x = 0.0f;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.x *= -1;
	}

	if (size.y >= m_sizeDest.y)
	{ // �T�C�Y���ڕW�̃T�C�Y�ȏ�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size.y = m_sizeDest.y;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.y *= -1;
	}

	if (size.y <= 0.0f)
	{ // �T�C�Y��0.0f�ȉ��ɂȂ����ꍇ

		// �T�C�Y��␳����
		size.y = 0.0f;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.y *= -1;
	}

	// �T�C�Y�����Z����
	size += m_GroundMove;

	// �T�C�Y�̐ݒ菈��
	m_apActUI[POLY_GROUND]->SetSize(size);
}

//============================
// �w�i�̏�ԕω�����
//============================
void CActionUI::GroundChange(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apActUI[POLY_GROUND]->GetSize();	// �T�C�Y���擾����

	if (size.x <= m_sizeDest.x)
	{ // �T�C�Y���ڕW�̃T�C�Y�ȉ��������ꍇ

		// �T�C�Y�̈ړ��ʂ�ݒ肷��
		m_GroundMove.x = -ACTIONUI_GROUND_SIZE_MOVE.x;
	}
	else
	{ // ��L�ȊO

		// �T�C�Y��␳����
		size.x = m_sizeDest.x;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.x = 0.0f;
	}

	if (size.y <= m_sizeDest.y)
	{ // �T�C�Y���ڕW�̃T�C�Y�ȉ��������ꍇ

		// �T�C�Y�̈ړ��ʂ�ݒ肷��
		m_GroundMove.y = -ACTIONUI_GROUND_SIZE_MOVE.y;
	}
	else
	{ // ��L�ȊO

		// �T�C�Y��␳����
		size.y = m_sizeDest.y;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.y = 0.0f;
	}
}

//============================
// �w�i�̕\����Ԏ�����
//============================
void CActionUI::GroundDispProcess(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apActUI[POLY_GROUND]->GetSize();	// �T�C�Y���擾����

	if (size.x >= m_sizeDest.x)
	{ // �T�C�Y���ڕW�̃T�C�Y�ȏ�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size.x = m_sizeDest.x;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.x = 0.0f;
	}

	if (size.y >= m_sizeDest.y)
	{ // �T�C�Y���ڕW�̃T�C�Y�ȏ�ɂȂ����ꍇ

		// �T�C�Y��␳����
		size.y = m_sizeDest.y;

		// X���̈ړ��ʂ�ύX����
		m_GroundMove.y = 0.0f;
	}

	// �T�C�Y�����Z����
	size += m_GroundMove;

	// �T�C�Y�̐ݒ菈��
	m_apActUI[POLY_GROUND]->SetSize(size);
}

//============================
//��������
//============================
CActionUI* CActionUI::Create(const TYPE type, const int nScore)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CActionUI* pActionUI = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pActionUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pActionUI = new CActionUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pActionUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pActionUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�A�N�V����UI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pActionUI->SetData(type, nScore);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �A�N�V����UI�̃|�C���^��Ԃ�
	return pActionUI;
}