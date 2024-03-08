//=======================================
//
// �Ւd���b�Z�[�W�̃��C������[alter_message.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "alter_message.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// �������O���
//=======================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\AlterMeta.png";		// �e�N�X�`��
	const int EXTEND_COUNT = 50;			// �L�я�Ԃ̃J�E���g��
	const int SHRINK_COUNT = 15;			// �k�ݏ�Ԃ̃J�E���g��
	const D3DXVECTOR3 SHIFT = D3DXVECTOR3(0.0f, 900.0f, 0.0f);			// �|���S���̂��炷��
	const D3DXVECTOR3 POLYGON_SIZE = D3DXVECTOR3(400.0f, 100.0f, 0.0f);	// �|���S���̃T�C�Y
	const D3DXVECTOR3 EXTEND_MAGNI = D3DXVECTOR3(0.8f, 1.2f, 0.0f);		// �L�я�Ԃ̔{��
	const D3DXVECTOR3 SHRINK_MAGNI = D3DXVECTOR3(1.2f, 0.8f, 0.0f);		// �k�ݏ�Ԃ̔{��
}

//=========================
// �R���X�g���N�^
//=========================
CAlterMessage::CAlterMessage() : CBillboard(CObject::TYPE_EFFECT, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_sizeDest = NONE_D3DXVECTOR3;	// �ړI�̃T�C�Y
	m_state = STATE_EXTEND;			// ���
	m_nStateCount = 0;				// ��ԃJ�E���g
}

//=========================
// �f�X�g���N�^
//=========================
CAlterMessage::~CAlterMessage()
{

}

//=========================
// ����������
//=========================
HRESULT CAlterMessage::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CAlterMessage::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CAlterMessage::Update(void)
{
	D3DXVECTOR3 size = GetSize();

	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	switch (m_state)
	{
	case CAlterMessage::STATE_EXTEND:

		// �T�C�Y��ݒ肷��
		useful::FrameCorrect(m_sizeDest.x, &size.x, (m_sizeDest.x - POLYGON_SIZE.x) / EXTEND_COUNT);
		useful::FrameCorrect(m_sizeDest.y, &size.y, (m_sizeDest.y - POLYGON_SIZE.y) / EXTEND_COUNT);

		if (m_nStateCount % EXTEND_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ��ԃJ�E���g������������
			m_nStateCount = 0;

			// �k����Ԃɂ���
			m_state = STATE_SHRINK;

			// �ړI�̃T�C�Y��ݒ肷��
			m_sizeDest = D3DXVECTOR3(POLYGON_SIZE.x * SHRINK_MAGNI.x, POLYGON_SIZE.y * SHRINK_MAGNI.y, 0.0f);
		}

		break;

	case CAlterMessage::STATE_SHRINK:

		// �T�C�Y��ݒ肷��
		useful::FrameCorrect(m_sizeDest.x, &size.x, (m_sizeDest.x - POLYGON_SIZE.x) / SHRINK_COUNT);
		useful::FrameCorrect(m_sizeDest.y, &size.y, (m_sizeDest.y - POLYGON_SIZE.y) / SHRINK_COUNT);

		if (m_nStateCount % SHRINK_COUNT == 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ��ԃJ�E���g������������
			m_nStateCount = 0;

			// �g���Ԃɂ���
			m_state = STATE_EXTEND;

			// �ړI�̃T�C�Y��ݒ肷��
			m_sizeDest = D3DXVECTOR3(POLYGON_SIZE.x * EXTEND_MAGNI.x, POLYGON_SIZE.y * EXTEND_MAGNI.y, 0.0f);
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �T�C�Y�̐ݒ菈��
	SetSize(size);

	// ���_���W�̐ݒ菈��
	SetVertex();
}

//=========================
// �`�揈��
//=========================
void CAlterMessage::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff(false);
}

//=========================
// ���̐ݒ菈��
//=========================
void CAlterMessage::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos + SHIFT);		// �ʒu
	SetPosOld(pos + SHIFT);		// �O��̈ʒu
	SetSize(POLYGON_SIZE);		// �T�C�Y
	SetEnableLookY(true);		// Y����

	// �S�Ă̒l������������
	m_sizeDest = D3DXVECTOR3(POLYGON_SIZE.x * EXTEND_MAGNI.x, POLYGON_SIZE.y * EXTEND_MAGNI.y, 0.0f);	// �ړI�̃T�C�Y
	m_state = STATE_EXTEND;		// ���
	m_nStateCount = 0;			// ��Ԃ̃J�E���g

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//=========================
// ��������
//=========================
CAlterMessage* CAlterMessage::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAlterMessage* pMessage = nullptr;	// �Ւd�̃C���X�^���X�𐶐�

	if (pMessage == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pMessage = new CAlterMessage;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pMessage != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pMessage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pMessage->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �Ւd�̃��b�Z�[�W�̃|�C���^��Ԃ�
	return pMessage;
}