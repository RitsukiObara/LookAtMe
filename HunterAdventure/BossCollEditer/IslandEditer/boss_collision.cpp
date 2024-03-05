//=========================================================
//
// �{�X�̓����蔻�菈�� [boss_collision.cpp]
// Author�F��������
//
//=========================================================
//*********************************************************
//	�C���N���[�h�t�@�C��
//*********************************************************
#include "boss_collision.h"
#include "boss.h"

#include "manager.h"
#include "objectX.h"

//===============================
// �R���X�g���N�^
//===============================
CBossCollision::CBossCollision()
{
	// �S�Ă̒l���N���A����
	memset(m_coll, 0, sizeof(m_coll));	// �����蔻��̏��
	m_nNumColl = 0;						// �����蔻��̑���
}

//===============================
// �f�X�g���N�^
//===============================
CBossCollision::~CBossCollision()
{

}

//===============================
// �I������
//===============================
void CBossCollision::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_COLLISION; nCnt++)
	{
		if (m_coll[nCnt].pModel != nullptr)
		{ // ���f���� NULL ����Ȃ��ꍇ

			// ���f���̏I������
			m_coll[nCnt].pModel->Uninit();
			m_coll[nCnt].pModel = nullptr;
		}
	}

	// ���g���폜����
	delete this;
}

//===============================
// �X�V����
//===============================
void CBossCollision::Update(void)
{

}

//===============================
// �`�揈��
//===============================
void CBossCollision::Draw(CBoss* pBoss, const int nIdx)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;

	// �ϐ���錾
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld = pBoss->GetMatrix();		// �}�g���b�N�X���擾����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, pBoss->GetScale().x, pBoss->GetScale().y, pBoss->GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->GetRot().y + D3DX_PI, pBoss->GetRot().x, pBoss->GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pBoss->GetPos().x, pBoss->GetPos().y, pBoss->GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// �}�g���b�N�X�̌v�Z����
	pBoss->GetHierarchy(nIdx)->MatrixCalc(&mtx, mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumColl; nCnt++)
	{
		m_coll[nCnt].pModel->DrawMatrix(mtx);
	}
}

//===============================
// ���̐ݒ菈��
//===============================
void CBossCollision::SetData(const int nNum)
{
	m_nNumColl = nNum;

	for (int nCnt = 0; nCnt < m_nNumColl; nCnt++)
	{
		m_coll[nCnt].pModel = CModel::Create(CObject::TYPE_NONE, CObject::PRIORITY_BG);

		m_coll[nCnt].pModel->SetFileData(CManager::Get()->GetXFile()->Regist("data\\MODEL\\CollPoint.x"));
	}
}

//===============================
// ��������
//===============================
CBossCollision* CBossCollision::Create(const int nNum)
{
	// �{�X�̃C���X�^���X
	CBossCollision* pColl = nullptr;

	if (pColl == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �{�X�𐶐�����
		pColl = new CBossCollision;

		// ���̐ݒ菈��
		pColl->SetData(nNum);
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return pColl;
	}

	// �{�X�̃|�C���^��Ԃ�
	return pColl;
}

//===============================
// �����蔻��̏��̐ݒ菈��
//===============================
void CBossCollision::SetColl(const SCollision& coll, const int nIdx)
{
	// �����蔻��̏���ݒ肷��
	m_coll[nIdx] = coll;
}

//===============================
// �����蔻��̏��̎擾����
//===============================
CBossCollision::SCollision CBossCollision::GetColl(const int nIdx) const
{
	// �����蔻��̏���Ԃ�
	return m_coll[nIdx];
}

//===============================
// �I�t�Z�b�g���W�̐ݒ菈��
//===============================
void CBossCollision::SetCollOffset(const D3DXVECTOR3& offset, const int nIdx)
{
	// �I�t�Z�b�g���W��ݒ肷��
	m_coll[nIdx].offset = offset;

	m_coll[nIdx].pModel->SetPos(offset);
}

//===============================
// �I�t�Z�b�g���W�̎擾����
//===============================
D3DXVECTOR3 CBossCollision::GetCollOffset(const int nIdx) const
{
	// �I�t�Z�b�g���W��Ԃ�
	return m_coll[nIdx].offset;
}

//===============================
// ���a�̐ݒ菈��
//===============================
void CBossCollision::SetRadius(const float fRadius, const int nIdx)
{
	// ���a��ݒ肷��
	m_coll[nIdx].fRadius = fRadius;

	float f = fRadius * 0.01f;

	m_coll[nIdx].pModel->SetScale(D3DXVECTOR3(f, f, f));
}

//===============================
// ���a�̎擾����
//===============================
float CBossCollision::GetRadius(const int nIdx) const
{
	// ���a��Ԃ�
	return m_coll[nIdx].fRadius;
}

//===============================
// ��_�󋵂̐ݒ菈��
//===============================
void CBossCollision::SetWeakness(const bool bWeak, const int nIdx)
{
	// ��_�󋵂�ݒ肷��
	m_coll[nIdx].bWeakness = bWeak;
}

//===============================
// ��_�󋵂̎擾����
//===============================
bool CBossCollision::GetWeakness(const int nIdx) const
{
	// ��_�󋵂�Ԃ�
	return m_coll[nIdx].bWeakness;
}

//===============================
// �����蔻��̑����̐ݒ菈��
//===============================
void CBossCollision::SetNumColl(const int nNum)
{
	// ������ݒ肷��
	m_nNumColl = nNum;
}

//===============================
// �����蔻��̑����̎擾����
//===============================
int CBossCollision::GetNumColl(void) const
{
	// ������Ԃ�
	return m_nNumColl;
}

//===============================
// ���f���̐ݒ菈��
//===============================
void CBossCollision::SetModel(const int nIdx)
{
	if (m_coll[nIdx].pModel == nullptr)
	{
		m_coll[nIdx].pModel = CModel::Create(CObject::TYPE_NONE, CObject::PRIORITY_BG);

		m_coll[nIdx].pModel->SetFileData(CManager::Get()->GetXFile()->Regist("data\\MODEL\\CollPoint.x"));
	}
}

//===============================
// ���f���̏�������
//===============================
void CBossCollision::DeleteModel(const int nIdx)
{
	if (m_coll[nIdx].pModel != nullptr)
	{ // ���f���� NULL ����Ȃ��ꍇ

		// �I������
		m_coll[nIdx].pModel->Uninit();
		m_coll[nIdx].pModel = nullptr;
	}
}