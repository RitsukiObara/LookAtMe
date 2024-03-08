//============================================
//
// �֗�����[useful.cpp]
// Author�F��������
//
//============================================
//********************************************
// �}�N����`
//********************************************
#include "useful.h"
#include "manager.h"

/*
* @brief �����̐��K��
* @param pRot [in/out] �����̃|�C���^
*/
void useful::RotNormalize(float *pRot)
{
	if (*pRot > D3DX_PI)
	{ // ������3.14f�𒴂��Ă����ꍇ

		// ������␳����
		*pRot -= D3DX_PI * 2;
	}
	else if (*pRot < -D3DX_PI)
	{ // ������-3.14f�𒴂��Ă����ꍇ

		// ������␳����
		*pRot += D3DX_PI * 2;
	}
}

/*
* @brief �d�͏���
* @param fGravity [in/out] �d�͂̃|�C���^
* @param pPos [in/out] �ʒu��Y�l�̃|�C���^
* @param fAdd [in] ���Z��
*/
void useful::Gravity(float* fGravity, float* pPos, const float fAdd)
{
	// �d�͂����Z����
	*fGravity -= fAdd;

	// �d�͂�������
	*pPos += *fGravity;
}

/*
* @brief �z�[�~���O����
* @param fDest [in] �ړI�l
* @param pRot [in/out] �Ώۂ̌����̃|�C���^
* @param fCorrect [in] �␳�W��
*/
void useful::RotCorrect(const float fDest, float *pRot, const float fCorrect)
{
	// ���[�J���ϐ��錾
	float fRotDiff;			// �����̍���

	// �����̍��������߂�
	fRotDiff = fDest - *pRot;

	// �����̐��K��
	RotNormalize(&fRotDiff);

	// ������␳����
	*pRot += fRotDiff * fCorrect;

	// �����̐��K��
	RotNormalize(pRot);
}

/*
* @brief �l�̕␳����
* @param fDest [in] �ړI�l
* @param pValue [in/out] �Ώۂ̃f�[�^�̃|�C���^
* @param fCorrect [in] �␳�W��
*/
void useful::Correct(const float fDest, float *pValue, const float fCorrect)
{
	// ���[�J���ϐ��錾
	float fDiff;			// �l�̍���

	// �l�̍��������߂�
	fDiff = fDest - *pValue;

	// �l��␳����
	*pValue += fDiff * fCorrect;
}

/*
* @brief 1.0f��0.0f�𔽓]���鏈��
* @param pData [in/out] �t�]������f�[�^�̃|�C���^
*/
void useful::ColReverse(float *pData)
{
	if (*pData > 1.0f)
	{ // �f�[�^�� 0.0f �ȉ��̏ꍇ

		// �f�[�^�� 1.0f �ɂ���
		*pData = 0.0f;
	}
	else if (*pData < 0.0f)
	{ // �f�[�^�� 1.0f �ȏ�̏ꍇ

		// �f�[�^�� 0.0f �ɂ���
		*pData = 1.0f;
	}
}


/*
* @brief �~�̓����蔻��(XY����)
* @param pos1 [in] �Ώ�1�̈ʒu
* @param pos2 [in] �Ώ�2�̈ʒu
* @param fRadius1 [in] �Ώ�1�̔��a
* @param fRadius2 [in] �Ώ�2�̔��a
* @return bool �~�ɓ����������ǂ���
*/
bool useful::CircleCollisionXY(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2)
{
	// ���[�J���ϐ��錾
	float fLength;

	// �~�͈̔͂��Z�o
	fLength = (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{ // �I�u�W�F�N�g�����������ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // �I�u�W�F�N�g���������Ă��Ȃ��ꍇ

		// false ��Ԃ�
		return false;
	}
}

/*
* @brief �~�̓����蔻��(XZ����)
* @param pos1 [in] �Ώ�1�̈ʒu
* @param pos2 [in] �Ώ�2�̈ʒu
* @param fRadius1 [in] �Ώ�1�̔��a
* @param fRadius2 [in] �Ώ�2�̔��a
* @return bool �~�ɓ����������ǂ���
*/
bool useful::CircleCollisionXZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const float fRadius1, const float fRadius2)
{
	// ���[�J���ϐ��錾
	float fLength;

	// �~�͈̔͂��Z�o
	fLength = (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.z - pos1.z) * (pos2.z - pos1.z);

	if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
	{ // �I�u�W�F�N�g�����������ꍇ

	  // true ��Ԃ�
		return true;
	}
	else
	{ // �I�u�W�F�N�g���������Ă��Ȃ��ꍇ

	  // false ��Ԃ�
		return false;
	}
}

