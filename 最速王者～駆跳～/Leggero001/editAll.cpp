//===========================================
//
// �S�G�f�B�b�g�̃��C������[editAll.cpp]
// Author ��������
//
//===========================================
#ifdef _DEBUG	// �f�o�b�O����

//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"

#include "editAll.h"
#include "edit.h"
#include "bgEdit.h"
#include "obstacleEdit.h"
#include "cameraEdit.h"
#include "range_move.h"
#include "player.h"
#include "objectX.h"
#include "Item.h"
#include "item_generator.h"

// �}�N����`
#define OBJECT_DELETE_RANGE		(70.0f)									// �I�u�W�F�N�g�̏����͈�
#define CAMERAEDIT_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// �J�����G�f�B�b�g�̐������͈̔�

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CEdit* CEditAll::m_pEdit = nullptr;						// �G�f�B�b�g�̃|�C���^
CBgEdit* CEditAll::m_pBgEdit = nullptr;					// �w�i�G�f�B�b�g�̃|�C���^
CObstacleEdit* CEditAll::m_pObstacleEdit = nullptr;		// ��Q���G�f�B�b�g�̃|�C���^
CCameraEdit* CEditAll::m_pCameraEdit = nullptr;			// �J�����G�f�B�b�g�̃|�C���^
CRangeMove* CEditAll::m_pRangeMove = nullptr;			// �͈͈ړ��̃|�C���^

//==========================
// �R���X�g���N�^
//==========================
CEditAll::CEditAll()
{
	// �S�Ă̒l���N���A����
	m_pEdit = nullptr;			// �X�e�[�W�G�f�B�b�g�̃|�C���^
	m_pBgEdit = nullptr;		// �w�i�G�f�B�b�g�̃|�C���^
	m_pObstacleEdit = nullptr;	// ��Q���G�f�B�b�g�̃|�C���^
	m_pCameraEdit = nullptr;	// �J�����G�f�B�b�g�̃|�C���^
	m_pRangeMove = nullptr;		// �͈͈ړ��̃|�C���^

	// �S�Ă̒l���N���A����
	m_style = STYLE_BLOCK;		// �X�^�C��
	m_saveStyle = STYLE_BLOCK;	// �ۑ��p�X�^�C��

	if (m_pEdit == nullptr)
	{ // �|�C���^�� NULL �̏ꍇ

		// ���������m�ۂ���
		m_pEdit = new CEdit;
	}
	else
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �x����
		MessageBox(NULL, "�G�f�B�b�g�̃|�C���^���g���Ă��܂��I", "�x���I", MB_ICONWARNING);
	}
}

//==========================
// �f�X�g���N�^
//==========================
CEditAll::~CEditAll()
{

}

