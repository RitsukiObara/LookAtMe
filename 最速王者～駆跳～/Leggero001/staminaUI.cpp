//============================================
//
// �X�^�~�iUI����[staminaUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "staminaUI.h"
#include "game_staminaUI.h"
#include "tutorial_staminaUI.h"
#include "object2D.h"
#include "animobject.h"
#include "texture.h"

#include "player.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define STAMINAUI_CONVERSION		(3.0f)																		// �ϊ��{��
#define STAMINAUI_GAGE_SIZE			(D3DXVECTOR3(PLAYER_MAX_STAMINA * STAMINAUI_CONVERSION, 40.0f, 0.0f))		// �X�^�~�iUI�̃T�C�Y
#define STAMINAUI_SUB_MAGNI			(0.1f)																		// �X�^�~�i�̌��Z�ʂ̔{��
#define STAMINAUI_COLOR				(1.0f / PLAYER_MAX_STAMINA)													// �X�^�~�iUI�̐F�̔{��
#define STAMINAUI_FLASH				(PLAYER_MAX_STAMINA * 0.3f)													// �X�^�~�iUI���_�ł���X�^�~�i��
#define STAMINAUI_FLASH_COUNT		(8)																			// �X�^�~�iUI���_�ł���J�E���g��
#define STAMINAUI_FLASH_COLOR		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))											// �X�^�~�iUI���_�ł��Ă��鎞�̐F
#define STAMINAUI_ANIM_COUNT_GO		(180)																		// �X�^�~�iUI�������^�C�~���O
#define STAMINAUI_ANIM_COUNT_BACK	(10)																		// �X�^�~�iUI���߂�܂ł̎���
#define STAMINAUI_ANIM_PATTERN		(2)																			// �X�^�~�iUI�̃A�j���[�V�����p�^�[��
#define STAMINAUI_HEALSTATE_COUNT	(50)																		// �X�^�~�iUI�̉񕜏�Ԃ̃J�E���g��
#define STAMINAUI_METER_TEXTURE		((float)(1.0f / STAMINAUI_GAGE_SIZE.x))										// ���xUI��1�������̃e�N�X�`���̒l

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CStaminaUI::m_apTexturename[CStaminaUI::TEX_MAX] =			// �e�N�X�`���̃p�X��
{
	"data/TEXTURE/StaminaGage.png",			// �Q�[�W
	"data/TEXTURE/StaminaMeter.png",		// ���[�^�[
	"data/TEXTURE/StaminaMark.png",			// �}�[�N
};

//============================
// �R���X�g���N�^
//============================
CStaminaUI::CStaminaUI() : CObject(TYPE_STAMINAUI, PRIORITY_UI)
{
	// �R���X�g���N�^�̔�
	Box();
}

//============================
// �I�[�o�[���[�h�R���X�g���N�^
//============================
CStaminaUI::CStaminaUI(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//============================
// �f�X�g���N�^
//============================
CStaminaUI::~CStaminaUI()
{

}

//============================
// �R���X�g���N�^�̔�
//============================
void CStaminaUI::Box(void)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;		// 2D�I�u�W�F�N�g�̏��
	}

	m_pAnim = nullptr;			// �A�j���[�V�����̏��
	m_fStaminaDest = 0.0f;		// �ڕW�̃X�^�~�i
	m_fSubStamina = 0.0f;		// �X�^�~�i�̌��Z�l
	m_bFlash = false;			// �_�ŏ�
	m_nFlashCount = 0;			// �_�ŃJ�E���g
	m_nStateCount = 0;			// ��ԃJ�E���g
	m_state = STATE_NONE;		// ���
}

