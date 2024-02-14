//============================================
//
// �����蔻��|���S���̃��C������[collpolygon.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "collpolygon.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------
// �ÓI�����o�ϐ�
//--------------------------------------------
int CCollPolygon::m_nNumAll = 0;			// �|���S���̑���

//===========================================
// �R���X�g���N�^
//===========================================
CCollPolygon::CCollPolygon() : CObject3D(TYPE_COLLPOLY, CObject::PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_vtxLeftUp = NONE_D3DXVECTOR3;		// ����̍��W
	m_vtxRightUp = NONE_D3DXVECTOR3;	// �E��̍��W
	m_vtxLeftDown = NONE_D3DXVECTOR3;	// �����̍��W
	m_vtxRightDown = NONE_D3DXVECTOR3;	// �E���̍��W
	m_nNumID = NONE_COLLIDX;			// �|���S���̔ԍ�
}

//===========================================
// �f�X�g���N�^
//===========================================
CCollPolygon::~CCollPolygon()
{

}

//===========================================
// ����������
//===========================================
HRESULT CCollPolygon::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CCollPolygon::Uninit(void)
{
	// �I��
	CObject3D::Uninit();

	// ���������Z����
	m_nNumAll--;
}

//===========================================
// �X�V����
//===========================================
void CCollPolygon::Update(void)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = GetPos();			// �e�̈ʒu

	// �O��̈ʒu�̐ݒ菈��
	SetPosOld(pos);
}

//===========================================
// �`�揈��
//===========================================
void CCollPolygon::Draw(void)
{
// �f�o�b�O�p
#ifdef _DEBUG

	// �F��ݒ肷��
	CObject3D::SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

#else

	// �F��ݒ肷��
	CObject3D::SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

#endif

	// �`�揈��
	CObject3D::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CCollPolygon::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	// ���̐ݒ菈��
	SetPos(pos);								// �ʒu
	SetPosOld(pos);								// �O��̈ʒu
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// ����
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �T�C�Y

	// �S�Ă̒l��ݒ肷��
	m_vtxLeftUp = D3DXVECTOR3(VtxMin.x, VtxMax.y, VtxMax.z);	// ����̍��W
	m_vtxRightUp = D3DXVECTOR3(VtxMax.x, VtxMax.y, VtxMin.z);	// �E��̍��W
	m_vtxLeftDown = D3DXVECTOR3(VtxMin.x, VtxMin.y, VtxMax.z);	// �����̍��W
	m_vtxRightDown = D3DXVECTOR3(VtxMax.x, VtxMin.y, VtxMin.z);	// �E���̍��W

	// ���_���̐ݒ菈��(���ߑł���)
	SetVertexHardCoding(m_vtxLeftUp, m_vtxRightUp, m_vtxLeftDown, m_vtxRightDown);

	// �|���S���̔ԍ���ݒ肷��
	m_nNumID = m_nNumAll;

	// ���������Z����
	m_nNumAll++;

// �f�o�b�O�p
#ifdef _DEBUG

	// �F��ݒ肷��
	CObject3D::SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

#else

	// �F��ݒ肷��
	CObject3D::SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

#endif
}

//===========================================
// ������W�̎擾����
//===========================================
D3DXVECTOR3 CCollPolygon::GetLeftUp(void) const
{
	// ����̍��W��Ԃ�
	return m_vtxLeftUp;
}

//===========================================
// �E����W�̎擾����
//===========================================
D3DXVECTOR3 CCollPolygon::GetRightUp(void) const
{
	// �E��̍��W��Ԃ�
	return m_vtxRightUp;
}

//===========================================
// �������W�̎擾����
//===========================================
D3DXVECTOR3 CCollPolygon::GetLeftDown(void) const
{
	// �����̍��W��Ԃ�
	return m_vtxLeftDown;
}

//===========================================
// �E�����W�̎擾����
//===========================================
D3DXVECTOR3 CCollPolygon::GetRightDown(void) const
{
	// �E���̍��W��Ԃ�
	return m_vtxRightDown;
}

