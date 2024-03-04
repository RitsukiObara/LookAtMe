//============================================
//
// �Ŕ̐�������[signboard_explain.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "signboard_explain.h"
#include "object2D.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include "useful.h"

#include "tutorial.h"
#include "signboard.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 SCREEN_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// �w�i�̈ʒu
	const D3DXVECTOR3 SCREEN_SIZE = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// �w�i�̃T�C�Y
	const D3DXVECTOR3 EXPLAIN_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -200.0f, 0.0f);					// �����̈ʒu
	const D3DXVECTOR3 EXPLAIN_SIZE = D3DXVECTOR3(500.0f, 250.0f, 0.0f);									// �����̃T�C�Y
	const char* EXPLAIN_TEXTURE[CSignboard::TYPE_MAX] = 	// �����̃e�N�X�`��
	{
		"data\\TEXTURE\\Signboard001.png",
		"data\\TEXTURE\\Signboard002.png",
		"data\\TEXTURE\\Signboard003.png",
		"data\\TEXTURE\\Signboard004.png",
		"data\\TEXTURE\\Signboard005.png",
		"data\\TEXTURE\\Signboard006.png",
		"data\\TEXTURE\\Signboard007.png",
		"data\\TEXTURE\\Signboard008.png",
		"data\\TEXTURE\\Signboard009.png",
	};
	const D3DXCOLOR SCREEN_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// �w�i�̐F
	const float SCREEN_ADD_ALPHA = 0.01f;			// �w�i�̓����x�̉��Z��
	const float SCREEN_MAX_ALPHA = 0.6f;			// �w�i�̓����x�̍ő吔

	const float APPEAR_ADD_MOVE = 1.2f;				// �o����Ԃ̈ړ��ʂ̒ǉ���
	const float LEAVE_ADD_MOVE = 1.2f;				// �ޏ��Ԃ̈ړ��ʂ̒ǉ���
	const float LEAVE_DEST_EXPLAIN_POSY = -300.0f;	// �ޏ��Ԃ̐������̖ړI�̍���
	const float SCREEN_LEAVE_ALPHA = 0.0f;			// �ޏ��Ԃ̔w�i�̓����x
}

//============================
// �R���X�g���N�^
//============================
CSignboardExpl::CSignboardExpl() : CObject(CObject::TYPE_SIGNEXPLAIN, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_screen.pScreen = nullptr;			// ��ʂ̏��
	m_screen.fAlpha = 0.0f;				// �����x
	m_explain.pExplain = nullptr;		// �����̃|���S��
	m_explain.fMoveY = 0.0f;			// Y���̈ړ���
	m_state = STATE_APPEAR;				// ���
}

//============================
// �f�X�g���N�^
//============================
CSignboardExpl::~CSignboardExpl()
{

}

//============================
// ����������
//============================
HRESULT CSignboardExpl::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CSignboardExpl::Uninit(void)
{
	if (m_screen.pScreen != nullptr)
	{ // �w�i�� NULL ����Ȃ��ꍇ

		// �w�i�̏I������
		m_screen.pScreen->Uninit();
		m_screen.pScreen = nullptr;
	}

	if (m_explain.pExplain != nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// �����̏I������
		m_explain.pExplain->Uninit();
		m_explain.pExplain = nullptr;
	}

	// �j������
	Release();
}

