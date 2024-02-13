//============================================
//
// �J�E���g�_�E������[countdown.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "game.h"
#include "countdown.h"
#include "locus2D.h"
#include "game_time.h"
#include "renderer.h"
#include "sound.h"
#include "texture.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define COUNTDOWN_POS		(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))		// �J�E���g�_�E���̈ʒu
#define NUMBER_SIZE			(D3DXVECTOR3(100.0f, 200.0f, 0.0f))									// �����̃T�C�Y
#define START_SIZE			(D3DXVECTOR3(400.0f, 100.0f, 0.0f))									// �X�^�[�g�̃T�C�Y
#define NUMBER_TEX_PATTERN	(0.1f)																// �����̃e�N�X�`���̃p�^�[��
#define START_TIME			(3)																	// �X�^�[�g�܂ł̃J�E���g
#define COUNTDOWN_INTERVAL	(30)																// �J�E���g�_�E���̊Ԋu
#define LOCUS_ADD_SIZE		(7.0f)																// �c���̃T�C�Y�̉��Z��
#define ALPHA_SUB			(1.0f / COUNTDOWN_INTERVAL)											// �����x�̌��Z��

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CCountdown::m_apTexturename[CCountdown::TEX_MAX] = // �e�N�X�`���̃p�X��
{
	"data/TEXTURE/Number.png",	// �J�E���g�_�E���̐���
	"data/TEXTURE/Start.png",	// �X�^�[�g
};

//========================
// �R���X�g���N�^
//========================
CCountdown::CCountdown() : CObject2D(TYPE_COUNTDOWN, PRIORITY_UI)
{
	// �S�Ă̒l������������
	m_pLocus = nullptr;			// �c���̃|�C���^
	m_state = STATE_COUNT;		// ���
	m_nCountDown = 0;			// �X�^�[�g�܂ł̃^�C��
	m_nIntervalCount = 0;		// �Ԋu�̃J�E���g
	m_fAlpha = 0.0f;			// �����x
}

//========================
// �f�X�g���N�^
//========================
CCountdown::~CCountdown()
{

}

//========================
// ����������
//========================
HRESULT CCountdown::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

	  // ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_pLocus = nullptr;			// �c���̃|�C���^
	m_state = STATE_COUNT;		// ���
	m_nCountDown = 0;			// �X�^�[�g�܂ł̃^�C��
	m_nIntervalCount = 0;		// �Ԋu�̃J�E���g
	m_fAlpha = 1.0f;			// �����x

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CCountdown::Uninit(void)
{
	if (SearchLocus() == true)
	{ // �c���̃|�C���^�� NULL ����Ȃ��ꍇ

		// �c�����I������
		m_pLocus->Uninit();
		m_pLocus = nullptr;
	}

	// �I������
	CObject2D::Uninit();
}

//========================
// �X�V����
//========================
void CCountdown::Update(void)
{
	switch (m_state)
	{
	case STATE_COUNT:		// �J�E���g���

		// �J�E���g�_�E������
		Count();

		if (SearchLocus() == true)
		{ // �c���̏�񂪂������ꍇ

			// �c���̃T�C�Y�̐ݒ菈��
			LocusSize();
		}

		if (m_nCountDown <= 0)
		{ // �J�E���g�_�E����0�ȉ��ɂȂ����ꍇ

			// �e�N�X�`���̊��蓖�ď���
			BindTexture(CManager::GetTexture()->Regist(m_apTexturename[TEX_START]));

			// �T�C�Y��ݒ肷��
			SetSize(START_SIZE);

			if (SearchLocus() == true)
			{ // �c���{�������� true �̏ꍇ

				// �I������
				m_pLocus->Uninit();
			}

			// �c���̃|�C���^�� NULL �ɂ���
			m_pLocus = nullptr;

			// �c���̐�������
			m_pLocus = m_pLocus->Create(GetPos(), GetSize(), 1.0f, COUNTDOWN_INTERVAL, GetTexIdx());

			if (m_pLocus != nullptr)
			{ // �c���̃|�C���^�� NULL ����Ȃ��ꍇ

				// �c���̃e�N�X�`�����W�̐ݒ菈��
				m_pLocus->SetVtxTexture();
			}

			// �X�^�[�g��Ԃɐݒ肷��
			m_state = STATE_START;

			// �e�N�X�`�����W�̐ݒ菈��
			SetVtxTexture();

			// �X�^�[�g����炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_START);
		}
		else
		{ // ��L�ȊO

			// �e�N�X�`�����W�̐ݒ菈��
			SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);
		}

		break;

	case STATE_START:		// �X�^�[�g���

		// �Ԋu�J�E���g�����Z����
		m_nIntervalCount++;

		if (SearchLocus() == true)
		{ // �c���̏�񂪂������ꍇ

			// �c���̃T�C�Y�̐ݒ菈��
			LocusSize();
		}

		// �����x�����Z����
		m_fAlpha -= ALPHA_SUB;

		if (m_nIntervalCount >= COUNTDOWN_INTERVAL)
		{ // �Ԋu�J�E���g����萔�ȏ�ɂȂ����ꍇ

			if (SearchLocus() == true)
			{ // �c���̏�񂪂������ꍇ

				// �c���̏I������
				m_pLocus->Uninit();
				m_pLocus = nullptr;
			}

			// �I������
			Uninit();

			// �v���C���[�h�ɕς���
			CGame::SetState(CGame::STATE_PLAY);

			// ���Ԃ��X�^�[�g������
			CGame::GetTime()->TimeStart();

			// ���̐�̏������s��Ȃ�
			return;
		}

		// �F��ݒ肷��
		SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

		break;
	}

	// ���_���̐ݒ菈��
	SetVertex();
}

