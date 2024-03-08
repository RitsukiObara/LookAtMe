//===========================================
//
// ��̃��C������[rock.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "rock.h"
#include "texture.h"
#include "sound.h"
#include "useful.h"

#include "fraction.h"

//-------------------------------------------
// �萔��`
//-------------------------------------------
namespace
{
	const char* MODEL[CRock::TYPE_MAX] =		// ���f���̖��O
	{
		"data\\MODEL\\Rock001.x",				// ���F��
		"data\\MODEL\\Rock002.x",				// �D�F��
		"data\\MODEL\\Rock001.x",				// �����
	};
	const char* BREAKTEXTURE = "data\\TEXTURE\\Rock002.png";	// �j��ł����̃e�N�X�`��
	const int TEXTURE_IDX = 0;					// �ς��e�N�X�`���̃C���f�b�N�X
	const int NUM_FRACTION = 10;				// �j��̎��̔j�Ђ̐�
	const int FRACTION_MOVE_WIDTH = 20;			// �j�Ђ̕�
	const int FRACTION_MOVE_HEIGHT = 10;		// �j�Ђ̍���
	const int FRACTION_LIFE = 60;				// �j�Ђ̎���
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CRock*> CRock::m_aList[area::NUM_AREA] = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CRock::CRock() : CModel(TYPE_ROCK, PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_type = TYPE_BROWN;		// ���
	m_fRadius = 0.0f;			// ���a
	m_fTop = 0.0f;				// ��̍���
	m_fBottom = 0.0f;			// ���̍���
	m_nFieldIdx = 0;			// �敪�̔ԍ�
}

//==============================
// �f�X�g���N�^
//==============================
CRock::~CRock()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CRock::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CRock::Uninit(void)
{
	// ������������
	m_aList[m_nFieldIdx].Pull(this);

	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CRock::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CRock::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CRock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(rot);					// ����
	SetScale(scale);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL[type]));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_type = type;				// ���

	if (m_type == TYPE_BREAK)
	{ // �����̏ꍇ

		// �e�N�X�`���̊��蓖�ď���
		BindTexture(CManager::Get()->GetTexture()->Regist(BREAKTEXTURE), TEXTURE_IDX);
	}

	// ���a��ݒ肷��
	m_fRadius = GetFileData().fRadius * ((scale.x + scale.z) * 0.5f);

	// ������ݒ肷��
	m_fTop = GetFileData().vtxMax.y * scale.y;
	m_fBottom = GetFileData().vtxMin.y * scale.y;

	// �敪�̐ݒ菈��
	m_nFieldIdx = area::SetFieldIdx(GetPos());

	// ���X�g�ɒǉ�����
	m_aList[m_nFieldIdx].Regist(this);
}

//=====================================
// �j�󏈗�
//=====================================
void CRock::Break(void)
{
	// �I������
	Uninit();

	D3DXVECTOR3 pos = GetPos();		// ��̈ʒu
	D3DXVECTOR3 posFrac;			// �j�Ђ̈ʒu
	int nRadius = (int)(m_fRadius * 0.5f);	// �o��͈�(XZ��)
	int nHeight = (int)(m_fTop * 0.5f);		// �o��͈�(Y��)

	for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
	{
		// �j�Ђ̈ʒu��ݒ肷��
		posFrac.x = pos.x + (float)(rand() % nRadius - (nRadius / 2));
		posFrac.y = pos.y + (float)(rand() % nHeight - (nHeight / 2));
		posFrac.z = pos.z + (float)(rand() % nRadius - (nRadius / 2));

		// �j�Ђ𐶐�
		CFraction::Create(posFrac, CFraction::TYPE_ROCK, FRACTION_LIFE, FRACTION_MOVE_WIDTH, FRACTION_MOVE_HEIGHT);
	}

	// ��̔j�󉹂�炷
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_ROCKBREAK);
}

//=======================================
// ��������
//=======================================
CRock* CRock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRock* pRock = nullptr;		// �C���X�^���X�𐶐�

	if (pRock == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��𐶐�����
		pRock = new CRock;
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
		pRock->SetData(pos, rot, scale, type);
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
CListManager<CRock*> CRock::GetList(const int nIdx)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_aList[nIdx];
}

//=======================================
// ��ނ̎擾����
//=======================================
CRock::TYPE CRock::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//=======================================
// ���a�̎擾����
//=======================================
float CRock::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_fRadius;
}

//=======================================
// ��̍����̎擾����
//=======================================
float CRock::GetTopHeight(void) const
{
	// ��̍�����Ԃ�
	return m_fTop;
}

//=======================================
// ���̍����̎擾����
//=======================================
float CRock::GetBottomHeight(void) const
{
	// ���̍�����Ԃ�
	return m_fBottom;
}