//============================
// �X�V����
//============================
void CSignboardExpl::Update(void)
{
	switch (m_state)
	{
	case CSignboardExpl::STATE_APPEAR:		// �o�����

		// �o����ԏ���
		Appear();

		break;

	case CSignboardExpl::STATE_STOP:		// ��~���

		if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
			CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
		{ // A�L�[���������ꍇ

			// �ޏ��Ԃɂ���
			m_state = STATE_LEAVE;

			// �Ŕ���炷
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNBOARD);
		}

		break;

	case CSignboardExpl::STATE_LEAVE:		// �ޏ���

		if (Leave() == true)
		{ // �ޏꂵ�������ꍇ

			// �ʏ��Ԃɂ���
			CTutorial::SetState(CTutorial::STATE_NONE);

			// �Ŕ̐����� NULL �ɂ���
			CTutorial::GetLookSign()->SetExplain(nullptr);

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

	// ���_���W�̐ݒ菈��
	m_explain.pExplain->SetVertex();

	// ���_�J���[�̐ݒ菈��
	m_screen.pScreen->SetVtxColor(D3DXCOLOR(SCREEN_COL.r, SCREEN_COL.g, SCREEN_COL.b, m_screen.fAlpha));
}

//============================
// �`�揈��
//============================
void CSignboardExpl::Draw(void)
{
	if (m_screen.pScreen != nullptr)
	{ // �w�i�� NULL ����Ȃ��ꍇ

		// �w�i�̕`�揈��
		m_screen.pScreen->Draw();
	}

	if (m_explain.pExplain != nullptr)
	{ // ������ NULL ����Ȃ��ꍇ

		// �����̕`�揈��
		m_explain.pExplain->Draw();
	}
}

//============================
// ���̐ݒ菈��
//============================
void CSignboardExpl::SetData(const CSignboard::TYPE type)
{
	if (m_screen.pScreen == nullptr)
	{ // �w�i�� NULL �̏ꍇ

		// �w�i�𐶐�
		m_screen.pScreen = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

		// ���̐ݒ菈��
		m_screen.pScreen->SetPos(SCREEN_POS);			// �ʒu
		m_screen.pScreen->SetPosOld(SCREEN_POS);		// �O��̈ʒu
		m_screen.pScreen->SetRot(NONE_D3DXVECTOR3);		// ����
		m_screen.pScreen->SetSize(SCREEN_SIZE);			// �T�C�Y
		m_screen.pScreen->SetAngle();					// ����
		m_screen.pScreen->SetLength();					// ����

		// ���_���W�̐ݒ菈��
		m_screen.pScreen->SetVertex();

		// ���_�J���[�̐ݒ菈��
		m_screen.pScreen->SetVtxColor(D3DXCOLOR(SCREEN_COL.r, SCREEN_COL.g, SCREEN_COL.b, m_screen.fAlpha));
	}
	m_screen.fAlpha = 0.0f;

	if (m_explain.pExplain == nullptr)
	{ // ������ NULL �̏ꍇ

		// �����𐶐�
		m_explain.pExplain = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

		// ���̐ݒ菈��
		m_explain.pExplain->SetPos(EXPLAIN_POS);		// �ʒu
		m_explain.pExplain->SetPosOld(EXPLAIN_POS);		// �O��̈ʒu
		m_explain.pExplain->SetRot(NONE_D3DXVECTOR3);	// ����
		m_explain.pExplain->SetSize(EXPLAIN_SIZE);		// �T�C�Y
		m_explain.pExplain->SetAngle();					// ����
		m_explain.pExplain->SetLength();				// ����

		// �e�N�X�`���̐ݒ菈��
		m_explain.pExplain->BindTexture(CManager::Get()->GetTexture()->Regist(EXPLAIN_TEXTURE[type]));

		// ���_���W�̐ݒ菈��
		m_explain.pExplain->SetVertex();
	}
	m_explain.fMoveY = 0.0f;

	m_state = STATE_APPEAR;			// ���
}

//============================
//��������
//============================
CSignboardExpl* CSignboardExpl::Create(const CSignboard::TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSignboardExpl* pExplain = nullptr;			// �����̃C���X�^���X�𐶐�

	if (pExplain == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pExplain = new CSignboardExpl;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pExplain != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pExplain->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pExplain->SetData(type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �����̃|�C���^��Ԃ�
	return pExplain;
}

//============================
// �o����ԏ���
//============================
void CSignboardExpl::Appear(void)
{
	// �w�i�����ɋ߂Â���
	useful::FrameCorrect(SCREEN_MAX_ALPHA, &m_screen.fAlpha, SCREEN_ADD_ALPHA);

	// �ړ��ʂ����Z����
	m_explain.fMoveY += APPEAR_ADD_MOVE;

	// �ʒu���擾����
	D3DXVECTOR3 pos = m_explain.pExplain->GetPos();

	// �ړ�����
	pos.y += m_explain.fMoveY;

	if (pos.y >= SCREEN_HEIGHT * 0.5f)
	{ // ���S�𒴂����ꍇ

		// �ʒu��␳����
		pos.y = SCREEN_HEIGHT * 0.5f;

		// ��~��Ԃɂ���
		m_state = STATE_STOP;

		// �ړ��ʂ����Z�b�g����
		m_explain.fMoveY = 0.0f;
	}

	// �ʒu��K�p
	m_explain.pExplain->SetPos(pos);
}

//============================
// �ޏ��ԏ���
//============================
bool CSignboardExpl::Leave(void)
{
	// �I����
	bool bEnd = false;

	// �w�i�����ɋ߂Â���
	useful::FrameCorrect(SCREEN_LEAVE_ALPHA, &m_screen.fAlpha, SCREEN_ADD_ALPHA);

	// �ړ��ʂ����Z����
	m_explain.fMoveY -= LEAVE_ADD_MOVE;

	// �ʒu���擾����
	D3DXVECTOR3 pos = m_explain.pExplain->GetPos();

	// �ړ�����
	pos.y += m_explain.fMoveY;

	if (pos.y <= LEAVE_DEST_EXPLAIN_POSY)
	{ // ���S�𒴂����ꍇ

		// �I������
		bEnd = true;
	}

	// �ʒu��K�p
	m_explain.pExplain->SetPos(pos);

	// �I���󋵂�Ԃ�
	return bEnd;
}