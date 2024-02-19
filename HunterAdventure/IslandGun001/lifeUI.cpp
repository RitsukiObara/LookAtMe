//============================================
//
// ����UI����[lifeUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "lifeUI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const float METER_WIDTH = 160.0f;		// ���[�^�[�̉���
	const D3DXVECTOR3 POLY_POS[CLifeUI::POLY_MAX] =			// �|���S���̈ʒu
	{
		D3DXVECTOR3(47.0f, 40.0f, 0.0f),
		D3DXVECTOR3(47.0f, 40.0f, 0.0f),
		D3DXVECTOR3(50.0f, 30.0f, 0.0f),
	};
	const D3DXVECTOR3 POLY_ROT[CLifeUI::POLY_MAX] =			// �|���S���̌���
	{
		NONE_D3DXVECTOR3,
		NONE_D3DXVECTOR3,
		D3DXVECTOR3(0.0f,0.0f,D3DX_PI * -0.25f),
	};
	const D3DXVECTOR3 POLY_SIZE[CLifeUI::POLY_MAX] =		// �|���S���̃T�C�Y
	{
		D3DXVECTOR3(METER_WIDTH, 10.0f, 0.0f),
		D3DXVECTOR3(METER_WIDTH, 10.0f, 0.0f),
		D3DXVECTOR3(20.0f, 20.0f, 0.0f),
	};
	const char* TEXTURE[CLifeUI::POLY_MAX] =		// �e�N�X�`��
	{
		"data\\TEXTURE\\LifeUIGage.png",
		"data\\TEXTURE\\LifeUIMeter.png",
		"data\\TEXTURE\\LifeUIIcon.png"
	};
	const float METER_MEMORY = METER_WIDTH * 0.01f;		// ���[�^�[��1�������̃T�C�Y
	const float METER_ADD = 1.0f;						// ���[�^�[�̉��Z��
	const int ICON_FLASH_LIFE = 30;						// �A�C�R�������ł������̗�
	const D3DXCOLOR ICON_FLASH_COL = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);		// �A�C�R���̓_�Ŏ��̐F
	const float ICON_CALC_COL = 0.05f;					// �A�C�R���̐F�̈ړ���
}

//========================
// �R���X�g���N�^
//========================
CLifeUI::CLifeUI() : CObject(TYPE_LIFEUI, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;		// �e�̏��
	}
	m_iconflash.col = NONE_D3DXCOLOR;		// �A�C�R���̐F
	m_iconflash.bAdd = false;				// �A�C�R���̉��Z��
	m_nLife = 0;							// ����
	m_fMeterDest = 0.0f;					// �ړI�̃��[�^�[
	m_fMeter = 0.0f;						// ���[�^�[
}

//========================
// �f�X�g���N�^
//========================
CLifeUI::~CLifeUI()
{

}

//========================
// ����������
//========================
HRESULT CLifeUI::Init(void)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] == nullptr)
		{ // �|���S���� NULL ����Ȃ��ꍇ

			// �|���S���𐶐�
			m_apObject2D[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);
		}
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CLifeUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // ��� NULL ����Ȃ��ꍇ

			// �|���S���̏I������
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	// �{�̂̏I������
	Release();
}

//========================
// �X�V����
//========================
void CLifeUI::Update(void)
{
	// �T�C�Y��ݒ肷��
	m_fMeterDest = METER_MEMORY * m_nLife;

	// �ϓ��Ȑ��l�̕␳����
	useful::FrameCorrect(m_fMeterDest, &m_fMeter, METER_ADD);

	{ // �T�C�Y�̓K�p

		// �T�C�Y���擾����
		D3DXVECTOR3 size = m_apObject2D[POLY_METER]->GetSize();

		// �T�C�Y��ݒ肷��
		size.x = m_fMeter;

		// �T�C�Y��K�p����
		m_apObject2D[POLY_METER]->SetSize(size);
	}

	// �A�C�R���̓_�ŏ���
	IconFlash();

	// ���_���W�̐ݒ菈��
	m_apObject2D[POLY_METER]->SetVtxWidthGage();

	// �e�N�X�`�����W�̐ݒ菈��
	m_apObject2D[POLY_METER]->SetVtxTextureWidth(m_fMeter / METER_WIDTH);
}

