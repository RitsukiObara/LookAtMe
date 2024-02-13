//============================================
//
// �J�ߌ��tUI����[praiseUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "game.h"
#include "object2D.h"
#include "number.h"
#include "combo.h"
#include "input.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define WORD_TEXTURE		"data/TEXTURE/Combo.png"				// �R���{�̕����̃e�N�X�`��
#define MULTI_TEXTURE		"data/TEXTURE/Multi.png"				// �~�}�[�N�̃e�N�X�`��
#define NUMBER_TEXTURE		"data/TEXTURE/Number.png"				// �����̃e�N�X�`��
#define WORD_POS			(D3DXVECTOR3(1050.0f, 50.0f, 0.0f))		// ���t�̈ʒu
#define WORD_SIZE			(D3DXVECTOR3(60.0f, 30.0f, 0.0f))		// ���t�̃T�C�Y
#define MULTI_POS			(D3DXVECTOR3(1130.0f, 50.0f, 0.0f))		// �~�}�[�N�̈ʒu
#define MULTI_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// �~�}�[�N�̃T�C�Y
#define NUMBER_POS			(D3DXVECTOR3(1160.0f, 50.0f, 0.0f))		// �����̈ʒu
#define NUMBER_SIZE			(D3DXVECTOR3(10.0f, 20.0f, 0.0f))		// �����̃T�C�Y
#define NUMBER_SHIFT		(20.0f)									// �����̂��炷��
#define MAX_COMBO_VALUE		(999)									// �R���{�̍ő�l

//============================
// �R���X�g���N�^
//============================
CCombo::CCombo() : CObject(CObject::TYPE_PRAISEUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = nullptr;		// 2D�|���S���̏��
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		m_aNumber[nCntNum].pNumber = nullptr;	// �����̏��
		m_aNumber[nCntNum].bDisp = false;		// �`���
	}
	m_nCombo = 0;								// �R���{��
}

//============================
// �f�X�g���N�^
//============================
CCombo::~CCombo()
{

}

//============================
// ����������
//============================
HRESULT CCombo::Init(void)
{
	// �S�Ă̒l���N���A����
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		m_apObject2D[nCntObj] = m_apObject2D[nCntObj]->Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);		// 2D�|���S���̏��

		if (m_apObject2D[nCntObj] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			switch (nCntObj)
			{
			case TYPE_WORD:		// ���t

				// ����ݒ肷��
				m_apObject2D[nCntObj]->SetPos(WORD_POS);						// �ʒu
				m_apObject2D[nCntObj]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ����
				m_apObject2D[nCntObj]->SetSize(WORD_SIZE);						// �T�C�Y

				// �e�N�X�`���̊��蓖�ď���
				m_apObject2D[nCntObj]->BindTexture(CManager::GetTexture()->Regist(WORD_TEXTURE));

				break;

			case TYPE_MULTI:	// �~�}�[�N

				// ����ݒ肷��
				m_apObject2D[nCntObj]->SetPos(MULTI_POS);						// �ʒu
				m_apObject2D[nCntObj]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ����
				m_apObject2D[nCntObj]->SetSize(MULTI_SIZE);						// �T�C�Y

				// �e�N�X�`���̊��蓖�ď���
				m_apObject2D[nCntObj]->BindTexture(CManager::GetTexture()->Regist(MULTI_TEXTURE));

				break;
			}

			m_apObject2D[nCntObj]->SetAngle();		// ����
			m_apObject2D[nCntObj]->SetLength();		// ����

			m_apObject2D[nCntObj]->SetVertex();		// ���_���̐ݒ菈��
		}
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		// �����̐�������
		m_aNumber[nCntNum].pNumber = new CNumber(TYPE_NONE, PRIORITY_UI);

		if (m_aNumber[nCntNum].pNumber != nullptr)
		{ // �����̃|�C���^�� NULL ����Ȃ��ꍇ

			if (SUCCEEDED(m_aNumber[nCntNum].pNumber->Init()))
			{ // �����̏������ɐ��������ꍇ

				// ���̐ݒ菈��
				m_aNumber[nCntNum].pNumber->SetPos(D3DXVECTOR3(NUMBER_POS.x + (NUMBER_SHIFT * nCntNum), NUMBER_POS.y, NUMBER_POS.z));			// �ʒu
				m_aNumber[nCntNum].pNumber->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// ����
				m_aNumber[nCntNum].pNumber->SetSize(NUMBER_SIZE);		// �T�C�Y
				m_aNumber[nCntNum].pNumber->SetAngle();		// ����
				m_aNumber[nCntNum].pNumber->SetLength();	// ����

				m_aNumber[nCntNum].pNumber->SetNumber(0);	// ����
				m_aNumber[nCntNum].pNumber->SetType(CNumber::TYPE_DECIMAL);		// ���

				m_aNumber[nCntNum].pNumber->SetVertex();	// ���_���̐ݒ菈��

				// �e�N�X�`���̊��蓖�ď���
				m_aNumber[nCntNum].pNumber->BindTexture(CManager::GetTexture()->Regist(NUMBER_TEXTURE));

				m_aNumber[nCntNum].bDisp = false;			// �`���
			}
		}
	}

	m_nCombo = 0;								// �R���{��

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CCombo::Uninit(void)
{
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �I������
			m_apObject2D[nCntObj]->Uninit();
			m_apObject2D[nCntObj] = nullptr;
		}
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		if (m_aNumber[nCntNum].pNumber != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �I������
			m_aNumber[nCntNum].pNumber->Uninit();
			m_aNumber[nCntNum].pNumber = nullptr;
		}
	}

	// �j������
	Release();
}

