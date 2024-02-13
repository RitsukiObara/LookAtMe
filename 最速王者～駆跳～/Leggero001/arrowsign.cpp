//===========================================
//
// ���Ŕ̃��C������[arrowsign.cpp]
// Author�F��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "manager.h"
#include "arrowsign.h"
#include "model.h"

//==============================
// �R���X�g���N�^
//==============================
CArrowSign::CArrowSign() : CObject(CObject::TYPE_ARROWSIGN, CObject::PRIORITY_BLOCK)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==============================
// �R���X�g���N�^
//==============================
CArrowSign::CArrowSign(CObject::TYPE type, CObject::PRIORITY priority) : CObject(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//==============================
// �f�X�g���N�^
//==============================
CArrowSign::~CArrowSign()
{

}

//==============================
// �R���X�g���N�^�̔�
//==============================
void CArrowSign::Box(void)
{
	// �S�Ă̒l���N���A����
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apArrow[nCnt] = nullptr;		// ���f���̏��
	}
}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CArrowSign::Init(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apArrow[nCnt] == nullptr)
		{ // ���f���̏�� NULL �������ꍇ

			// ���f���̃I�u�W�F�N�g�𐶐�
			m_apArrow[nCnt] = new CModel(TYPE_NONE, PRIORITY_BLOCK);

			if (m_apArrow[nCnt] != nullptr)
			{ // ���f���̏�� NULL ����Ȃ��ꍇ

				if (FAILED(m_apArrow[nCnt]->Init()))
				{ // �������Ɏ��s�����ꍇ

					// ���s��Ԃ�
					return E_FAIL;
				}
			}
			else
			{ // ��L�ȊO

				// ���s��Ԃ�
				return E_FAIL;
			}
		}
		else
		{ // ��L�ȊO

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CArrowSign::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �I������
		m_apArrow[nCnt]->Uninit();
		m_apArrow[nCnt] = nullptr;
	}

	// �j������
	Release();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CArrowSign::Update(void)
{

}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CArrowSign::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apArrow[nCnt] != nullptr)
		{ // ���f���̏�� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apArrow[nCnt]->Draw();
		}
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CArrowSign::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign)
{
	// ���̏��ݒ菈��
	SetDataPole(pos, rotPole);

	// ���̏��ݒ菈��
	SetDataArrow(rotSign);
}

//=======================================
// ��������
//=======================================
CArrowSign* CArrowSign::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CArrowSign* pArrow = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pArrow == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pArrow = new CArrowSign;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pArrow != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pArrow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���Ŕ̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pArrow->SetData(pos, rotPole, rotSign);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �v���C���[�̃|�C���^��Ԃ�
	return pArrow;
}

//=======================================
// ���̏��̐ݒ菈��
//=======================================
void CArrowSign::SetDataPole(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_apArrow[TYPE_POLE] != nullptr)
	{ // ���̃|�C���^�� NULL ����Ȃ��ꍇ

		// ����ݒ肷��
		m_apArrow[TYPE_POLE]->SetPos(pos);								// �ʒu
		m_apArrow[TYPE_POLE]->SetRot(rot);								// ����
		m_apArrow[TYPE_POLE]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));	// �g�嗦
		m_apArrow[TYPE_POLE]->SetFileData(CXFile::TYPE_ARROWPOLE);		// ���f���̏��
	}
}

//=======================================
// ���̏��̐ݒ菈��
//=======================================
void CArrowSign::SetDataArrow(const D3DXVECTOR3& rot)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �ʒu

	if (m_apArrow[TYPE_POLE] != nullptr &&
		m_apArrow[TYPE_ARROW] != nullptr)
	{ // ���ƒ��̃|�C���^�� NULL ����Ȃ��ꍇ

		// �ʒu��ݒ肷��
		pos.x = m_apArrow[TYPE_POLE]->GetPos().x;
		pos.y = m_apArrow[TYPE_POLE]->GetPos().y + cosf(m_apArrow[TYPE_POLE]->GetRot().z) * m_apArrow[TYPE_POLE]->GetFileData().vtxMax.y;
		pos.z = m_apArrow[TYPE_POLE]->GetPos().z + m_apArrow[TYPE_POLE]->GetFileData().vtxMin.z;

		// ����ݒ肷��
		m_apArrow[TYPE_ARROW]->SetPos(pos);				// �ʒu
		m_apArrow[TYPE_ARROW]->SetRot(rot);				// ����
		m_apArrow[TYPE_ARROW]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));	// �g�嗦
		m_apArrow[TYPE_ARROW]->SetFileData(CXFile::TYPE_ARROWSIGN);		// ���f���̏��
	}
}