//==========================
// ����������
//==========================
HRESULT CEditAll::Init(void)
{
	// �S�Ă̒l���N���A����
	m_style = STYLE_BLOCK;		// �X�^�C��
	m_saveStyle = STYLE_BLOCK;	// �ۑ��p�X�^�C��

	if (m_pEdit != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(m_pEdit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�G�f�B�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}

		// ���̐ݒ菈��
		m_pEdit->SetData(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	else
	{ // �|�C���^�� NULL �̏ꍇ

		// �x����
		MessageBox(NULL, "�G�f�B�b�g�̃|�C���^��NULL�I", "�x���I", MB_ICONWARNING);
	}

	// ������Ԃ�
	return S_OK;
}

//==========================
// �I������
//==========================
void CEditAll::Uninit(void)
{
	// �S��������
	AllDelete();
}

//==========================
// �X�V����
//==========================
void CEditAll::Update(void)
{
	switch (m_style)
	{
	case STYLE_BLOCK:		// �u���b�N(�X�e�[�W)

		// �X�V����
		m_pEdit->Update();

		break;

	case STYLE_BGOBJECT:	// �w�i�I�u�W�F�N�g

		// �X�V����
		m_pBgEdit->Update();

		break;

	case STYLE_OBSTACLE:	// ��Q��

		// �X�V����
		m_pObstacleEdit->Update();

		break;

	case STYLE_CAMERA:		// �J�����͈̔�

		// �X�V����
		m_pCameraEdit->Update();

		break;

	case STYLE_RANGEMOVE:	// �͈͈ړ�

		// �X�V����
		m_pRangeMove->Update();

		break;
	}

	if (m_style != STYLE_RANGEMOVE)
	{ // �͈͈ړ��̏ꍇ

		// �X�^�C���ύX����
		StyleChange();
	}

	// �A�C�e���̐ݒ菈��
	SetItem();

	// �A�C�e���̏�������
	DeleteItem();

	// �A�C�e��������̐ݒ菈��
	SetItemGenerator();

	// �A�C�e��������̏�������
	DeleteItemGenerator();

	// �͈͈ړ��̐ݒ菈��
	RangeMoveChange();

	// �f�o�b�O�\��
	CManager::GetDebugProc()->Print("F2�L�[�F�A�C�e���̐ݒu\nF3�L�[�F�A�C�e���̏���\nF4�L�[�F�A�C�e��������̐ݒu\n"
		"F5�L�[�F�A�C�e��������̏���\nBACKSPACE�L�[�F�͈͈ړ��̐؂�ւ�");
}

//==========================
// �`�揈��
//==========================
void CEditAll::Draw(void)
{
	switch (m_style)
	{
	case STYLE_BLOCK:		// �u���b�N(�X�e�[�W)

		// �`�揈��
		m_pEdit->Draw();

		break;

	case STYLE_BGOBJECT:	// �w�i�I�u�W�F�N�g

		// �`�揈��
		m_pBgEdit->Draw();

		break;

	case STYLE_OBSTACLE:	// ��Q��

		// �`�揈��
		m_pObstacleEdit->Draw();

		break;

	case STYLE_CAMERA:		// �J����

		// �`�揈��
		m_pCameraEdit->Draw();

		break;

	case STYLE_RANGEMOVE:	// �͈͈ړ�

		// �`�揈��
		m_pRangeMove->Draw();

		break;
	}
}

//==========================
// �X�^�C���̐ݒ菈��
//==========================
void CEditAll::StyleChange(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ۑ��p�̈ʒu�̕ϐ�

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{ // 6�L�[���������ꍇ

		// �ʒu�̕ۑ�����
		posSave = GetEditPos();

		// �S��������
		AllDelete();

		// �X�^�C����ݒ肷��
		m_style = (STYLE)((m_style + 1) % STYLE_RANGEMOVE);

		// �ݒ菈��
		Set(posSave);
	}
}

//==========================
// �͈͈ړ��̐ݒ菈��
//==========================
void CEditAll::RangeMoveChange(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_BACKSPACE) == true)
	{ // BACKSPACE�L�[���������ꍇ

		// �ʒu�̕ۑ�����
		pos = GetEditPos();

		if (m_style == STYLE_RANGEMOVE)
		{ // �͈͈ړ��������ꍇ

			// �S��������
			AllDelete();

			// �X�^�C����ݒ肷��
			m_style = m_saveStyle;

			// �ݒ菈��
			Set(pos);
		}
		else
		{ // ��L�ȊO

			// �ۑ��p�X�^�C����ݒ肷��
			m_saveStyle = m_style;

			// �X�^�C����ݒ肷��
			m_style = STYLE_RANGEMOVE;

			// ��������
			AllDelete();

			// �͈͂𐶐�
			m_pRangeMove = m_pRangeMove->Create(pos);
		}
	}
}

//==========================
// �A�C�e���̐ݒ菈��
//==========================
void CEditAll::SetItem(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{ // F2�L�[���������ꍇ

		// �A�C�e���𐶐�����
		CItem::Create(GetEditPos(), CItem::STATE_NONE);
	}
}

//==========================
// �A�C�e���̏�������
//==========================
void CEditAll::DeleteItem(void)
{
	// ���[�J���|�C���^�錾
	CObject* pObj = nullptr;	// �I�u�W�F�N�g�̃|�C���^
	CItem* pItem = nullptr;		// �A�C�e���̃|�C���^
	D3DXVECTOR3 posSave = GetEditPos();		// �ۑ��p�̈ʒu�̕ϐ�

	// �I�u�W�F�N�g���擾����
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_ITEM)
		{ // �I�u�W�F�N�g���A�C�e���������ꍇ

			// �A�C�e���̏����擾����
			pItem = dynamic_cast<CItem*>(pObj);

			if (pItem != nullptr)
			{ // �A�C�e���̏�� NULL ����Ȃ��ꍇ

				if (posSave.x >= pItem->GetPos().x - OBJECT_DELETE_RANGE &&
					posSave.x <= pItem->GetPos().x + OBJECT_DELETE_RANGE &&
					posSave.y >= pItem->GetPos().y - OBJECT_DELETE_RANGE &&
					posSave.y <= pItem->GetPos().y + OBJECT_DELETE_RANGE)
				{ // �͈͓��ɓ������ꍇ

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3) == true)
					{ // F3�L�[���������ꍇ

						// �폜����
						pItem->Uninit();
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObj->GetNext();
	}
}

//==========================
// �A�C�e��������̐ݒ菈��
//==========================
void CEditAll::SetItemGenerator(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F4) == true)
	{ // F4�L�[���������ꍇ

		// �A�C�e���𐶐�����
		CItemGenerator::Create(GetEditPos());
	}
}