/*
* @brief �O�ς̍��E���f����(XZ����)
* @param posLeft [in] ���̈ʒu
* @param posRight [in] �E�̈ʒu
* @param pos [in] �Ώۂ̈ʒu
* @return float �O�ς̌v�Z����
*/
float useful::LineOuterProductXZ(const D3DXVECTOR3& posLeft, const D3DXVECTOR3& posRight, const D3DXVECTOR3& pos)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine;	// ���E���x�N�g��
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// ���E���x�N�g�������߂�
	vecLine = posRight - posLeft;

	// ���[�ƈʒu�̃x�N�g�������߂�
	vecToPos = pos - posLeft;

	// �O�ς̌v�Z���ʂ�Ԃ�
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

/*
* @brief �O�ς̍��E���f����(XY����)
* @param posUp [in] ��̈ʒu
* @param posDown [in] ���̈ʒu
* @param pos [in] �Ώۂ̈ʒu
* @return float �O�ς̌v�Z����
*/
float useful::LineOuterProductXY(const D3DXVECTOR3& posUp, const D3DXVECTOR3& posDown, const D3DXVECTOR3& pos)
{
	// �ϐ���錾
	D3DXVECTOR3 vecLine;	// ���E���x�N�g��
	D3DXVECTOR3 vecToPos;	// ���[�ƈʒu�̃x�N�g��

	// ���E���x�N�g�������߂�
	vecLine = posDown - posUp;

	// ���[�ƈʒu�̃x�N�g�������߂�
	vecToPos = pos - posUp;

	// �O�ς̌v�Z���ʂ�Ԃ�
	return (vecLine.x * vecToPos.y) - (vecLine.y * vecToPos.x);
}

/*
* @brief �@���̌v�Z(���K��)
* @param nor [in/out] �@���̃|�C���^
* @param posRight [in] �E�̈ʒu
* @param posLeft [in] ���̈ʒu
* @param pos [in] �Ώۂ̈ʒu
*/
void useful::NormalizeVector(D3DXVECTOR3* nor, const D3DXVECTOR3& posRight, const D3DXVECTOR3& posLeft, const D3DXVECTOR3& pos)
{
	// �x�N�g����錾
	D3DXVECTOR3 vec1;
	D3DXVECTOR3 vec2;

	// �x�N�g�����v�Z����
	vec1 = posLeft - pos;
	vec2 = posRight - pos;

	// �@�����Z�o����
	nor->x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
	nor->y = (vec1.z * vec2.x) - (vec1.x * vec2.z);
	nor->z = (vec1.x * vec2.y) - (vec1.y * vec2.x);

	// �@���𐳋K������
	D3DXVec3Normalize(nor, nor);
}

/*
* @brief ���ς̔��菈��
* @param VecMove [in] �ړ��ʂ̃x�N�g��
* @param VecNor [in] �@���x�N�g��
* @return float ���ς̌���
*/
float useful::InnerProduct(const D3DXVECTOR3& VecMove, const D3DXVECTOR3& VecNor)
{
	// ���[�J���ϐ��錾
	float fDot = 0.0f;			// ����

	// ���ς̒l���Z�o����
	fDot = (VecMove.x * VecNor.x) + (VecMove.y * VecNor.y) + (VecMove.z * VecNor.z);

	// ���ς̒l��Ԃ�
	return fDot;
}

