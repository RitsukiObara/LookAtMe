//============================================
//
// ���̍�UI����[gold_bone_UI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "gold_bone_UI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

namespace
{
	const char* TEXTURE = "data\\TEXTURE\\BoneMark.png";					// �e�N�X�`��
	const D3DXVECTOR3 MARK_SIZE = D3DXVECTOR3(30.0f, 20.0f, 0.0f);			// �}�[�N�̃T�C�Y
	const D3DXVECTOR3 INIT_MARK_POS = D3DXVECTOR3(40.0f, 80.0f, 0.0f);		// �}�[�N�̏����ʒu
	const D3DXVECTOR3 EXTEND_MARK_SIZE = D3DXVECTOR3(45.0f, 30.0f, 0.0f);	// �}�[�N�̊g�厞�̃T�C�Y
	const float MARK_SHIFT = 70.0f;											// �}�[�N�̂��炷��
	const D3DXCOLOR SHADOW_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);			// �e�̐F
	const D3DXVECTOR3 STATE_CORRECT = D3DXVECTOR3(1.5f, 1.0f, 0.0f);		// ��Ԃɂ��␳�̔{��
}

//========================
// �R���X�g���N�^
//========================
CGoldBoneUI::CGoldBoneUI() : CObject(TYPE_GOLDBONEUI, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		m_aBoneMark[nCnt].pShadow = nullptr;		// �e�̏��
		m_aBoneMark[nCnt].pMark = nullptr;			// �e�̃}�[�N
		m_aBoneMark[nCnt].state = STATE_NONE;		// ���
	}
}

//========================
// �f�X�g���N�^
//========================
CGoldBoneUI::~CGoldBoneUI()
{

}

//========================
// ����������
//========================
HRESULT CGoldBoneUI::Init(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow == nullptr)
		{ // �e�̏�� NULL ����Ȃ��ꍇ

			// �e�̏��
			m_aBoneMark[nCnt].pShadow = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}

		m_aBoneMark[nCnt].pMark = nullptr;			// �}�[�N�̏��
		m_aBoneMark[nCnt].state = STATE_NONE;		// ���
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CGoldBoneUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow != nullptr)
		{ // �e�̏�� NULL ����Ȃ��ꍇ

			// �e�̏��̏I��
			m_aBoneMark[nCnt].pShadow->Uninit();
			m_aBoneMark[nCnt].pShadow = nullptr;
		}

		if (m_aBoneMark[nCnt].pMark != nullptr)
		{ // �}�[�N�̏�� NULL ����Ȃ��ꍇ

			// �}�[�N�̏��̏I��
			m_aBoneMark[nCnt].pMark->Uninit();
			m_aBoneMark[nCnt].pMark = nullptr;
		}
	}

	// �{�̂̏I������
	Release();
}

//========================
// �X�V����
//========================
void CGoldBoneUI::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		switch (m_aBoneMark[nCnt].state)
		{
		case STATE_NONE:		// �����

			break;

		case STATE_EXTEND:		// �g����

			if (m_aBoneMark[nCnt].pMark != nullptr)
			{ // �}�[�N�̏�� NULL ����Ȃ��ꍇ

				// �T�C�Y���擾����
				D3DXVECTOR3 size = m_aBoneMark[nCnt].pMark->GetSize();

				// �ϓ��ȕ␳����
				if (useful::FrameCorrect(EXTEND_MARK_SIZE.x, &size.x, STATE_CORRECT.x) == true ||
					useful::FrameCorrect(EXTEND_MARK_SIZE.y, &size.y, STATE_CORRECT.y) == true)
				{ // ���̃T�C�Y�ɂȂ����ꍇ

					// �k����Ԃɂ���
					m_aBoneMark[nCnt].state = STATE_SHRINK;
				}

				// �T�C�Y��K�p����
				m_aBoneMark[nCnt].pMark->SetSize(size);
			}

			break;

		case STATE_SHRINK:		// �k�����

			if (m_aBoneMark[nCnt].pMark != nullptr)
			{ // �}�[�N�̏�� NULL ����Ȃ��ꍇ

				// �T�C�Y���擾����
				D3DXVECTOR3 size = m_aBoneMark[nCnt].pMark->GetSize();

				// �ϓ��ȕ␳����
				if (useful::FrameCorrect(MARK_SIZE.x, &size.x, STATE_CORRECT.x) == true ||
					useful::FrameCorrect(MARK_SIZE.y, &size.y, STATE_CORRECT.y) == true)
				{ // ���̃T�C�Y�ɂȂ����ꍇ

					// ��~��Ԃɂ���
					m_aBoneMark[nCnt].state = STATE_STOP;
				}

				// �T�C�Y��K�p����
				m_aBoneMark[nCnt].pMark->SetSize(size);
			}

			break;

		case STATE_STOP:		// ��~���

			break;

		default:

			// ��~
			assert(false);

			break;
		}

		if (m_aBoneMark[nCnt].pMark != nullptr)
		{ // �}�[�N�� NULL ����Ȃ��ꍇ

			// ���_���W�̐ݒ菈��
			m_aBoneMark[nCnt].pMark->SetVertex();
		}
	}
}

