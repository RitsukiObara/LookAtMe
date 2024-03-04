//===========================================
//
// �ǂ̃��C������[wall.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "wall.h"
#include "texture.h"
#include "useful.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL[CWall::TYPE_MAX] =		// ���f���̖��O
	{
		"data\\MODEL\\Wall001.x",		// �ʏ���
		"data\\MODEL\\Wall002.x",		// �ׂ����
	};
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CWall*> CWall::m_aList[area::NUM_AREA] = {};			// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CWall::CWall() : CModel(TYPE_WALL, PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_vtxMax = NONE_D3DXVECTOR3;		// ���_�̍ő�l
	m_vtxMin = NONE_D3DXVECTOR3;		// ���_�̍ŏ��l
	m_type = TYPE_NORMAL;				// ���
	m_rottype = ROTTYPE_FRONT;			// �����̎��
	m_nFieldIdx = 0;					// �敪�̔ԍ�
}

//==============================
// �f�X�g���N�^
//==============================
CWall::~CWall()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CWall::Init(void)
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
void CWall::Uninit(void)
{
	// ������������
	m_aList[m_nFieldIdx].Pull(this);

	// �I������
	CModel::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CWall::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CWall::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CWall::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetScale(scale);				// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL[type]));	// ���f���̏��

	// ��ނ�ݒ肷��
	m_type = type;				// ���
	m_rottype = rottype;		// �����̎��

	// ���_�̐ݒ菈��
	SetVertex();

	// �敪�̔ԍ��̐ݒ菈��
	m_nFieldIdx = area::SetFieldIdx(GetPos());

	// ���X�g�ɒǉ�����
	m_aList[m_nFieldIdx].Regist(this);
}

//=======================================
// ��������
//=======================================
CWall* CWall::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CWall* pBlock = nullptr;		// �C���X�^���X�𐶐�

	if (pBlock == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �u���b�N�𐶐�����
		pBlock = new CWall;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBlock != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBlock->SetData(pos, scale, type, rottype);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �u���b�N�̃|�C���^��Ԃ�
	return pBlock;
}

//=======================================
// ���_�̍ő�l�̎擾����
//=======================================
D3DXVECTOR3 CWall::GetVtxMax(void) const
{
	// ���_�̍ő�l��Ԃ�
	return m_vtxMax;
}

//=======================================
// ���_�̍ŏ��l�̎擾����
//=======================================
D3DXVECTOR3 CWall::GetVtxMin(void) const
{
	// ���_�̍ŏ��l��Ԃ�
	return m_vtxMin;
}

//=======================================
// ��ނ̎擾����
//=======================================
CWall::TYPE CWall::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//=======================================
// �����̎�ނ̎擾����
//=======================================
CWall::ROTTYPE CWall::GetRotType(void) const
{
	// �����̎�ނ�Ԃ�
	return m_rottype;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CWall*> CWall::GetList(const int nIdx)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_aList[nIdx];
}

//=======================================
// ���_�̐ݒ菈��
//=======================================
void CWall::SetVertex(void)
{
	CXFile::SXFile filedata = GetFileData();	// X�t�@�C���f�[�^
	D3DXVECTOR3 scale = GetScale();				// �g�嗦
	D3DXVECTOR3 vtxMax = NONE_D3DXVECTOR3;		// ���_�̍ő�l
	D3DXVECTOR3 vtxMin = NONE_D3DXVECTOR3;		// ���_�̍ŏ��l

	// ���_����U�Z�o����
	vtxMax.x = filedata.vtxMax.x * scale.x;
	vtxMax.y = filedata.vtxMax.y * scale.y;
	vtxMax.z = filedata.vtxMax.z * scale.z;
	vtxMin.x = filedata.vtxMin.x * scale.x;
	vtxMin.y = filedata.vtxMin.y * scale.y;
	vtxMin.z = filedata.vtxMin.z * scale.z;

	switch (m_rottype)
	{
	case CWall::ROTTYPE_FRONT:		// �O����

		// ������ݒ肷��
		SetRot(NONE_D3DXVECTOR3);

		// �ő�l�ƍŏ��l��ݒ肷��
		m_vtxMax = vtxMax;			// �ő�l
		m_vtxMin = vtxMin;			// �ŏ��l

		break;

	case CWall::ROTTYPE_RIGHT:		// �E����

		// ������ݒ肷��
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

		// �ő�l�ƍŏ��l��ݒ肷��
		m_vtxMax = D3DXVECTOR3(-vtxMin.z, vtxMax.y, vtxMax.x);
		m_vtxMin = D3DXVECTOR3(-vtxMax.z, vtxMin.y, vtxMin.x);

		break;

	case CWall::ROTTYPE_BACK:		// ������

		// ������ݒ肷��
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

		// �ő�l�ƍŏ��l��ݒ肷��
		m_vtxMax = D3DXVECTOR3(-vtxMin.x, vtxMax.y, -vtxMin.z);
		m_vtxMin = D3DXVECTOR3(-vtxMax.x, vtxMin.y, -vtxMax.z);

		break;

	case CWall::ROTTYPE_LEFT:		// ������

		// ������ݒ肷��
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		// �ő�l�ƍŏ��l��ݒ肷��
		m_vtxMax = D3DXVECTOR3(vtxMax.z, vtxMax.y, -vtxMin.x);
		m_vtxMin = D3DXVECTOR3(vtxMin.z, vtxMin.y, -vtxMax.x);

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}