/*
* @brief �O�ς̓����蔻�菈��(XZ����)
* @param Targetpos [in/out] �Ώۂ̈ʒu
* @param TargetposOld [in] �O��̑Ώۂ̈ʒu
* @param posRight [in] �E�̈ʒu
* @param posLeft [in] ���̈ʒu
* @return bool ���E���𒴂������ǂ���
*/
bool useful::CollisionOuterProductXZ(D3DXVECTOR3* Targetpos, const D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	// �O�ς̕ϐ�
	float fRate;										// �����̕ϐ�

	// �e�̋O�Ղ𑪂�
	vecMove.x = (Targetpos->x - TargetposOld.x);
	vecMove.y = (Targetpos->y - TargetposOld.y);
	vecMove.z = (Targetpos->z - TargetposOld.z);

	// �ǂ̋��E���𑪂�
	vecLine.x = (posRight.x - posLeft.x);
	vecLine.y = (posRight.y - posLeft.y);
	vecLine.z = (posRight.z - posLeft.z);

	// �ǂ̎n�_����e�̈ʒu�̋����𑪂�
	vecTopos.x = (Targetpos->x - posLeft.x);
	vecTopos.y = (Targetpos->y - posLeft.y);
	vecTopos.z = (Targetpos->z - posLeft.z);

	// ���������߂�
	fRate = ((vecTopos.z * vecMove.x) - (vecTopos.x * vecMove.z)) / ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z));

	// ��_�����߂�
	posCross.x = vecLine.x * fRate - posLeft.x;
	posCross.y = vecLine.y * fRate - posLeft.y;
	posCross.z = vecLine.z * fRate - posLeft.z;

	if ((vecLine.z * vecTopos.x) - (vecLine.x * vecTopos.z) < 0.0f)
	{ // ���E���𒴂����ꍇ

		if (fRate >= 0.0f && fRate <= 1.0f)
		{ // ������0.0f�`1.0f�̊Ԃ�����(���E���𒴂���)�ꍇ

			// �ʒu��ݒ肷��
			*Targetpos = posCross;

			// true ��Ԃ�
			return true;
		}
		else
		{ // ��L�ȊO

			// false ��Ԃ�
			return false;
		}
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

/*
* @brief �O�ς̓����蔻�菈��(XY����)
* @param Targetpos [in/out] �Ώۂ̈ʒu
* @param TargetposOld [in] �O��̑Ώۂ̈ʒu
* @param posRight [in] �E�̈ʒu
* @param posLeft [in] ���̈ʒu
* @return bool ���E���𒴂������ǂ���
*/
bool useful::CollisionOuterProductXY(D3DXVECTOR3* Targetpos, const D3DXVECTOR3& TargetposOld, const D3DXVECTOR3 posRight, const D3DXVECTOR3 posLeft)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 vecMove, vecLine, vecTopos, posCross;	// �O�ς̕ϐ�
	float fRate;										// �����̕ϐ�

	// �e�̋O�Ղ𑪂�
	vecMove.x = (Targetpos->x - TargetposOld.x);
	vecMove.y = (Targetpos->y - TargetposOld.y);
	vecMove.z = (Targetpos->z - TargetposOld.z);

	// �ǂ̋��E���𑪂�
	vecLine.x = (posRight.x - posLeft.x);
	vecLine.y = (posRight.y - posLeft.y);
	vecLine.z = (posRight.z - posLeft.z);

	// �ǂ̎n�_����e�̈ʒu�̋����𑪂�
	vecTopos.x = (Targetpos->x - posLeft.x);
	vecTopos.y = (Targetpos->y - posLeft.y);
	vecTopos.z = (Targetpos->z - posLeft.z);

	// ���������߂�
	fRate = ((vecTopos.x * vecMove.y) - (vecTopos.y * vecMove.x)) / ((vecLine.x * vecMove.y) - (vecLine.y * vecMove.x));

	// ��_�����߂�
	posCross.x = vecLine.x * fRate - posLeft.x;
	posCross.y = vecLine.y * fRate - posLeft.y;
	posCross.z = vecLine.z * fRate - posLeft.z;

	if ((vecLine.x * vecTopos.y) - (vecLine.y * vecTopos.x) < 0.0f)
	{ // ���E���𒴂����ꍇ

		if (fRate >= 0.0f && fRate <= 1.0f)
		{ // ������0.0f�`1.0f�̊Ԃ�����(���E���𒴂���)�ꍇ

			// �ʒu��ݒ肷��
			*Targetpos = posCross;

			// true ��Ԃ�
			return true;
		}
		else
		{ // ��L�ȊO

			// false ��Ԃ�
			return false;
		}
	}
	else
	{ // ��L�ȊO

		// false ��Ԃ�
		return false;
	}
}

