//===========================================
//
// �G���A�̃��C������[area.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "area.h"

//===================
// �敪�̐ݒ菈��
//===================
int area::SetFieldIdx(const D3DXVECTOR3& pos)
{
	for (int nCnt = 0; nCnt < area::NUM_AREA; nCnt++)
	{
		if (pos.x >= area::AREA_RANGE[nCnt][0].x &&
			pos.x <= area::AREA_RANGE[nCnt][1].x &&
			pos.z >= area::AREA_RANGE[nCnt][0].z &&
			pos.z <= area::AREA_RANGE[nCnt][1].z)
		{ // �͈͓��ɂ������ꍇ

			// �敪�̔ԍ���Ԃ�
			return nCnt;
		}
	}

	// 0��Ԃ�(�ً}�p)
	return 0;
}

//===================
// �ԍ��̔��菈��
//===================
bool area::IndexCheck(const int nIdx)
{
	if (nIdx >= 0 &&
		nIdx < NUM_AREA)
	{ // �ԍ����͈͓��������ꍇ

		// true ��Ԃ�
		return true;
	}
	else
	{ // ��L�ȊO

		// false��Ԃ�
		return false;
	}
}