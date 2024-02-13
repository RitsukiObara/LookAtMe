//============================================
//
// ���xUI����[speedUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "speedUI.h"
#include "object2D.h"
#include "texture.h"

#include "tutorial_speedUI.h"
#include "game_speedUI.h"
#include "player.h"
#include "particle2D.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SPEEDUI_GAGE_SIZE			(D3DXVECTOR3(300.0f, 40.0f, 0.0f))								// ���xUI�̃Q�[�W�̃T�C�Y
#define SPEEDUI_METER_SIZE			(D3DXVECTOR3(0.0f, SPEEDUI_GAGE_SIZE.y, 0.0f))					// ���xUI�̃��[�^�[�̃T�C�Y
#define SPEEDUI_METER_MAX			(SPEEDUI_GAGE_SIZE.x)											// ���xUI�̃��[�^�[�̍ō��l
#define SPEEDUI_METER_CONVERSION	(SPEEDUI_METER_MAX / (PLAYER_MAX_SPEED - PLAYER_MIN_SPEED))		// ���xUI�̃��[�^�[�̕ϊ���
#define SPEEDUI_METER_TEXTURE		((float)(1.0f / (PLAYER_MAX_SPEED - PLAYER_MIN_SPEED)))			// ���xUI��1�������̃e�N�X�`���̒l
#define SPEEDUI_OVERTOP_TEXTURE		"data/TEXTURE/SpeedUIOver.png"									// ���xUI�̌��E�˔j�̃e�N�X�`��

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CSpeedUI::m_apTexturename[CSpeedUI::TYPE_MAX] =			// �e�N�X�`���̃p�X��
{
	"data/TEXTURE/SpeedUIGage.png",				// �Q�[�W
	"data/TEXTURE/SpeedUIMeter.png",			// ���[�^�[
	"data/TEXTURE/SpeedUIMark.png",				// �}�[�N
};

//============================
// �R���X�g���N�^
//============================
CSpeedUI::CSpeedUI() : CObject(CObject::TYPE_SPEEDUI, PRIORITY_UI)
{
	// �R���X�g���N�^�̔�
	Box();
}

//============================
// �I�[�o�[���[�h�R���X�g���N�^
//============================
CSpeedUI::CSpeedUI(CObject::TYPE type, PRIORITY priority) : CObject(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//============================
// �f�X�g���N�^
//============================
CSpeedUI::~CSpeedUI()
{

}

//============================
// �R���X�g���N�^�̔�
//============================
void CSpeedUI::Box(void)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;			// 2D�I�u�W�F�N�g�̏��
	}

	m_fLevel = 0.0f;				// ���x��
	m_fLevelDest = 0.0f;			// �ڕW�̃��x��
	m_nOverTopTex = NONE_TEXIDX;	// �X�[�p�[�u�[�X�g���̃e�N�X�`���̃C���f�b�N�X
	m_nMeterTex = NONE_TEXIDX;		// ���[�^�[�̃e�N�X�`���̃C���f�b�N�X�̕ۑ��ϐ�
}

//============================
// ����������
//============================
HRESULT CSpeedUI::Init(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// ��������
		m_apObject2D[nCntUI] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, CObject::PRIORITY_UI);

		switch (nCntUI)
		{
		case TYPE_GAGE:		// �Q�[�W

			m_apObject2D[nCntUI]->SetSize(SPEEDUI_GAGE_SIZE);			// �T�C�Y�ݒ�

			break;

		case TYPE_METER:	// ���[�^�[

			m_apObject2D[nCntUI]->SetSize(SPEEDUI_METER_SIZE);			// �T�C�Y�ݒ�

			break;
				
		case TYPE_MARK:		// �}�[�N

			m_apObject2D[nCntUI]->SetSize(SPEEDUI_MARK_SIZE);			// �T�C�Y�ݒ�

			break;
		}

		// �X�N���[���̐ݒ菈��
		m_apObject2D[nCntUI]->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �ʒu�ݒ�
		m_apObject2D[nCntUI]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �����ݒ�
		m_apObject2D[nCntUI]->SetLength();								// �����ݒ�
		m_apObject2D[nCntUI]->SetAngle();								// �����ݒ�

		if (nCntUI == TYPE_MARK)
		{ // �}�[�N�̏ꍇ

			// ���_���̏�����
			m_apObject2D[nCntUI]->SetVertex();
		}
		else
		{ // ��L�ȊO

			// ���_���̏�����
			m_apObject2D[nCntUI]->SetVtxWidthGage();
		}

		// �e�N�X�`���̊��蓖�ď���
		m_apObject2D[nCntUI]->BindTexture(CManager::GetTexture()->Regist(m_apTexturename[nCntUI]));
	}

	m_fLevel = 0.0f;			// ���x��
	m_fLevelDest = 0.0f;		// �ڕW�̃��x��
	m_nOverTopTex = CManager::GetTexture()->Regist(SPEEDUI_OVERTOP_TEXTURE);	// �X�[�p�[�u�[�X�g���̃e�N�X�`���̃C���f�b�N�X�̕ۑ��ϐ�
	m_nMeterTex = m_apObject2D[TYPE_METER]->GetTexIdx();						// ���[�^�[�̃e�N�X�`���̃C���f�b�N�X�̕ۑ��ϐ�

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CSpeedUI::Uninit(void)
{
	for (int nCntUI = 0; nCntUI < TYPE_MAX; nCntUI++)
	{
		// �I������
		m_apObject2D[nCntUI]->Uninit();
		m_apObject2D[nCntUI] = nullptr;
	}

	// �j������
	Release();
}

