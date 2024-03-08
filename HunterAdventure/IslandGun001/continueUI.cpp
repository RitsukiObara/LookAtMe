//============================================
//
// �R���e�B�j���[UI����[continueUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "continueUI.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "sound.h"
#include "useful.h"

#include "object2D.h"
#include "game.h"
#include "player.h"
#include "camera.h"
#include "game_score.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const char* TEXTURE[CContinueUI::TYPE_MAX] =			// �e�N�X�`��
	{
		nullptr,
		"data\\TEXTURE\\ContGround.png",
		"data\\TEXTURE\\ContQuery.png",
		"data\\TEXTURE\\ContSubScore.png",
		"data\\TEXTURE\\ContYes.png",
		"data\\TEXTURE\\ContNo.png",
	};
	const D3DXVECTOR3 POS[CContinueUI::TYPE_MAX] =			// �ʒu
	{
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,200.0f,0.0f),
		D3DXVECTOR3(750.0f,330.0f,0.0f),
		D3DXVECTOR3(390.0f,480.0f,0.0f),
		D3DXVECTOR3(890.0f,480.0f,0.0f),
	};
	const D3DXVECTOR3 SELECT_SIZE = D3DXVECTOR3(200.0f, 100.0f, 0.0f);	// �I�𒆂̑I�����̃T�C�Y
	const D3DXVECTOR3 NONE_SIZE = D3DXVECTOR3(160.0f, 80.0f, 0.0f);		// �I�����Ă��Ȃ��I�����̃T�C�Y

	const D3DXVECTOR3 POLY_SIZE[CContinueUI::TYPE_MAX] =	// �T�C�Y
	{
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f),
		D3DXVECTOR3(550.0f,330.0f,0.0f),
		D3DXVECTOR3(500.0f,80.0f,0.0f),
		D3DXVECTOR3(80.0f,40.0f,0.0f),
		NONE_SIZE,
		NONE_SIZE,
	};
	const D3DXCOLOR SCREEN_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f);		// �w�i�̐F
	const int CONTINUE_LIFE = 100;			// �R���e�B�j���[�����Ƃ��̗̑�
	const int INIT_SUB_SCORE = 100;			// �X�R�A�̌��Z�ʂ̏����l
	const int MAX_SUB_SCORE = 9999;			// �X�R�A�̌��Z�ʂ̍ő吔

	const D3DXVECTOR3 NUMBER_POS = D3DXVECTOR3(460.0f, 330.0f, 0.0f);		// �����̈ʒu
	const D3DXVECTOR3 NUMBER_SIZE = D3DXVECTOR3(30.0f, 40.0f, 0.0f);		// �����̃T�C�Y
	const float NUMBER_SHIFT = 60.0f;		// �����̂��炷��
	const char* NUMBER_TEXTURE = "data\\TEXTURE\\Number.png";				// �����̃e�N�X�`��
}

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
int CContinueUI::m_nSubScore = INIT_SUB_SCORE;		// ���_�̌��Z��

//========================
// �R���X�g���N�^
//========================
CContinueUI::CContinueUI() : CObject(TYPE_CONTINUEUI, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject[nCnt] = nullptr;			// �|���S���̏��
	}
	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		m_apNumber[nCntCost] = nullptr;		// �ԍ�
	}
	m_select = SELECT_YES;			// �I����
}

//========================
// �f�X�g���N�^
//========================
CContinueUI::~CContinueUI()
{

}

//========================
// ����������
//========================
HRESULT CContinueUI::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CContinueUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // �|���S���̏�� NULL ����Ȃ��ꍇ

			// �|���S���̏I������
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		if (m_apNumber[nCntCost] != nullptr)
		{ // �����̏�� NULL ����Ȃ��ꍇ

			// �I������
			m_apNumber[nCntCost]->Uninit();
			m_apNumber[nCntCost] = nullptr;
		}
		m_apNumber[nCntCost] = nullptr;		// �ԍ�
	}

	// �����̏I������
	Release();
}

