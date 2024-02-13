//=======================================
//
// �A�C�e��������̃��C������[item_generator.cpp]
// Author ��������
//
//=======================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "item_generator.h"
#include "sound.h"
#include "useful.h"

#include "Item.h"

// �}�N����`
#define SET_STATE_COUNT			(90)			// �ݒ��Ԃ̃J�E���g��
#define VIBRATE_STATE_COUNT		(30)			// �U����Ԃ̃J�E���g��
#define SWING_COUNT				(5)				// �U������J�E���g��
#define POS_Z					(80.0f)			// �ݒu����ʒu
#define ROT_CORRECT_MAGNI		(0.5f)			// �����̕␳�{��
#define ROT_CHANGE_MAGNI		(-0.6f)			// �����̕ω����̔{��

//==========================================
// �R���X�g���N�^
//==========================================
CItemGenerator::CItemGenerator() : CModel(CObject::TYPE_ITEMGENERATOR, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pItemAddress = nullptr;	// �A�C�e���̃A�h���X
	m_state = STATE_SET;		// ���
	m_nStateCount = 0;			// �����J�E���g
	m_fRotDest = 0.0f;			// �ڕW�̌���
}

//==========================================
// �f�X�g���N�^
//==========================================
CItemGenerator::~CItemGenerator()
{

}

//==========================================
// �A�C�e���̏���������
//==========================================
HRESULT CItemGenerator::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_pItemAddress = nullptr;	// �A�C�e���̃A�h���X
	m_state = STATE_SET;		// ���
	m_nStateCount = 0;			// �����J�E���g
	m_fRotDest = 0.0f;			// �ڕW�̌���

	// ������Ԃ�
	return S_OK;
}

//========================================
// �A�C�e���̏I������
//========================================
void CItemGenerator::Uninit(void)
{
	// �A�C�e���̃A�h���X�� NULL �ɂ���
	m_pItemAddress = nullptr;

	// �I������
	CModel::Uninit();
}

//========================================
// �A�C�e���̍X�V����
//========================================
void CItemGenerator::Update(void)
{

	// �����[�X�ł̂�
#ifndef _DEBUG

	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu
	D3DXVECTOR3 rot = GetRot();		// ����

	switch (m_state)
	{
	case STATE_SET:		// �ݒ���

		// �A�C�e���̃A�h���X�� NULL �ɂ���
		m_pItemAddress = nullptr;

		// �����J�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % SET_STATE_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ��ԃJ�E���g������������
			m_nStateCount = 0;

			// ������ݒ肷��
			m_fRotDest = 0.3f;

			// �ʏ��Ԃɂ���
			m_state = STATE_VIBRATE;

			if (CManager::GetMode() == CScene::MODE_GAME &&
				CGame::GetState() == CGame::STATE_PLAY)
			{ // �v���C���̏ꍇ

				// ���̋@�̗h��鉹��炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_VENDING_SWING);
			}
		}

		break;

	case STATE_VIBRATE:	// �U�����

		// �A�C�e���̃A�h���X�� NULL �ɂ���
		m_pItemAddress = nullptr;

		// �����J�E���g�����Z����
		m_nStateCount++;

		// �����̕␳����
		RotCorrect(m_fRotDest, &rot.z, ROT_CORRECT_MAGNI);

		if (m_nStateCount % SWING_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ������ς���
			m_fRotDest *= ROT_CHANGE_MAGNI;
		}

		if (m_nStateCount % VIBRATE_STATE_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// �A�C�e���̐�������
			m_pItemAddress = CItem::Create(D3DXVECTOR3(GetPos().x, GetPos().y, 0.0f), CItem::STATE_APPEAR);

			if (CManager::GetMode() == CScene::MODE_GAME &&
				CGame::GetState() == CGame::STATE_PLAY)
			{ // �v���C���̏ꍇ

				// �A�C�e���̏o������炷
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM_APPEAR);
			}

			// ������ݒ肷��
			rot.z = 0.0f;

			// ��ԃJ�E���g������������
			m_nStateCount = 0;

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}

		break;

	case STATE_NONE:	// �ʏ���

		// ���ʏ���
		Discriminate();

		break;
	}

	// �������X�V����
	SetRot(rot);

#endif // _DEBUG
}

//===========================================
// �A�C�e���̕`�揈��
//===========================================
void CItemGenerator::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CItemGenerator::SetData(const D3DXVECTOR3& pos)
{
	// �S�Ă̒l������������
	m_pItemAddress = nullptr;	// �A�C�e���̃A�h���X
	m_state = STATE_SET;		// ���
	m_nStateCount = 0;			// �����J�E���g
	m_fRotDest = 0.0f;			// �ڕW�̌���

	// ���̐ݒ菈��
	SetPos(D3DXVECTOR3(pos.x, pos.y, POS_Z));		// �ʒu�ݒ�
	SetPosOld(D3DXVECTOR3(pos.x, pos.y, POS_Z));	// �O��̈ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// �����ݒ�
	SetScale(NONE_SCALE);							// �g�嗦�ݒ�
	SetFileData(CXFile::TYPE_ITEMGENERATOR);		// �f�[�^�̐ݒ菈��
}

//===========================================
// ���ʏ���
//===========================================
void CItemGenerator::Discriminate(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	STATE state = STATE_SET;		// ��Ԃ̕ϐ�

	// �I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_ENTITY);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�ւ̃|�C���^�� NULL ����Ȃ������

		// ���̃I�u�W�F�N�g��������
		pObjNext = pObj->GetNext();

		if (pObj->GetType() == TYPE_ITEM &&
			pObj == m_pItemAddress)
		{ // �I�u�W�F�N�g�̎�ނ��A�C�e�����A�A�h���X����v�����ꍇ
			
			// ��Ԃ�ݒ肷��
			state = STATE_NONE;

			// while���𔲂��o��
			break;
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObjNext;
	}

	// ��Ԃ�������
	m_state = state;
}

//===========================================
// ��������
//===========================================
CItemGenerator* CItemGenerator::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CItemGenerator* pItem = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pItem == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pItem = new CItemGenerator;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pItem != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pItem->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�A�C�e��������̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pItem->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pItem;
}