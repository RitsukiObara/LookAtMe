//============================================
//
// �֗��w�b�_�[[useful.h]
// Author�F��������
//
//============================================
#ifndef _USEFUL_H_				//���̃}�N����`������Ă��Ȃ�������
#define _USEFUL_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "main.h"

//--------------------------------------------
// ���O���
//--------------------------------------------
namespace useful
{
	void RotNormalize(float *pRot);													// �����̐��K��
	void Gravity(float *fGravity, float* pPos, const float fAdd);					// �d�͏���
	void RotCorrect(const float fDest, float *pRot, const float fCorrect);			// �����̕␳����(�z�[�~���O����)
	void Correct(const float fDest, float *pValue, const float fCorrect);			// �l�̕␳����
	void ColReverse(float *pData);													// 1.0f��0.0f�𔽓]���鏈��
	bool CircleCollisionXY(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2);		// �~�̓����蔻��(XY����)
	bool CircleCollisionYZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2);		// �~�̓����蔻��(YZ����)
	bool CircleCollisionXZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2);		// �~�̓����蔻��(XZ����)
	float LineOuterProductXZ(const D3DXVECTOR3& posLeft, const D3DXVECTOR3& posRight, const D3DXVECTOR3& pos);										// �O�ς̍��E���f����(XZ����)
	float LineOuterProductXY(const D3DXVECTOR3& posUp, const D3DXVECTOR3& posDown, const D3DXVECTOR3& pos);											// �O�ς̍��E���f����(XY����)
	void NormalizeVector(D3DXVECTOR3* nor, const D3DXVECTOR3& posRight, const D3DXVECTOR3& posLeft, const D3DXVECTOR3& pos);	// �@���̌v�Z(���K��)
	float InnerProduct(const D3DXVECTOR3& VecMove, const D3DXVECTOR3& VecNor);		// ���ς̔��菈��
	bool CollisionOuterProductXZ(D3DXVECTOR3* pTargetpos, const D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft);		// �O�ς̓����蔻�菈��(XZ����)
	bool CollisionOuterProductXY(D3DXVECTOR3* pTargetpos, const D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft);		// �O�ς̓����蔻�菈��(XY����)
	bool RectangleCollisionXY(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2);		// XY���W�̋�`�̓����蔻��
	bool RectangleCollisionXZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2);		// XZ���W�̋�`�̓����蔻��
	bool RectangleCollisionYZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2);		// YZ���W�̋�`�̓����蔻��
	bool CylinderCollision(D3DXVECTOR3* pTargetPos, const D3DXVECTOR3& ObjectPos, const float fObjectRadius);					// �~���̓����蔻��
	void DecimalCalculation(const int nDigit, const int nValue, int* aAnswer);		// 10�i���ւ̕ϊ�����
	bool FrameCorrect(const float fDest, float* fTarget, const float fCorrect);		// �ϓ��Ȑ��l�̕␳(float��)
	bool FrameCorrect(const int nDest, int* pTarget, const int nCorrect);			// �ϓ��Ȑ��l�̕␳(int��)
	float QuadraticCurve(const float fPos, const float fStart, const float fEnd, const float fHeight);		// �񎟋Ȑ�����
	void Parabola(const float fVelo, const float fAdd, const int nCount, float* pGravity, float* pHeight);	// ����������
	D3DXVECTOR3 VtxMinConv(const D3DXVECTOR3& size);		// �L�����N�^�[�̒��_�̍ŏ��l�ϊ�����
}

#endif