//========================
// �X�V����
//========================
void CContinueUI::Update(void)
{
	// �I������
	Select();

	if (Decide() == true)
	{ // ���肵���ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//========================
// �`�揈��
//========================
void CContinueUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] != nullptr)
		{ // �|���S���� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apObject[nCnt]->Draw();
		}
	}

	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		if (m_apNumber[nCntCost] != nullptr)
		{ // ������ NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apNumber[nCntCost]->Draw();
		}
	}
}

//========================
// ���̐ݒ菈��
//========================
void CContinueUI::SetData(void)
{
	int aNum[NUM_COSTDIGIT];

	// ���l�̌����Ƃ̎Z�o
	useful::DecimalCalculation(NUM_COSTDIGIT, m_nSubScore, &aNum[0]);

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject[nCnt] == nullptr)
		{ // �|���S���� NULL ����Ȃ��ꍇ

			// �|���S���𐶐�
			m_apObject[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

			// �ݒ菈��
			m_apObject[nCnt]->SetPos(POS[nCnt]);			// �ʒu
			m_apObject[nCnt]->SetPosOld(POS[nCnt]);			// �O��̈ʒu
			m_apObject[nCnt]->SetRot(NONE_D3DXVECTOR3);		// ����
			m_apObject[nCnt]->SetSize(POLY_SIZE[nCnt]);		// �T�C�Y
			m_apObject[nCnt]->SetAngle();					// ����
			m_apObject[nCnt]->SetLength();					// ����

			// �e�N�X�`���̊��蓖�ď���
			m_apObject[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]));

			// ���_���W�̐ݒ菈��
			m_apObject[nCnt]->SetVertexRot();

			if (nCnt == TYPE::TYPE_SCREEN)
			{ // �w�i�̏ꍇ

				// �F��ݒ肷��
				m_apObject[nCnt]->SetVtxColor(SCREEN_COL);
			}
		}
	}
	m_select = SELECT_YES;			// �I����

	for (int nCntCost = 0; nCntCost < NUM_COSTDIGIT; nCntCost++)
	{
		if (m_apNumber[nCntCost] == nullptr)
		{ // ������ NULL �̏ꍇ
			
			// ���l�𐶐�����
			m_apNumber[nCntCost] = CNumber::Create();

			// ���̐ݒ菈��
			m_apNumber[nCntCost]->SetPos(D3DXVECTOR3(NUMBER_POS.x + (nCntCost * NUMBER_SHIFT), NUMBER_POS.y, 0.0f));		// �ʒu�ݒ�
			m_apNumber[nCntCost]->SetRot(NONE_D3DXVECTOR3);		// �����ݒ�
			m_apNumber[nCntCost]->SetSize(NUMBER_SIZE);			// �T�C�Y�ݒ�
			m_apNumber[nCntCost]->SetLength();					// �����ݒ�
			m_apNumber[nCntCost]->SetAngle();					// �����ݒ�

			m_apNumber[nCntCost]->SetType(CNumber::TYPE_DECIMAL);	// �ԍ��̎��
			m_apNumber[nCntCost]->SetNumber(aNum[nCntCost]);		// �ԍ���ݒ肷��

			// ���_���̐ݒ菈��
			m_apNumber[nCntCost]->SetVertex();

			// �e�N�X�`���̊��蓖�ď���
			m_apNumber[nCntCost]->BindTexture(CManager::Get()->GetTexture()->Regist(NUMBER_TEXTURE));

			// �e�N�X�`���̐ݒ菈��(�A�j���[�V�����o�[�W����)
			m_apNumber[nCntCost]->SetVtxTextureAnim(NUMBER_TEXTURE_PATTERN, aNum[nCntCost]);
		}
	}
}

