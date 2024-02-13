//============================================
//
// �����o������[balloon.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "balloon.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "balloonUI.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define TEXTURE				"data/TEXTURE/BalloonKari.png"		// �e�N�X�`��(��)
#define SMALL_SIZE			(D3DXVECTOR3(10.0f, 10.0f, 0.0f))	// ���k��Ԃł̃T�C�Y
#define SIZE_CORRECT		(0.3f)								// �T�C�Y�̕␳�{��
#define DISP_COUNT			(20)								// �\����ԂɂȂ�܂ł̃J�E���g��
#define BOUND_SIZE			(3.0f)								// �o�E���h�̃T�C�Y��
#define BOUND_MOVE			(0.1f)								// �o�E���h�̈ړ���
#define REDUCE_CHANGE_SIZE	(0.001f)							// �k�����̕ω��󋵂̕ς��T�C�Y

//============================
// �R���X�g���N�^
//============================
CBalloon::CBalloon() : CBillboard(CObject::TYPE_BALLOON, PRIORITY_SHADOW)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_SMALL;			// ���
	m_type = TYPE::TYPE_NORMAL;		// ���
	m_sizeDisp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �`�掞�̃T�C�Y
	m_sizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̈ړ���
	m_bChange = false;				// �ω���
}

//============================
// �f�X�g���N�^
//============================
CBalloon::~CBalloon()
{

}

//============================
// ����������
//============================
HRESULT CBalloon::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_state = STATE_SMALL;			// ���
	m_type = TYPE::TYPE_NORMAL;		// ���
	m_sizeDisp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �`�掞�̃T�C�Y
	m_sizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̈ړ���
	m_bChange = false;				// �ω���

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CBalloon::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//============================
// �X�V����
//============================
void CBalloon::Update(void)
{
	switch (m_state)
	{
	case STATE_SMALL:		// ���k���

		// �k������
		Small();

		break;

	case STATE_DISP:		// �\�����

		// �\������
		Disp();

		break;
	}

	// ���_���W�̐ݒ菈��
	SetVtxPosUnder();
}

//============================
// �`�揈��
//============================
void CBalloon::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff();
}

//============================
// ��Ԃ̐ݒ菈��
//============================
void CBalloon::SetState(const STATE state)
{
	if (m_state == state)
	{ // ��Ԃ���肵�Ă���ꍇ

		// ���̐�̏������s��Ȃ�
		return;
	}
	// ��Ԃ�ݒ肷��
	m_state = state;

	if (m_state == STATE_DISP)
	{ // �\����ԂɂȂ����Ƃ�

		// �T�C�Y�̈ړ��ʂ�ݒ肷��
		m_sizeMove = m_sizeDisp / DISP_COUNT;
	}

	// �ω��󋵂�ݒ肷��
	m_bChange = true;
}

//============================
// ��ނ̎擾����
//============================
CBalloon::TYPE CBalloon::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================
// �\����Ԃ̏���
//============================
void CBalloon::Disp(void)
{
	if (m_bChange == true)
	{ // �ω���Ԃ̏ꍇ
		
		// �g�又��
		Expansion();
	}
	else
	{ // ��L�ȊO

		// �o�E���h����
		Bound();
	}
}

//============================
// ���k��Ԃ̏���
//============================
void CBalloon::Small(void)
{
	if (m_bChange == true)
	{ // �ω���Ԃ̏ꍇ

		// �k�����
		Reduce();
	}
	else
	{ // ��L�ȊO

	}
}