//============================
// �X�V����
//============================
void CCombo::Update(void)
{
	// �`�攻�菈��
	DispDecide();

	// �v�Z����
	Calculation();

	// �S�Ă̒l������������
	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		// �e�N�X�`�����W�̐ݒ菈��
		m_aNumber[nCntNum].pNumber->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, m_aNumber[nCntNum].pNumber->GetNumber());
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_0) == true)
	{
		// �R���{�������Z����
		m_nCombo++;
	}
}

//============================
// �`�揈��
//============================
void CCombo::Draw(void)
{
	for (int nCntObj = 0; nCntObj < TYPE_MAX; nCntObj++)
	{
		if (m_apObject2D[nCntObj] != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �I������
			m_apObject2D[nCntObj]->Draw();
		}
	}

	for (int nCntNum = 0; nCntNum < COMBO_DIGIT; nCntNum++)
	{
		if (m_aNumber[nCntNum].pNumber != nullptr &&
			m_aNumber[nCntNum].bDisp == true)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �I������
			m_aNumber[nCntNum].pNumber->Draw();
		}
	}
}

//============================
// �v�Z����
//============================
void CCombo::Calculation(void)
{
	// ���[�J���ϐ��錾
	int nNum[COMBO_DIGIT];

	// 10�i���̌v�Z����
	DecimalCalculation(COMBO_DIGIT, m_nCombo, &nNum[0]);

	for (int nCnt = 0; nCnt < COMBO_DIGIT; nCnt++)
	{
		// ������ݒ肷��
		m_aNumber[nCnt].pNumber->SetNumber(nNum[nCnt]);
	}
}

//============================
// �`�攻�菈��
//============================
void CCombo::DispDecide(void)
{
	if (m_nCombo >= MAX_COMBO_VALUE)
	{ // �R���{�̍ő�l�𒴂����ꍇ

		// �␳����
		m_nCombo = MAX_COMBO_VALUE;
	}

	for (int nCnt = 0; nCnt < COMBO_DIGIT; nCnt++)
	{
		if (m_nCombo >= pow(10, (COMBO_DIGIT - (nCnt + 1))))
		{ // �R���{������萔�𒴂��Ă��Ȃ������ꍇ

			// �`�悷��
			m_aNumber[nCnt].bDisp = true;
		}
		else
		{ // ��L�ȊO

			// �`�悵�Ȃ�
			m_aNumber[nCnt].bDisp = false;
		}
	}
}

//============================
//��������
//============================
CCombo* CCombo::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCombo* pCombo = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pCombo == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCombo = new CCombo;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCombo != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCombo->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�R���{�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
	return pCombo;
}