//========================
// ��������
//========================
CContinueUI* CContinueUI::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CContinueUI* pUI = nullptr;			// UI�̃C���X�^���X�𐶐�

	if (pUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pUI = new CContinueUI;
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
// ���_�̌��Z�ʂ̃��Z�b�g����
//========================
void CContinueUI::SubScoreReset(void)
{
	// ���_�̌��Z�ʂ����Z�b�g����
	m_nSubScore = INIT_SUB_SCORE;
}

//========================
// ���_�̌��Z�ʂ̎擾����
//========================
int CContinueUI::GetSubScore(void)
{
	// ���_�̌��Z�ʂ�Ԃ�
	return m_nSubScore;
}

//========================
// �I������
//========================
void CContinueUI::Select(void)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_LEFT, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_RIGHT, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_A) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_D) == true)
	{ // �E�������������ꍇ

		// �I������ς���
		m_select = (SELECT)((m_select + 1) % SELECT_MAX);
	}

	if (m_select == SELECT_YES)
	{ // YES��I�����Ă����ꍇ

		// �I�����̃T�C�Y��ݒ�
		m_apObject[TYPE_YES]->SetSize(SELECT_SIZE);
		m_apObject[TYPE_YES]->SetLength();
		m_apObject[TYPE_YES]->SetAngle();
		m_apObject[TYPE_YES]->SetVertexRot();
		m_apObject[TYPE_YES]->SetVtxColor(NONE_D3DXCOLOR);

		m_apObject[TYPE_NO]->SetSize(NONE_SIZE);
		m_apObject[TYPE_NO]->SetLength();
		m_apObject[TYPE_NO]->SetAngle();
		m_apObject[TYPE_NO]->SetVertexRot();
		m_apObject[TYPE_NO]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));
	}
	else
	{ // ��L�ȊO

		// �I�����̃T�C�Y��ݒ�
		m_apObject[TYPE_YES]->SetSize(NONE_SIZE);
		m_apObject[TYPE_YES]->SetLength();
		m_apObject[TYPE_YES]->SetAngle();
		m_apObject[TYPE_YES]->SetVertexRot();
		m_apObject[TYPE_YES]->SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f));

		m_apObject[TYPE_NO]->SetSize(SELECT_SIZE);
		m_apObject[TYPE_NO]->SetLength();
		m_apObject[TYPE_NO]->SetAngle();
		m_apObject[TYPE_NO]->SetVertexRot();
		m_apObject[TYPE_NO]->SetVtxColor(NONE_D3DXCOLOR);
	}
}

//========================
// ���菈��
//========================
bool CContinueUI::Decide(void)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
	{ // ����L�[���������ꍇ

		switch (m_select)
		{
		case CContinueUI::SELECT_YES:

			// �R���e�B�j���[����
			Continue();

			// �Q�[��BGM�𕜊�������
			CManager::Get()->GetSound()->Replay(CSound::SOUND_LABEL_BGM_GAME);

			// �Q�[��BGM�𓙑��ɂ���
			CManager::Get()->GetSound()->SetFrequency(CSound::SOUND_LABEL_BGM_GAME, 1.0f);

			break;


		case CContinueUI::SELECT_NO:

			// �I����Ԃɂ���
			CGame::SetState(CGame::STATE_FINISH);

			break;

		default:

			// ��~����
			assert(false);

			break;
		}

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

//========================
// �R���e�B�j���[����
//========================
void CContinueUI::Continue(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();		// �v���C���[�̏��
	CGameScore* pScore = CGame::GetGameScore();	// �Q�[���X�R�A�̏��

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �̗͂�ݒ肷��
		pPlayer->SetLife(CONTINUE_LIFE);

		// �v���C���[�𖳓G��Ԃɐݒ�
		CPlayer::SState state = pPlayer->GetState();
		state.state = CPlayer::STATE_INVINSIBLE;
		pPlayer->SetState(state);

		// �v���C���[�h�ɂ���
		CGame::SetState(CGame::STATE_PLAY);

		// �ʏ�J�����ɂ���
		CManager::Get()->GetCamera()->SetType(CCamera::TYPE_NONE);

		// �X�R�A�����炷
		pScore->SetScore(pScore->GetScore() - m_nSubScore);

		// �X�R�A�̌��Z�ʂ��ǂ�ǂ�{�ɂ��Ă���
		m_nSubScore *= 2;

		if (m_nSubScore >= MAX_SUB_SCORE)
		{ // ���_�̌��Z�ʂ��ő吔�𒴂����ꍇ

			// ���l��␳����
			m_nSubScore = MAX_SUB_SCORE;
		}
	}
}