//============================
// ����������
//============================
HRESULT CStaminaUI::Init(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// ��������
		m_apObject2D[nCntUI] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

		if (m_apObject2D[nCntUI] != nullptr)
		{ // �I�u�W�F�N�g�̏�� NULL ����Ȃ��ꍇ

			// �X�N���[���̐ݒ菈��
			m_apObject2D[nCntUI]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �ʒu�ݒ�
			m_apObject2D[nCntUI]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �����ݒ�
			m_apObject2D[nCntUI]->SetSize(STAMINAUI_GAGE_SIZE);				// �T�C�Y�ݒ�
			m_apObject2D[nCntUI]->SetLength();								// �����ݒ�
			m_apObject2D[nCntUI]->SetAngle();								// �����ݒ�
			
			// ���_���̏�����
			m_apObject2D[nCntUI]->SetVtxWidthGage();

			// �e�N�X�`���̊��蓖�ď���
			m_apObject2D[nCntUI]->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[nCntUI]));
		}

			// ���_���̏�����
			m_apObject2D[nCntUI]->SetVertex();
	}

	if (m_pAnim == nullptr)
	{ // �A�j���[�V�����̏�� NULL �̏ꍇ

		// �A�j���[�V�����̐���
		m_pAnim = CAnim::Create(CObject::TYPE_NONE, CObject::PRIORITY_UI);

		if (m_pAnim != nullptr)
		{ // �A�j���[�V�����̏�� NULL ����Ȃ��ꍇ

			// ���̐ݒ菈��
			m_pAnim->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �ʒu
			m_pAnim->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
			m_pAnim->SetSize(STAMINAUI_MARK_SIZE);				// �T�C�Y
			m_pAnim->SetAngle();								// ����
			m_pAnim->SetLength();								// ����
			m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_GO, STAMINAUI_ANIM_PATTERN);		// �A�j���[�V�����̏��

			// �e�N�X�`���̊��蓖�ď���
			m_pAnim->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[TEX_MARK]));

			// ���_�̐ݒ菈��
			m_pAnim->SetVertex();

			// �e�N�X�`�����W�̐ݒ菈��
			m_pAnim->SetVtxTextureAnim(m_pAnim->GetTexPattern(), m_pAnim->GetPattern());
		}
	}

	// ���̏�����
	m_fStaminaDest = m_apObject2D[TYPE_METER]->GetSize().x;		// �ڕW�̃X�^�~�i
	m_fSubStamina = 0.0f;										// �X�^�~�i�̌��Z�l
	m_bFlash = false;											// �_�ŏ�
	m_nFlashCount = 0;											// �_�ŃJ�E���g
	m_nStateCount = 0;											// ��ԃJ�E���g
	m_state = STATE_NONE;										// ���

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CStaminaUI::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// �I������
		m_apObject2D[nCntUI]->Uninit();
		m_apObject2D[nCntUI] = nullptr;
	}

	// �A�j���[�V�����̏I������
	m_pAnim->Uninit();
	m_pAnim = nullptr;

	// �j������
	Release();
}

//============================
// �X�V����
//============================
void CStaminaUI::Update(void)
{
	// �X�^�~�i�̔��f����
	StaminaInfluence();

	// �T�C�Y�̌��Z����
	SizeSub();

	// �F�̐ݒ菈��
	ColorCalc();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

			// ���_���̐ݒ菈��
			m_apObject2D[nCnt]->SetVtxWidthGage();
		}
	}

	switch (m_state)
	{
	case STATE_NONE:		// �ʏ���

		if (m_pAnim != nullptr)
		{ // �A�j���[�V�����̏�� NULL ����Ȃ������ꍇ

			// �ʏ��Ԃ̏���
			NoneStateProcess();
		}

		break;

	case STATE_HEAL:		// �񕜏��

		if (m_pAnim != nullptr)
		{ // �A�j���[�V�����̏�� NULL ����Ȃ������ꍇ

			// �񕜏�Ԃ̏���
			HealStateProcess();
		}

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount >= STAMINAUI_HEALSTATE_COUNT)
		{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;

			// ��ԃJ�E���g������������
			m_nStateCount = 0;
		}

		break;
	}

	// ���_���W�̐ݒ菈��
	m_pAnim->SetVertex();
}

//============================
// �`�揈��
//============================
void CStaminaUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // 2D�I�u�W�F�N�g�̏�� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apObject2D[nCnt]->Draw();
		}
	}

	if (m_pAnim != nullptr)
	{ // �A�j���[�V������ NULL ����Ȃ��ꍇ

		// �A�j���[�V�����̕`�揈��
		m_pAnim->Draw();
	}
}

//============================
// ��Ԃ̐ݒ菈��
//============================
void CStaminaUI::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//============================
// ��Ԃ̎擾����
//============================
CStaminaUI::STATE CStaminaUI::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================
// �I�u�W�F�N�g2D�̎擾����
//============================
CObject2D* CStaminaUI::GetObject2D(const TYPE type) const
{
	// �I�u�W�F�N�g2D�̏���Ԃ�
	return m_apObject2D[type];
}

//============================
// �A�j���[�V�����̎擾����
//============================
CAnim* CStaminaUI::GetAnim(void) const
{
	// �A�j���[�V�����̏���Ԃ�
	return m_pAnim;
}

//============================
//��������
//============================
CStaminaUI* CStaminaUI::Create(const CScene::MODE mode)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CStaminaUI* pStamina = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pStamina == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:		// �`���[�g���A��

			// �I�u�W�F�N�g�𐶐�
			pStamina = new CTutorialStaminaUI;

			break;

		case CScene::MODE_GAME:			// �Q�[��

			// �I�u�W�F�N�g�𐶐�
			pStamina = new CGameStaminaUI;

			break;

		default:			// ��L�ȊO

			// NULL ��Ԃ�
			return nullptr;

			break;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pStamina != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pStamina->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�X�^�~�iUI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���[�h�̐ݒ菈��
		pStamina->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pStamina;
}