//============================
// �g�又��
//============================
void CBalloon::Expansion(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();		// �T�C�Y���擾����

	if (size.x < m_sizeDisp.x)
	{ // �T�C�Y���܂��\�����̃T�C�Y�����������ꍇ

		// �T�C�Y�����Z����
		size.x += m_sizeMove.x;

		if (size.x >= m_sizeDisp.x)
		{ // �T�C�Y���`�掞�̃T�C�Y�ȏ�ɂȂ����ꍇ

			// �T�C�Y��␳����
			size.x = m_sizeDisp.x;

			// �ω��󋵂�ݒ肷��
			m_bChange = false;
		}
	}

	if (size.y < m_sizeDisp.y)
	{ // �T�C�Y���܂��\�����̃T�C�Y�����������ꍇ

		// �T�C�Y�����Z����
		size.y += m_sizeMove.y;

		if (size.y >= m_sizeDisp.y)
		{ // �T�C�Y���`�掞�̃T�C�Y�ȏ�ɂȂ����ꍇ

			// �T�C�Y��␳����
			size.y = m_sizeDisp.y;

			// �ω��󋵂�ݒ肷��
			m_bChange = false;
		}
	}

	// �T�C�Y��ݒ肷��
	SetSize(size);
}

//============================
// �k������
//============================
void CBalloon::Reduce(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();		// �T�C�Y���擾����

	// �T�C�Y�̕␳����
	Correct(SMALL_SIZE.x, &size.x, SIZE_CORRECT);
	Correct(SMALL_SIZE.y, &size.y, SIZE_CORRECT);

	if (size.x <= SMALL_SIZE.x + REDUCE_CHANGE_SIZE &&
		size.y <= SMALL_SIZE.y + REDUCE_CHANGE_SIZE)
	{ // �T�C�Y�����ȉ��ɂȂ����ꍇ

		// �T�C�Y��ݒ肷��
		size = SMALL_SIZE;

		// �ω��󋵂�ݒ肷��
		m_bChange = false;
	}

	// �T�C�Y��ݒ肷��
	SetSize(size);
}

//============================
// �o�E���h����
//============================
void CBalloon::Bound(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();		// �T�C�Y���擾����

	// �T�C�Y�����Z����
	size.y += m_sizeMove.y;

	if (size.y < m_sizeDisp.y - BOUND_SIZE)
	{ // �T�C�Y���܂��\�����̃T�C�Y�����������ꍇ

		// �T�C�Y��ݒ肷��
		size.y = m_sizeDisp.y - BOUND_SIZE;

		// �T�C�Y�̈ړ��ʂ�ݒ肷��
		m_sizeMove.y = BOUND_MOVE;
	}
	else if (size.y > m_sizeDisp.y + BOUND_SIZE)
	{ // �T�C�Y���܂��\�����̃T�C�Y���߂������ꍇ

		// �T�C�Y��ݒ肷��
		size.y = m_sizeDisp.y + BOUND_SIZE;

		// �T�C�Y�̈ړ��ʂ�ݒ肷��
		m_sizeMove.y = -BOUND_MOVE;
	}

	// �T�C�Y��ݒ肷��
	SetSize(size);
}

//============================
// ���̐ݒ菈��
//============================
void CBalloon::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ���[�J���ϐ��錾
	CFile::SBalloonData data;		// �����o���̏��

	if (CManager::GetFile() != nullptr)
	{ // �t�@�C���̏�� NULL ����Ȃ��ꍇ

		// �����o���̏����擾����
		data = CManager::GetFile()->GetBalloon(type);
	}

	// �S�Ă̒l������������
	m_state = STATE_SMALL;		// ���
	m_type = type;				// ���
	m_sizeDisp = data.size;		// �`�掞�̃T�C�Y
	m_sizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̈ړ���
	m_bChange = false;			// �ω���

	// ���̏�����
	SetPos(pos);
	SetSize(SMALL_SIZE);

	// ���_���̏�����
	SetVtxPosUnder();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(data.aTextureName));
}

//============================
//��������
//============================
CBalloon* CBalloon::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBalloon* pBalloon = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pBalloon == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBalloon = new CBalloon;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBalloon != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBalloon->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�����o���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBalloon->SetData(pos, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �^�C�g���̑I�����̃|�C���^��Ԃ�
	return pBalloon;
}