//========================
// �`�揈��
//========================
void CCountdown::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//========================
// ���̐ݒ菈��
//========================
void CCountdown::SetData()
{
	// �ݒ菈��
	SetPos(COUNTDOWN_POS);						// �ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetSize(NUMBER_SIZE);						// �T�C�Y�ݒ�
	SetLength();								// �����ݒ�
	SetAngle();									// �����ݒ�

	// �S�Ă̏���ݒ肷��
	m_pLocus = nullptr;			// �c���̃|�C���^
	m_nCountDown = START_TIME;	// �X�^�[�g�܂ł̃^�C��
	m_nIntervalCount = 0;		// �Ԋu�J�E���g

	// ���_���̐ݒ菈��
	SetVertex();

	// �e�N�X�`�����W�̐ݒ菈��
	SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(m_apTexturename[TEX_NUMBER]));

	// �c���̐�������
	m_pLocus = m_pLocus->Create(GetPos(), GetSize(), 1.0f, COUNTDOWN_INTERVAL, GetTexIdx());

	if (m_pLocus != nullptr)
	{ // �c���̃|�C���^�� NULL ����Ȃ��ꍇ

		// �c���̃e�N�X�`�����W�̐ݒ菈��
		m_pLocus->SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);
	}
}

//========================
// �J�E���g�_�E������
//========================
void CCountdown::Count(void)
{
	// �Ԋu�J�E���g�����Z����
	m_nIntervalCount++;

	if (m_nIntervalCount >= COUNTDOWN_INTERVAL)
	{ // �Ԋu�J�E���g����萔�ȏ�ɂȂ����ꍇ

		// �Ԋu�J�E���g������������
		m_nIntervalCount = 0;

		// �J�E���g�_�E�������Z����
		m_nCountDown--;

		if (m_nCountDown > 0)
		{ // �J�E���g�� 0���߂̏ꍇ

			if (SearchLocus() == true)
			{ // �c���{�������� true �̏ꍇ

				// �I������
				m_pLocus->Uninit();
			}

			// �c���̃|�C���^�� NULL �ɂ���
			m_pLocus = nullptr;

			// �c���̐�������
			m_pLocus = m_pLocus->Create(GetPos(), GetSize(), 1.0f, COUNTDOWN_INTERVAL, GetTexIdx());

			if (m_pLocus != nullptr)
			{ // �c���̃|�C���^�� NULL ����Ȃ��ꍇ

				// �c���̃e�N�X�`�����W�̐ݒ菈��
				m_pLocus->SetVtxTextureAnim(NUMBER_TEX_PATTERN, m_nCountDown);
			}

			// �J�E���g�_�E������炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
		}
	}
}

//========================
// �c���{������
//========================
bool CCountdown::SearchLocus(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�̃|�C���^

	// �擪�̃I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�̏�񂪂�������

		if (pObj->GetType() == TYPE_LOCUS &&
			pObj == m_pLocus)
		{ // ��ނ��c���������ꍇ

			// true ��Ԃ�
			return true;
		}

		// ���̃I�u�W�F�N�g�̏�������
		pObj = pObj->GetNext();
	}

	// ����ꏊ�� NULL �ɂ���
	m_pLocus = nullptr;

	// false ��Ԃ�
	return false;
}

//========================
// �c���̃T�C�Y�ݒ菈��
//========================
void CCountdown::LocusSize(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_pLocus->GetSize();		// �c���̃T�C�Y���擾����

	// �T�C�Y���g�傷��
	size.x += LOCUS_ADD_SIZE;
	size.y += LOCUS_ADD_SIZE;

	// �c���̃T�C�Y��ݒ肷��
	m_pLocus->SetSize(size);
}

//========================
// ��������
//========================
CCountdown* CCountdown::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCountdown* pCountDown = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pCountDown == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCountDown = new CCountdown;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCountDown != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCountDown->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���m�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pCountDown->SetData();

		// �J�E���g�_�E������炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pCountDown;
}