//============================
// �X�^�~�i���f����
//============================
void CStaminaUI::StaminaInfluence(void)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = nullptr;					// �v���C���[�̃|�C���^
	float fStaminaDestOld = m_fStaminaDest;		// �O��̃X�^�~�i�l

	// �v���C���[�̏����擾����
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �ڕW�̃X�^�~�i��ݒ肷��
		m_fStaminaDest = pPlayer->GetStamina() * STAMINAUI_CONVERSION;

		if (m_fStaminaDest != fStaminaDestOld)
		{ // �X�^�~�i�l���ω����Ă����ꍇ

			// �X�^�~�i�̌��Z�ʌv�Z����
			CalcStaminaSub();
		}
	}
}

//============================
// �T�C�Y�̌��Z����
//============================
void CStaminaUI::SizeSub(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apObject2D[TYPE_METER]->GetSize();		// �T�C�Y���擾����

	if (m_fStaminaDest < m_apObject2D[TYPE_METER]->GetSize().x)
	{ // �ڕW�̃X�^�~�i�ɒB���Ă��Ȃ��ꍇ

		// �T�C�Y�����炷
		size.x += m_fSubStamina;
	}
	else
	{ // ��L�ȊO

		// �X�^�~�i�̒l��␳����
		size.x = m_fStaminaDest;

		// �X�^�~�i�̍���������������
		m_fSubStamina = 0.0f;
	}

	// �T�C�Y��ݒ肷��
	m_apObject2D[TYPE_METER]->SetSize(size);

	// �e�N�X�`���̐ݒ菈��
	m_apObject2D[TYPE_METER]->SetVtxTextureWidth(size.x * STAMINAUI_METER_TEXTURE);
}

//============================
// �X�^�~�i�̌��Z�ʌv�Z����
//============================
void CStaminaUI::CalcStaminaSub(void)
{
	if (m_apObject2D[TYPE_METER] != nullptr)
	{ // ���[�^�[�̃|�C���^���g���Ă���ꍇ

		// �X�^�~�i�̌��Z�ʂ��Z�o����
		m_fSubStamina = (m_fStaminaDest - m_apObject2D[TYPE_METER]->GetSize().x) * STAMINAUI_SUB_MAGNI;
	}
}

//============================
// �X�^�~�i�̃J���[�v�Z����
//============================
void CStaminaUI::ColorCalc(void)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = nullptr;					// �v���C���[�̃|�C���^

	// �v���C���[�̏����擾����
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		if (pPlayer->GetStamina() <= STAMINAUI_FLASH)
		{ // �X�^�~�i����萔�ȉ��ɂȂ����ꍇ

			// �_�ŃJ�E���g�����Z����
			m_nFlashCount++;

			if(m_nFlashCount % STAMINAUI_FLASH_COUNT == 0)
			{ // �_�ŃJ�E���g����萔�ɂȂ����ꍇ

				// �_�ŏ󋵂�؂�ւ���
				m_bFlash = m_bFlash ? false : true;
			}

			if (m_bFlash == true)
			{ // �_�ŏ󋵂� true �������ꍇ

				// ���_�J���[�̐ݒ菈��
				m_apObject2D[TYPE_METER]->SetVtxColor(STAMINAUI_FLASH_COLOR);
			}
			else
			{ // ��L�ȊO

				// ���_�J���[�̐ݒ菈��
				m_apObject2D[TYPE_METER]->SetVtxColor(D3DXCOLOR(1.0f - STAMINAUI_COLOR * pPlayer->GetStamina(), STAMINAUI_COLOR * pPlayer->GetStamina(), 0.0f, 1.0f));
			}
		}
		else
		{ // �X�^�~�i���܂��\���ɂ������ꍇ

			// ���_�J���[�̐ݒ菈��
			m_apObject2D[TYPE_METER]->SetVtxColor(D3DXCOLOR(1.0f - STAMINAUI_COLOR * pPlayer->GetStamina(), STAMINAUI_COLOR * pPlayer->GetStamina(), 0.0f, 1.0f));

			// �_�ŏ󋵂� false �ɂ���
			m_bFlash = false;

			// �_�ŃJ�E���g������������
			m_nFlashCount = 0;
		}
	}
}

//============================
// �ʏ��Ԃ̏���
//============================
void CStaminaUI::NoneStateProcess(void)
{
	// �A�j���[�V�����̍X�V����
	m_pAnim->Update();

	if (m_pAnim->GetPattern() == 0)
	{ // �A�j���[�V������ 0�R�}�ڂ̏ꍇ

		// �A�j���[�V������ݒ肷��
		m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_GO, STAMINAUI_ANIM_PATTERN);
	}
	else
	{ // ��L�ȊO

		// �A�j���[�V������ݒ肷��
		m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_BACK, STAMINAUI_ANIM_PATTERN);
	}
}

//============================
// �񕜏�Ԃ̏���
//============================
void CStaminaUI::HealStateProcess(void)
{
	// �A�j���[�V�����̍X�V����
	m_pAnim->Update();

	// �A�j���[�V������ݒ肷��
	m_pAnim->SetAnim(STAMINAUI_ANIM_COUNT_BACK, STAMINAUI_ANIM_PATTERN);
}