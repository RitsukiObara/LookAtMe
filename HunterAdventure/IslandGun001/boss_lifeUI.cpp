//============================================
//
// �{�X�̎���UI����[boss_lifeUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "boss_lifeUI.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "object2D.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const float METER_WIDTH = 480.0f;		// ���[�^�[�̉���
	const D3DXVECTOR3 POLY_SIZE = D3DXVECTOR3(METER_WIDTH, 30.0f, 0.0f);		// �|���S���̃T�C�Y
	const D3DXVECTOR3 POLY_POS[CBossLifeUI::POLY_MAX] =			// �|���S���̈ʒu
	{
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - METER_WIDTH, 650.0f, 0.0f),
		D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - METER_WIDTH, 650.0f, 0.0f),
	};
	const D3DXVECTOR3 POLY_ROT = NONE_D3DXVECTOR3;			// �|���S���̌���
	const char* TEXTURE[CBossLifeUI::POLY_MAX] =			// �e�N�X�`��
	{
		"data\\TEXTURE\\BossLifeGage.png",
		"data\\TEXTURE\\BossLifeMeter.png",
	};
	const float METER_ADD = 1.0f;							// ���[�^�[�̉��Z��
}

//========================
// �R���X�g���N�^
//========================
CBossLifeUI::CBossLifeUI() : CObject(TYPE_LIFEUI, DIM_2D, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		m_apObject2D[nCnt] = nullptr;		// �|���S��
	}
	m_nLife = 0;							// ����
	m_fMeterDest = 0.0f;					// �ړI�̃��[�^�[
	m_fMeter = 0.0f;						// ���[�^�[
	m_fMemory = 0.0f;						// 1�������̒l
}

//========================
// �f�X�g���N�^
//========================
CBossLifeUI::~CBossLifeUI()
{

}

//========================
// ����������
//========================
HRESULT CBossLifeUI::Init(void)
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
void CBossLifeUI::Uninit(void)
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
void CBossLifeUI::Update(void)
{
	// �T�C�Y��ݒ肷��
	m_fMeterDest = m_fMemory * m_nLife;

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

	// ���_���W�̐ݒ菈��
	m_apObject2D[POLY_METER]->SetVtxWidthGage();

	// �e�N�X�`�����W�̐ݒ菈��
	m_apObject2D[POLY_METER]->SetVtxTextureWidth(m_fMeter / METER_WIDTH);
}

//========================
// �`�揈��
//========================
void CBossLifeUI::Draw(void)
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
void CBossLifeUI::SetData(const int nLife)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �|���S���� NULL ����Ȃ��ꍇ

			// ���̐ݒ菈��
			m_apObject2D[nCnt]->SetPos(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetPosOld(POLY_POS[nCnt]);
			m_apObject2D[nCnt]->SetRot(POLY_ROT);
			m_apObject2D[nCnt]->SetSize(POLY_SIZE);
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

			default:

				// ��~
				assert(false);

				break;
			}
		}
	}
	m_nLife = nLife;						// ����
	m_fMemory = METER_WIDTH / nLife;		// 1�������̒l
	m_fMeterDest = nLife * m_fMemory;		// �ړI�̃��[�^�[
	m_fMeter = m_fMeterDest;				// ���[�^�[
}

//========================
// ��������
//========================
CBossLifeUI* CBossLifeUI::Create(const int nLife)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBossLifeUI* pUI = nullptr;	// UI�̃C���X�^���X�𐶐�

	if (pUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pUI = new CBossLifeUI;
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
void CBossLifeUI::SetLife(const int nLife)
{
	// ������ݒ肷��
	m_nLife = nLife;
}

//========================
// �����̎擾����
//========================
int CBossLifeUI::GetLife(void) const
{
	// ������Ԃ�
	return m_nLife;
}