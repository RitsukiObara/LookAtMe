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
	// ���g���폜����
	delete this;
}

//===============================
// ���̐ݒ菈��
//===============================
void CBossCollision::SetData(const int nNum)
{
	m_nNumColl = nNum;
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
void CBossCollision::SetEnableWeakness(const bool bWeak, const int nIdx)
{
	// ��_�󋵂�ݒ肷��
	m_coll[nIdx].bWeakness = bWeak;
}

//===============================
// ��_�󋵂̎擾����
//===============================
bool CBossCollision::IsWeakness(const int nIdx) const
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