//==========================
// �A�C�e��������̏�������
//==========================
void CEditAll::DeleteItemGenerator(void)
{
	// ���[�J���|�C���^�錾
	CObject* pObj = nullptr;	// �I�u�W�F�N�g�̃|�C���^
	CItemGenerator* pItem = nullptr;		// �A�C�e���̃|�C���^
	D3DXVECTOR3 posSave = GetEditPos();		// �ۑ��p�̈ʒu�̕ϐ�

	// �I�u�W�F�N�g���擾����
	pObj = CObject::GetTop(CObject::PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == CObject::TYPE_ITEMGENERATOR)
		{ // �I�u�W�F�N�g���A�C�e�������킾�����ꍇ

			// �A�C�e��������̏����擾����
			pItem = dynamic_cast<CItemGenerator*>(pObj);

			if (pItem != nullptr)
			{ // �A�C�e��������̏�� NULL ����Ȃ��ꍇ

				if (posSave.x >= pItem->GetPos().x - OBJECT_DELETE_RANGE &&
					posSave.x <= pItem->GetPos().x + OBJECT_DELETE_RANGE &&
					posSave.y >= pItem->GetPos().y - OBJECT_DELETE_RANGE &&
					posSave.y <= pItem->GetPos().y + OBJECT_DELETE_RANGE)
				{ // �͈͓��ɓ������ꍇ

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_F5) == true)
					{ // F5�L�[���������ꍇ

						// �폜����
						pItem->Uninit();
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObj->GetNext();
	}
}

//==========================
// �|�C���^�S��������
//==========================
void CEditAll::AllDelete(void)
{
	if (m_pEdit != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �I������
		m_pEdit->Uninit();

		// �|�C���^�� NULL �ɂ���
		m_pEdit = nullptr;
	}

	if (m_pBgEdit != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �I������
		m_pBgEdit->Uninit();

		// �|�C���^�� NULL �ɂ���
		m_pBgEdit = nullptr;
	}

	if (m_pObstacleEdit != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �I������
		m_pObstacleEdit->Uninit();

		// �|�C���^�� NULL �ɂ���
		m_pObstacleEdit = nullptr;
	}

	if (m_pCameraEdit != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �I������
		m_pCameraEdit->Uninit();

		// �|�C���^�� NULL �ɂ���
		m_pCameraEdit = nullptr;
	}

	if (m_pRangeMove != nullptr)
	{ // �|�C���^�� NULL ����Ȃ��ꍇ

		// �I������
		m_pRangeMove->Uninit();

		// �|�C���^�� NULL �ɂ���
		m_pRangeMove = nullptr;
	}
}

//==========================
// �ݒ菈��
//==========================
void CEditAll::Set(const D3DXVECTOR3& pos)
{
	switch (m_style)
	{
	case STYLE_BLOCK:			// �u���b�N(�X�e�[�W)

		if (m_pEdit == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ��������
			m_pEdit = m_pEdit->Create
			(
				D3DXVECTOR3(pos.x, pos.y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				NONE_SCALE
			);
		}

		break;

	case STYLE_BGOBJECT:		// �w�i�I�u�W�F�N�g

		if (m_pBgEdit == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ��������
			m_pBgEdit = m_pBgEdit->Create
			(
				pos,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				NONE_SCALE,
				CXFile::GetXFile(CXFile::TYPE_ENERGYITEM)
			);
		}

		break;

	case STYLE_OBSTACLE:

		if (m_pObstacleEdit == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ��������
			m_pObstacleEdit = m_pObstacleEdit->Create(D3DXVECTOR3(pos.x, pos.y, 0.0f));
		}

		break;

	case STYLE_CAMERA:

		if (m_pCameraEdit == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// ��������
			m_pCameraEdit = m_pCameraEdit->Create
			(
				pos,
				CAMERAEDIT_SIZE,
				CCamera::TYPE_UPWARD
			);
		}

		break;
	}
}

//==========================
// �ʒu�̎擾����
//==========================
D3DXVECTOR3 CEditAll::GetEditPos(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ۑ��p�̈ʒu�̕ϐ�

	switch (m_style)
	{
	case STYLE_BLOCK:			// �u���b�N(�X�e�[�W)

		if (m_pEdit != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ʒu��ۑ����Ă���
			posSave = m_pEdit->GetPos();
		}

		break;

	case STYLE_BGOBJECT:		// �w�i�I�u�W�F�N�g

		if (m_pBgEdit != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ʒu��ۑ����Ă���
			posSave = m_pBgEdit->GetPos();
		}

		break;

	case STYLE_OBSTACLE:		// ��Q��

		if (m_pObstacleEdit != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ʒu��ۑ����Ă���
			posSave = m_pObstacleEdit->GetPos();
		}

		break;

	case STYLE_CAMERA:			// �J�����͈̔�

		if (m_pCameraEdit != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ʒu��ۑ����Ă���
			posSave = m_pCameraEdit->GetPos();
		}

		break;

	case STYLE_RANGEMOVE:		// �͈͈ړ�

		if (m_pRangeMove != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ʒu��ۑ����Ă���
			posSave = m_pRangeMove->GetPos();
		}

		break;
	}

	// �ʒu��ݒ肷��
	return posSave;
}

//==========================
// �X�^�C���̎擾����
//==========================
CEditAll::STYLE CEditAll::GetStyle(void) const
{
	// �X�^�C����Ԃ�
	return m_style;
}

//==========================
// ��������
//==========================
CEditAll* CEditAll::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEditAll* pEditAll = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEditAll == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEditAll = new CEditAll;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEditAll != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

	  // ����������
		if (FAILED(pEditAll->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�S�G�f�B�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

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
	return pEditAll;
}

#endif