//===========================================
// �|���S���̔ԍ��̎擾����
//===========================================
int CCollPolygon::GetNumID(void) const
{
	// �ԍ���Ԃ�
	return m_nNumID;
}

//===========================================
// �ʒu�ƌ����̐ݒ菈��
//===========================================
void CCollPolygon::SetPosRot(const int nIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (nIdx > NONE_COLLIDX)
	{ // �ԍ��� -1 ���߂̏ꍇ

		// ���[�J���|�C���^��錾
		CCollPolygon* pPoly = GetCollPolygon(nIdx);

		if (pPoly != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ʒu�̐ݒ菈��
			pPoly->SetPos(pos);

			// �����̐ݒ菈��
			pPoly->SetRot(rot);
		}
		else
		{ // �|�C���^�� NULL �̏ꍇ

			// ��~
			assert(false);
		}
	}
}

//===========================================
// ��������
//===========================================
int CCollPolygon::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCollPolygon* pCollPolygon = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pCollPolygon == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCollPolygon = new CCollPolygon;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// -1 ��Ԃ�
		return NONE_COLLIDX;
	}

	if (pCollPolygon != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCollPolygon->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// -1 ��Ԃ�
			return NONE_COLLIDX;
		}

		// ���̐ݒ菈��
		pCollPolygon->SetData(pos, VtxMax, VtxMin);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// -1 ��Ԃ�
		return NONE_COLLIDX;
	}

	// �����蔻��|���S���̔ԍ���Ԃ�
	return pCollPolygon->GetNumID();
}

//===========================================
// �j������
//===========================================
int CCollPolygon::Delete(const int nIdx)
{
	// ���[�J���ϐ��錾
	CCollPolygon* pColl = GetCollPolygon(nIdx);		// �����蔻��|���S���̃|�C���^

	if (pColl != nullptr)
	{ // �����蔻��|���S���� NULL ����Ȃ��ꍇ

		// �I������
		pColl->Uninit();
	}

	// �C���f�b�N�X������������
	return NONE_COLLIDX;
}

//===========================================
// �����蔻��|���S���̎擾����
//===========================================
CCollPolygon* CCollPolygon::GetCollPolygon(const int nIdx)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;			// �I�u�W�F�N�g�̃|�C���^
	CCollPolygon* pCollPoly = nullptr;	// �e�̃|�C���^

	if (nIdx != INIT_SHADOW)
	{ // �e�̃C���f�b�N�X������ꍇ

		// �I�u�W�F�N�g�̏����擾����
		pObj = GetTop(DIM_3D, PRIORITY_BG);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�̃|�C���^�� NULL ����Ȃ������

			if (pObj->GetType() == TYPE_COLLPOLY)
			{ // ��ނ������蔻��|���S���̏ꍇ

				// �e�̏����擾����
				pCollPoly = dynamic_cast<CCollPolygon*>(pObj);

				if (pCollPoly != nullptr)
				{ // �e�̃|�C���^�� NULL ����Ȃ��ꍇ

					if (pCollPoly->GetNumID() == nIdx)
					{ // �e�̔ԍ��������ƈ�v�����ꍇ

						// �e�̃|�C���^��Ԃ�
						return pCollPoly;
					}
					else
					{ // ��L�ȊO

						// ���̃I�u�W�F�N�g��ݒ肷��
						pObj = pObj->GetNext();
					}
				}
				else
				{ // �e�̃|�C���^�� NULL �̏ꍇ

					// ���̃I�u�W�F�N�g��ݒ肷��
					pObj = pObj->GetNext();
				}
			}
			else
			{ // ��L�ȊO

				// ���̃I�u�W�F�N�g��ݒ肷��
				pObj = pObj->GetNext();
			}
		}

		// NULL ��Ԃ�
		return nullptr;
	}
	else
	{ // ��L�ȊO

		// NULL ��Ԃ�
		return nullptr;
	}
}