//========================
// �`�揈��
//========================
void CGoldBoneUI::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow != nullptr)
		{ // �e�̏�� NULL ����Ȃ��ꍇ

			// �e�̏��̏I��
			m_aBoneMark[nCnt].pShadow->Draw();
		}
	}

	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pMark != nullptr)
		{ // �}�[�N�̏�� NULL ����Ȃ��ꍇ

			// �}�[�N�̏��̏I��
			m_aBoneMark[nCnt].pMark->Draw();
		}
	}
}

//========================
// ���̐ݒ菈��
//========================
void CGoldBoneUI::SetData(void)
{
	// �}�[�N�̈ʒu��錾����
	D3DXVECTOR3 posMark = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pShadow != nullptr)
		{ // �e�̏�� NULL ����Ȃ��ꍇ

			// �}�[�N�̈ʒu��ݒ肷��
			posMark.x = INIT_MARK_POS.x + (MARK_SHIFT * nCnt);
			posMark.y = INIT_MARK_POS.y;
			posMark.z = 0.0f;

			// �ݒ菈��
			m_aBoneMark[nCnt].pShadow->SetPos(posMark);				// �ʒu
			m_aBoneMark[nCnt].pShadow->SetPosOld(posMark);			// �O��̈ʒu
			m_aBoneMark[nCnt].pShadow->SetRot(NONE_D3DXVECTOR3);	// ����
			m_aBoneMark[nCnt].pShadow->SetSize(MARK_SIZE);			// �T�C�Y
			m_aBoneMark[nCnt].pShadow->SetAngle();					// ����
			m_aBoneMark[nCnt].pShadow->SetLength();					// ����

			// �e�N�X�`���̊��蓖�ď���
			m_aBoneMark[nCnt].pShadow->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// ���_���W�̐ݒ菈��
			m_aBoneMark[nCnt].pShadow->SetVertex();

			// ���_�J���[�̐ݒ菈��
			m_aBoneMark[nCnt].pShadow->SetVtxColor(SHADOW_COL);
		}
	}
}

//========================
// ��������
//========================
CGoldBoneUI* CGoldBoneUI::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGoldBoneUI* pUI = nullptr;	// UI�̃C���X�^���X�𐶐�

	if (pUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pUI = new CGoldBoneUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pUI->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// UI�̃|�C���^��Ԃ�
	return pUI;
}

//========================
// ���̍��̃Q�b�g����
//========================
void CGoldBoneUI::GetGoldBone(void)
{
	// �}�[�N�̈ʒu��錾����
	D3DXVECTOR3 posMark = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < MAX_BONE_MARK; nCnt++)
	{
		if (m_aBoneMark[nCnt].pMark == nullptr &&
			m_aBoneMark[nCnt].state == STATE_NONE)
		{ // �}�[�N���܂��o�����Ă��Ȃ��ԍ��̏ꍇ

			// �}�[�N�̈ʒu��ݒ肷��
			posMark.x = INIT_MARK_POS.x + (MARK_SHIFT * nCnt);
			posMark.y = INIT_MARK_POS.y;
			posMark.z = 0.0f;

			// �}�[�N�̏��
			m_aBoneMark[nCnt].pMark = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

			// �ݒ菈��
			m_aBoneMark[nCnt].pMark->SetPos(posMark);				// �ʒu
			m_aBoneMark[nCnt].pMark->SetPosOld(posMark);			// �O��̈ʒu
			m_aBoneMark[nCnt].pMark->SetRot(NONE_D3DXVECTOR3);		// ����
			m_aBoneMark[nCnt].pMark->SetSize(NONE_D3DXVECTOR3);		// �T�C�Y
			m_aBoneMark[nCnt].pMark->SetAngle();					// ����
			m_aBoneMark[nCnt].pMark->SetLength();					// ����

			// �e�N�X�`���̊��蓖�ď���
			m_aBoneMark[nCnt].pMark->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// ���_���W�̐ݒ菈��
			m_aBoneMark[nCnt].pMark->SetVertex();

			// �g���Ԃ�ݒ肷��
			m_aBoneMark[nCnt].state = STATE_EXTEND;

			// �����o��
			break;
		}
	}
}