/*
* @brief ��`�̓����蔻��(XY����)
* @param pos1 [in] �Ώ�1�̈ʒu
* @param pos2 [in] �Ώ�2�̈ʒu
* @param VtxMax1 [in] �Ώ�1�̍ő�l
* @param VtxMax2 [in] �Ώ�2�̍ő�l
* @param VtxMin1 [in] �Ώ�1�̍ŏ��l
* @param VtxMin2 [in] �Ώ�2�̍ŏ��l
* @return bool �����蔻��ɓ����������ǂ���
*/
bool useful::RectangleCollisionXY(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2)
{
	if (pos1.x + VtxMax1.x >= pos2.x + VtxMin2.x &&
		pos1.x + VtxMin1.x <= pos2.x + VtxMax2.x &&
		pos1.y + VtxMax1.y >= pos2.y + VtxMin2.y &&
		pos1.y + VtxMin1.y <= pos2.y + VtxMax2.y)
	{ // �����蔻��ɓ������Ă����ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // �����蔻��ɓ������Ă��Ȃ��ꍇ

		// false ��Ԃ�
		return false;
	}
}

/*
* @brief ��`�̓����蔻��(XZ����)
* @param pos1 [in] �Ώ�1�̈ʒu
* @param pos2 [in] �Ώ�2�̈ʒu
* @param VtxMax1 [in] �Ώ�1�̍ő�l
* @param VtxMax2 [in] �Ώ�2�̍ő�l
* @param VtxMin1 [in] �Ώ�1�̍ŏ��l
* @param VtxMin2 [in] �Ώ�2�̍ŏ��l
* @return bool �����蔻��ɓ����������ǂ���
*/
bool useful::RectangleCollisionXZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2)
{
	if (pos1.x + VtxMax1.x >= pos2.x + VtxMin2.x &&
		pos1.x + VtxMin1.x <= pos2.x + VtxMax2.x &&
		pos1.z + VtxMax1.z >= pos2.z + VtxMin2.z &&
		pos1.z + VtxMin1.z <= pos2.z + VtxMax2.z)
	{ // �����蔻��ɓ������Ă����ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // �����蔻��ɓ������Ă��Ȃ��ꍇ

		// false ��Ԃ�
		return false;
	}
}

/*
* @brief ��`�̓����蔻��(YZ����)
* @param pos1 [in] �Ώ�1�̈ʒu
* @param pos2 [in] �Ώ�2�̈ʒu
* @param VtxMax1 [in] �Ώ�1�̍ő�l
* @param VtxMax2 [in] �Ώ�2�̍ő�l
* @param VtxMin1 [in] �Ώ�1�̍ŏ��l
* @param VtxMin2 [in] �Ώ�2�̍ŏ��l
* @return bool �����蔻��ɓ����������ǂ���
*/
bool useful::RectangleCollisionYZ(const D3DXVECTOR3& pos1, const D3DXVECTOR3& pos2, const D3DXVECTOR3& VtxMax1, const D3DXVECTOR3& VtxMax2, const D3DXVECTOR3& VtxMin1, const D3DXVECTOR3& VtxMin2)
{
	if (pos1.y + VtxMax1.y >= pos2.y + VtxMin2.y &&
		pos1.y + VtxMin1.y <= pos2.y + VtxMax2.y &&
		pos1.z + VtxMax1.z >= pos2.z + VtxMin2.z &&
		pos1.z + VtxMin1.z <= pos2.z + VtxMax2.z)
	{ // �����蔻��ɓ������Ă����ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // �����蔻��ɓ������Ă��Ȃ��ꍇ

		// false ��Ԃ�
		return false;
	}
}