//========================
// �`�揈��
//========================
void CLifeUI::Draw(void)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �|���S���� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apObject2D[nCnt]->Draw();
		}
	}
}

//========================
// ���̐ݒ菈��
//========================
void CLifeUI::SetData(const int nLife)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �|���S���� NULL ����Ȃ��ꍇ

			// ���̐ݒ菈��
			m_apObject2D[nCnt]->SetPos(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetPosOld(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetRot(POLY_ROT[nCnt]);
			m_apObject2D[nCnt]->SetSize(POLY_SIZE[nCnt]);
			m_apObject2D[nCnt]->SetAngle();
			m_apObject2D[nCnt]->SetLength();

			// ���_���W�̐ݒ菈��
			m_apObject2D[nCnt]->SetVertex();

			// �e�N�X�`���̊��蓖�ď���
			m_apObject2D[nCnt]->BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]));

			switch (nCnt)
			{
			case POLY::POLY_GROUND:

				// ���_���W�̐ݒ菈��
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case POLY::POLY_METER:

				// ���_���W�̐ݒ菈��
				m_apObject2D[nCnt]->SetVtxWidthGage();

				break;

			case POLY::POLY_WIPE:

				// ���_���W�̐ݒ菈��
				m_apObject2D[nCnt]->SetVertexRot();

				break;

			default:

				// ��~
				assert(false);

				break;
			}
		}
	}
	m_iconflash.col = NONE_D3DXCOLOR;		// �A�C�R���̐F
	m_iconflash.bAdd = false;				// �A�C�R���̉��Z��
	m_nLife = nLife;						// ����
	m_fMeterDest = nLife * METER_MEMORY;	// �ړI�̃��[�^�[
	m_fMeter = m_fMeterDest;				// ���[�^�[
}

//========================
// ��������
//========================
CLifeUI* CLifeUI::Create(const int nLife)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CLifeUI* pUI = nullptr;	// UI�̃C���X�^���X�𐶐�

	if (pUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pUI = new CLifeUI;
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
		pUI->SetData(nLife);
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
// �����̐ݒ菈��
//========================
void CLifeUI::SetLife(const int nLife)
{
	// ������ݒ肷��
	m_nLife = nLife;
}

//========================
// �����̎擾����
//========================
int CLifeUI::GetLife(void) const
{
	// ������Ԃ�
	return m_nLife;
}

//========================
// �A�C�R���̓_�ŏ���
//========================
void CLifeUI::IconFlash(void)
{
	if (m_nLife <= ICON_FLASH_LIFE)
	{ // �̗͂����ȉ��̏ꍇ

		if (m_iconflash.bAdd == true)
		{ // ���Z�󋵂� true �̏ꍇ

			// �F�𔒂ɋ߂Â���
			m_iconflash.col.g += ICON_CALC_COL;
			m_iconflash.col.b += ICON_CALC_COL;

			if (m_iconflash.col.g >= NONE_D3DXCOLOR.g &&
				m_iconflash.col.b >= NONE_D3DXCOLOR.b)
			{ // ���ɂȂ����ꍇ

				// �F��ݒ肷��
				m_iconflash.col = NONE_D3DXCOLOR;

				// ���Z�󋵂� false �ɂ���
				m_iconflash.bAdd = false;
			}
		}
		else
		{ // ��L�ȊO

			// �F��Ԃɋ߂Â���
			m_iconflash.col.g -= ICON_CALC_COL;
			m_iconflash.col.b -= ICON_CALC_COL;

			if (m_iconflash.col.g <= ICON_FLASH_COL.g &&
				m_iconflash.col.b <= ICON_FLASH_COL.b)
			{ // �ԂɂȂ����ꍇ

				// �F��ݒ肷��
				m_iconflash.col = ICON_FLASH_COL;

				// ���Z�󋵂� true �ɂ���
				m_iconflash.bAdd = true;
			}
		}
	}
	else
	{ // ��L�ȊO

		// �A�C�R���̐F��ݒ肷��
		m_iconflash.col = NONE_D3DXCOLOR;
	}

	// ���_�J���[�̐ݒ菈��
	m_apObject2D[POLY_WIPE]->SetVtxColor(m_iconflash.col);
}