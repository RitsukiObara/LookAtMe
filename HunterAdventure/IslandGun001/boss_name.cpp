//============================================
//
// �{�X�̖��O�\������[boss_name.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "boss_name.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 INIT_POS = D3DXVECTOR3(-400.0f, SCREEN_HEIGHT * 0.5f, 0.0f);			// �����ʒu
	const D3DXVECTOR3 NAME_SIZE = D3DXVECTOR3(100.0f, 300.0f, 0.0f);						// �T�C�Y
	const char* TEXTURE = "data\\TEXTURE\\BossName.png";		// �e�N�X�`��
	const D3DXVECTOR3 APPEAR_DEST_POS = D3DXVECTOR3(1100.0f, SCREEN_HEIGHT * 0.5f, 0.0f);	// �o����Ԃ̖ړI�̈ʒu
	const float APPEAR_MOVE = 100.0f;		// �o����Ԃ̈ړ���
	const int STOP_COUNT = 5;				// ��~��Ԃ̃J�E���g��
	const D3DXVECTOR3 SHADOW_DEST_POS = D3DXVECTOR3(1095.0f, SCREEN_HEIGHT * 0.5f + 5.0f, 0.0f);		// �e�̖ړI�̈ʒu
	const D3DXVECTOR3 SHADOW_MOVE = D3DXVECTOR3(0.1f, 0.1f, 0.0f);			// �e�̈ړ���
	const int SHADOW_COUNT = 70;			// �e��Ԃ̃J�E���g
	const float LEAVE_DEST_POS = 1500.0f;	// �ޏ��Ԃ̖ړI�̈ʒu
}

//========================
// �R���X�g���N�^
//========================
CBossName::CBossName() : CObject(TYPE_BOSSNAME, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject[nCnt] = nullptr;		// ���O�\���̏��
	}
	m_state = STATE_APPEAR;				// ���
	m_nStateCount = 0;					// ��ԃJ�E���g
}

//========================
// �f�X�g���N�^
//========================
CBossName::~CBossName()
{

}

//========================
// ����������
//========================
HRESULT CBossName::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CBossName::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // �c�e�̏�� NULL ����Ȃ��ꍇ

			// �c�e�̏I������
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	// �����̏I������
	Release();
}

//========================
// �X�V����
//========================
void CBossName::Update(void)
{
	switch (m_state)
	{
	case CBossName::STATE_APPEAR:

		if (Appear(m_apObject[TYPE::TYPE_NAMESHADOW]) == true ||
			Appear(m_apObject[TYPE::TYPE_NAME]) == true)
		{ // �ړI�̈ʒu�ɒ������ꍇ

			// �o����ԑJ�ڏ���
			AppearArrival();
		}

		break;

	case CBossName::STATE_STOP:

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % STOP_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɒB�����ꍇ

			// ��ԃJ�E���g�����Z�b�g����
			m_nStateCount = 0;

			// �e��Ԃɂ���
			m_state = STATE_SHADOW;
		}

		break;

	case CBossName::STATE_SHADOW:

		// �e��ԏ���
		Shadow();

		break;

	case CBossName::STATE_LEAVE:

		if (Leave(m_apObject[TYPE::TYPE_NAMESHADOW]) == true ||
			Leave(m_apObject[TYPE::TYPE_NAME]) == true)
		{ // �ړI�̈ʒu�ɒ������ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // ���O�\���� NULL ����Ȃ��ꍇ

			// ���_���W�̐ݒ菈��
			m_apObject[nCnt]->SetVertex();
		}
	}
}

//========================
// �`�揈��
//========================
void CBossName::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // �c�e���`��ł����Ԃ̏ꍇ

			// �`�揈��
			m_apObject[nCnt]->Draw();
		}
	}
}

//========================
// ���̐ݒ菈��
//========================
void CBossName::SetData(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] == nullptr)
		{ // ���O�\���� NULL ����Ȃ��ꍇ

			// ���O�\���𐶐�
			m_apObject[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

			// �ݒ菈��
			m_apObject[nCnt]->SetPos(INIT_POS);			// �ʒu
			m_apObject[nCnt]->SetPosOld(INIT_POS);		// �O��̈ʒu
			m_apObject[nCnt]->SetRot(NONE_D3DXVECTOR3);	// ����
			m_apObject[nCnt]->SetSize(NAME_SIZE);		// �T�C�Y
			m_apObject[nCnt]->SetAngle();				// ����
			m_apObject[nCnt]->SetLength();				// ����

			// �e�N�X�`���̊��蓖�ď���
			m_apObject[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

			// ���_���W�̐ݒ菈��
			m_apObject[nCnt]->SetVertex();
		}
	}
	// ���_�J���[�̐ݒ菈��
	m_apObject[TYPE_NAMESHADOW]->SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	m_state = STATE_APPEAR;				// ���
	m_nStateCount = 0;					// ��ԃJ�E���g
}

//========================
// ��������
//========================
CBossName* CBossName::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBossName* pName = nullptr;			// ���O�̃C���X�^���X�𐶐�

	if (pName == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pName = new CBossName;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pName != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pName->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pName->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���O�̃|�C���^��Ԃ�
	return pName;
}

//========================
// �o����ԏ���
//========================
bool CBossName::Appear(CObject2D* pObject)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = pObject->GetPos();
	bool bArrival = false;		// ������

	// �ړ�����
	bArrival = useful::FrameCorrect(APPEAR_DEST_POS.x, &pos.x, APPEAR_MOVE);

	// �ʒu��K�p
	pObject->SetPos(pos);

	// �����󋵂�Ԃ�
	return bArrival;
}

//========================
// �o����ԑJ�ڏ���
//========================
void CBossName::AppearArrival(void)
{
	D3DXVECTOR3 pos = NONE_D3DXVECTOR3;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �ʒu���擾
		pos = m_apObject[nCnt]->GetPos();

		// �ʒu��ݒ肷��
		pos = APPEAR_DEST_POS;

		// �ʒu��K�p
		m_apObject[nCnt]->SetPos(pos);
	}

	// ��~��Ԃɂ���
	m_state = STATE_STOP;
}

//========================
// �e��ԏ���
//========================
void CBossName::Shadow(void)
{
	// ��ԃJ�E���g�����Zsjr��������
	m_nStateCount++;

	// �ʒu���擾
	D3DXVECTOR3 pos = m_apObject[TYPE_NAMESHADOW]->GetPos();

	// �ړ�������
	useful::FrameCorrect(SHADOW_DEST_POS.x, &pos.x, SHADOW_MOVE.x);
	useful::FrameCorrect(SHADOW_DEST_POS.y, &pos.y, SHADOW_MOVE.y);

	if(m_nStateCount % SHADOW_COUNT == 0)
	{ // ���J�E���g�o�߂����ꍇ

		// �ʒu��␳
		pos = SHADOW_DEST_POS;

		// �ޏ��Ԃɂ���
		m_state = STATE_LEAVE;
	}

	// �ʒu��K�p
	m_apObject[TYPE_NAMESHADOW]->SetPos(pos);
}

//========================
// �ޏ��ԏ���
//========================
bool CBossName::Leave(CObject2D* pObject)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = pObject->GetPos();
	bool bEnd = false;		// ������

	// �ړ�����
	bEnd = useful::FrameCorrect(LEAVE_DEST_POS, &pos.x, APPEAR_MOVE);

	// �ʒu��K�p
	pObject->SetPos(pos);

	//�I���󋵂�Ԃ�
	return bEnd;
}