/*
* @brief �~���̓����蔻��
* @param TargetPos [in/out] �Ώۂ̈ʒu
* @param ObjectPos [in] �I�u�W�F�N�g�̈ʒu
* @param fObjectRadius [in] �ΏۂƃI�u�W�F�N�g�Ԃ̋���(���a)
* @return bool �����蔻��ɓ��������ǂ���
*/
bool useful::CylinderCollision(D3DXVECTOR3* TargetPos, const D3DXVECTOR3& ObjectPos, const float fObjectRadius)
{
	// ���[�J���ϐ��錾
	float fLength = 0.0f;			// ����
	float fAngle = 0.0f;			// ����

	// ���������
	fAngle = atan2f((TargetPos->x - ObjectPos.x), (TargetPos->z - ObjectPos.z));

	// ���a�𑪂�
	fLength = sqrtf((TargetPos->x - ObjectPos.x) * (TargetPos->x - ObjectPos.x) + (TargetPos->z - ObjectPos.z) * (TargetPos->z - ObjectPos.z));

	if (fLength <= fObjectRadius)
	{ // �ΏۂƂ̋������I�u�W�F�N�g�̔��a�ȉ��̏ꍇ

		// �ʒu��ݒ肷��
		TargetPos->x = ObjectPos.x + sinf(fAngle) * fObjectRadius;
		TargetPos->z = ObjectPos.z + cosf(fAngle) * fObjectRadius;

		// true ��Ԃ�
		return true;
	}

	// false ��Ԃ�
	return false;
}

/*
* @brief 10�i���ւ̕ϊ�����
* @param nDigit [in] ����
* @param nValue [in] �l
* @param aAnswer [in/out] ���̔z��
*/
void useful::DecimalCalculation(const int nDigit, const int nValue, int* aAnswer)
{
	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		// �l���v�Z
		aAnswer[nCnt] = (nValue % (int)(pow(10, (nDigit - (nCnt)))) / (int)(pow(10, (nDigit - (nCnt + 1)))));
	}
}

/*
* @brief �ϓ��Ȑ��l�̕␳
* @param fDest [in] �ړI�l
* @param fTarget [in/out] �Ώۂ̒l
* @param fCorrect [in] �␳�̉��Z/���Z��
* @return bool �␳���I��������ǂ���
*/
bool useful::FrameCorrect(const float fDest, float *fTarget, const float fCorrect)
{
	if (*fTarget > fDest)
	{ // �T�C�Y���ڕW���߂������ꍇ

		// �T�C�Y�����Z����
		*fTarget -= fCorrect;

		if (*fTarget <= fDest)
		{ // �ڕW�̃T�C�Y�ȉ��ɂȂ����ꍇ

			// �T�C�Y��␳����
			*fTarget = fDest;

			// true ��Ԃ�
			return true;
		}
	}

	if (*fTarget < fDest)
	{ // �T�C�Y���ڕW�����������ꍇ

		// �T�C�Y�����Z����
		*fTarget += fCorrect;

		if (*fTarget >= fDest)
		{ // �ڕW�̃T�C�Y�ȏ�ɂȂ����ꍇ

			// �T�C�Y��␳����
			*fTarget = fDest;

			// true ��Ԃ�
			return true;
		}
	}

	// false ��Ԃ�
	return false;
}

/*
* @brief �񎟋Ȑ�����
* @param fPos [in] �ʒu
* @param fStart [in] �o���n�_
* @param fEnd [in] �I���n�_
* @param fHeight [in] �ō����x
* @return float ����
*/
float useful::QuadraticCurve(const float fPos, const float fStart, const float fEnd, const float fHeight)
{
	// �ϐ���錾
	float fPosY = 0.0f;		// Y���W
	float fMax = fStart + ((fEnd - fStart) / 2.0f);		// �ō����B�_��X���W
	float fMagni = -fHeight / (((fEnd - fStart) / 2.0f) * ((fEnd - fStart) / 2.0f));	// �{��

	// Y���W��ݒ肷��
	fPosY = fMagni * ((fPos - fMax) * (fPos - fMax)) + fHeight;

	// Y���W��Ԃ�
	return fPosY;
}

/*
* @brief ����������
* @param fVelo [in] ����
* @param fGravity [in] �d��
* @param nCount [in] �o�ߎ���
* @param pGravity [out] �d��
* @param pHeight [out] ����
*/
void useful::Parabola(const float fVelo, const float fAdd, const int nCount, float* pGravity, float* pHeight)
{
	// �d�͂�ݒ肷��
	*pGravity = fVelo + fAdd * nCount;

	// ������ݒ肷��
	*pHeight = (fVelo * nCount) + (0.5f * fAdd * (nCount * nCount));
}