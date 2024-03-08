//===========================================
//
// ����̃��C������[lake.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "lake.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* TEXTURE[CLake::NUM_MAX] =				// �e�N�X�`��
	{
		"data\\TEXTURE\\Lake001.png",				// �g��
		"data\\TEXTURE\\Lake002.png",				// �C
	};
	const float TEXTURE_VTX_MAX = 1.0f;					// �e�N�X�`�����W�̍ő�l
	const D3DXVECTOR2 TEXTURE_MOVE[CLake::NUM_MAX] =	// �e�N�X�`�����W�̈ړ���
	{
		D3DXVECTOR2(0.004f, -0.002f),		// �g��
		D3DXVECTOR2(0.003f, 0.001f)			// �C
	};		
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CLake*> CLake::m_list = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CLake::CLake() : CObject3DMultiTex(TYPE_LAKE, PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		m_aTexU[nCnt] = NONE_D3DXVECTOR2;			// �e�N�X�`�����W�̊J�n�ʒu(U�l)
	}

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CLake::~CLake()
{

}

//==============================
// ����̏���������
//==============================
HRESULT CLake::Init(void)
{
	if (FAILED(CObject3DMultiTex::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
// ����̏I������
//========================================
void CLake::Uninit(void)
{
	// �I������
	CObject3DMultiTex::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
// ����̍X�V����
//========================================
void CLake::Update(void)
{
	for (int nCntBG = 0; nCntBG < NUM_MAX; nCntBG++)
	{
		//�ړ��ʂ����Z����
		m_aTexU[nCntBG].x += TEXTURE_MOVE[nCntBG].x;
		m_aTexU[nCntBG].y += TEXTURE_MOVE[nCntBG].y;

		if (m_aTexU[nCntBG].x >= TEXTURE_VTX_MAX ||
			m_aTexU[nCntBG].x <= -TEXTURE_VTX_MAX)
		{//�w�i����ԉ��܂ōs�����ꍇ

			//�w�i��0�ɖ߂�
			m_aTexU[nCntBG].x = 0.0f;
		}

		if (m_aTexU[nCntBG].y >= TEXTURE_VTX_MAX ||
			m_aTexU[nCntBG].y <= -TEXTURE_VTX_MAX)
		{//�w�i����ԉ��܂ōs�����ꍇ

			//�w�i��0�ɖ߂�
			m_aTexU[nCntBG].y = 0.0f;
		}

		// �e�N�X�`�����W�̐ݒ菈��
		SetVtxTextureAnim(m_aTexU[nCntBG], nCntBG);
	}
}

//=====================================
// ����̕`�揈��
//=====================================
void CLake::Draw(void)
{
	// �`�揈��
	CObject3DMultiTex::DrawLightOff();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CLake::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ���̐ݒ菈��
	SetPos(pos);
	SetPosOld(pos);
	SetRot(NONE_D3DXVECTOR3);
	SetSize(size);
	SetType(CObject3DMultiTex::TYPE::TYPE_FUSION);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// �e�N�X�`���̊��蓖�ď���
		BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[nCnt]), nCnt);
	}

	// ���_���W�̐ݒ菈��
	SetVertex();

	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < NUM_MAX; nCnt++)
	{
		m_aTexU[nCnt] = NONE_D3DXVECTOR2;			// �e�N�X�`�����W�̊J�n�ʒu(U�l)
	}
}

//=======================================
// ��������
//=======================================
CLake* CLake::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CLake* pRock = nullptr;		// �C���X�^���X�𐶐�

	if (pRock == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��𐶐�����
		pRock = new CLake;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRock != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRock->SetData(pos, size);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ��̃|�C���^��Ԃ�
	return pRock;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CLake*> CLake::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}