//============================
// �X�V����
//============================
void CSpeedUI::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size;		// �T�C�Y

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

			switch (nCnt)
			{
			case TYPE_GAGE:			// �Q�[�W

				// ���_���̐ݒ菈��
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case TYPE_METER:		// ���[�^�[

				// ���[�^�[�ݒ菈��
				SetSpeedMeter();

				// �T�C�Y�̕␳����
				SizeCorrect();

				if (CScene::GetPlayer()->GetState() == CPlayer::STATE_BOOST)
				{ // �u�[�X�g��Ԃ̏ꍇ

					if (m_apObject2D[TYPE_METER]->GetTexIdx() != m_nOverTopTex)
					{ // �e�N�X�`����������ꍇ

						// �X�[�p�[�u�[�X�g�̃e�N�X�`�������蓖�Ă�
						m_apObject2D[TYPE_METER]->BindTexture(m_nOverTopTex);
					}

					// �e�N�X�`�����W�̐ݒ菈��
					m_apObject2D[nCnt]->SetVtxTexture();
				}
				else
				{ // ��L�ȊO

					if (m_apObject2D[TYPE_METER]->GetTexIdx() != m_nMeterTex)
					{ // �e�N�X�`����������ꍇ

						// �ʏ�̃e�N�X�`�������蓖�Ă�
						m_apObject2D[TYPE_METER]->BindTexture(m_nMeterTex);
					}

					// �e�N�X�`�����W�̐ݒ菈��
					m_apObject2D[nCnt]->SetVtxTextureWidth(m_fLevel * SPEEDUI_METER_TEXTURE);
				}

				// ���_���̐ݒ菈��
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case TYPE_MARK:			// �}�[�N

				// ���_���̐ݒ菈��
				m_apObject2D[nCnt]->SetVertex();

				break;
			}
		}
	}
}

//============================
// �`�揈��
//============================
void CSpeedUI::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // 2D�I�u�W�F�N�g�̏�� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apObject2D[nCnt]->Draw();
		}
	}
}

//============================
// �I�u�W�F�N�g�̎擾����
//============================
CObject2D* CSpeedUI::GetObject2D(const TYPE type)
{
	// �I�u�W�F�N�g�̏���Ԃ�
	return m_apObject2D[type];
}

//============================
//��������
//============================
CSpeedUI* CSpeedUI::Create(const CScene::MODE mode)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSpeedUI* pSpeedUI = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pSpeedUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:		// �`���[�g���A��

			// �I�u�W�F�N�g�𐶐�
			pSpeedUI = new CTutorialSpeedUI;

			break;

		case CScene::MODE_GAME:			// �Q�[��

			// �I�u�W�F�N�g�𐶐�
			pSpeedUI = new CGameSpeedUI;

			break;

		default:		// ��L�ȊO

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

	if (pSpeedUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pSpeedUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���xUI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pSpeedUI->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���xUI�̃|�C���^��Ԃ�
	return pSpeedUI;
}

//============================
// ���[�^�[�ݒ菈��
//============================
void CSpeedUI::SetSpeedMeter(void)
{
	// ���[�J���|�C���^��錾
	CPlayer* pPlayer = CScene::GetPlayer();						// �v���C���[�̃|�C���^���擾

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// ���x����ݒ肷��
		m_fLevelDest = (pPlayer->GetSpeed() - (int)(PLAYER_MIN_SPEED));
	}
}

//============================
// �T�C�Y�̕␳����
//============================
void CSpeedUI::SizeCorrect(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = m_apObject2D[TYPE_METER]->GetSize();		// �T�C�Y���擾����

	// �␳����
	Correct(m_fLevelDest, &m_fLevel, 0.5f);

	// �ړI�̑��x��ݒ肷��
	size.x = m_fLevel * SPEEDUI_METER_CONVERSION;

	// �T�C�Y���X�V����
	m_apObject2D[TYPE_METER]->SetSize(size);
}