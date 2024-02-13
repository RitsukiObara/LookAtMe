//===========================================
//
// �W���̃��C������[obstacle.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "input.h"
#include "signpole.h"
#include "sound.h"
#include "useful.h"

#include "block.h"
#include "debugproc.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------
#define SIGNPOLE_TOPPLE_ADDSPEED	(0.005f)		// �|���Ԃ̎��̓|���X�s�[�h�̉��Z��

//==============================
// �R���X�g���N�^
//==============================
CSignPole::CSignPole() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_NONE;		// ���
	m_fGravity = 0.0f;			// �d��
	ZeroMemory(&m_vtx[0], sizeof(m_vtx));		// ���_���W
}

//==============================
// �f�X�g���N�^
//==============================
CSignPole::~CSignPole()
{

}

//==============================
//�W���̏���������
//==============================
HRESULT CSignPole::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_state = STATE_NONE;		// ���
	m_fGravity = 0.0f;			// �d��
	ZeroMemory(&m_vtx[0], sizeof(m_vtx));		// ���_���W

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�W���̏I������
//========================================
void CSignPole::Uninit(void)
{
	// �I������
	CObstacle::Uninit();
}

//========================================
//�W���̍X�V����
//========================================
void CSignPole::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();

	switch (m_state)
	{
	case STATE_NONE:			// �ʏ���

		break;

	case STATE_TOPPLE:			// �|����

		// �d�͂����Z����
		m_fGravity += SIGNPOLE_TOPPLE_ADDSPEED;

		// �W����|���Ă���
		rot.z -= m_fGravity;

		if (rot.z <= -D3DX_PI * 0.5f)
		{ // ���������ȏ�ɂȂ����ꍇ

			// ������␳����
			rot.z = -D3DX_PI * 0.5f;

			// �d�͂�����������
			m_fGravity = 0.0f;

			// ��~��Ԃɂ���
			m_state = STATE_STOP;

			// �W���̎~�܂鉹��炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNPOLE_STOP);
		}

		// �������X�V����
		SetRot(rot);

		if (ToppleSystem() == true)
		{ // �����蔻�肪 true �������ꍇ

			// �d�͂�����������
			m_fGravity = 0.0f;

			// ��~��Ԃɂ���
			m_state = STATE_STOP;

			// �W���̎~�܂鉹��炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNPOLE_STOP);
		}

		break;

	case STATE_STOP:			// ��~���

		break;

	default:

		// �x����
		MessageBox(NULL, "�W���̏�ԂŋK��O�̐��l�����o����܂����I", "�x���I", MB_ICONWARNING);

		break;
	}

	// �X�V����
	CObstacle::Update();
}

//=====================================
//�W���̕`�揈��
//=====================================
void CSignPole::Draw(void)
{
	// �`�揈��
	CObstacle::Draw();
}

//=====================================
// �q�b�g����
//=====================================
bool CSignPole::Hit(void)
{
	// �|���Ԃɂ���
	m_state = STATE_TOPPLE;

	// �d�͂� 0.0f �ɂ���
	m_fGravity = 0.0f;

	// �����蔻��̐ݒ菈��
	SetCollision(false);

	// �A�N�V��������̐ݒ菈��
	SetAttack(false);

	// �W�����|��鉹��炷
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNPOLE_FALL);

	// true �ɕԂ�
	return true;
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CSignPole::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ����������
	SetFileData(CXFile::TYPE_SIGNPOLE);		// �f�[�^�̐ݒ菈��

	// �S�Ă̒l������������
	m_state = STATE_NONE;		// ���
	m_fGravity = 0.0f;			// �d��

	// ���̐ݒ菈��
	CObstacle::SetData(pos, type);
}

//=====================================
// ��Ԃ̐ݒ菈��
//=====================================
CSignPole::STATE CSignPole::GetState(void)
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=====================================
// ���_�̎擾����
//=====================================
D3DXVECTOR3 CSignPole::GetVtx(const int nIdx)
{
	// ���_���W��Ԃ�
	return m_vtx[nIdx];
}

//=====================================
// �΂߂̏�Q���̓����蔻��
//=====================================
float CSignPole::ElevationCollision(const D3DXVECTOR3 pos)
{
	// ���[�J���ϐ��錾
	float fHeight = 0.0f;			// �Ώۂ̍���

	m_vtx[0] = D3DXVECTOR3			// �㉜
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMax.y)),
		GetPos().z + GetFileData().vtxMax.z
	);

	m_vtx[1] = D3DXVECTOR3			// ���O
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMax.y)),
		GetPos().z + GetFileData().vtxMin.z
	);

	m_vtx[2] = D3DXVECTOR3			// ����
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMin.y)),
		GetPos().z + GetFileData().vtxMax.z
	);

	m_vtx[3] = D3DXVECTOR3			// ����O
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMin.y)),
		GetPos().z + GetFileData().vtxMin.z
	);

	// �΂߂̓����蔻��
	fHeight = ObliqueCollision(m_vtx, pos);

	// ������Ԃ�
	return fHeight;
}

//=======================================
// �΂߂̓����蔻��
//=======================================
float CSignPole::ObliqueCollision(const D3DXVECTOR3* Vtx,const D3DXVECTOR3& pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 nor, vec1, vec2;	// �ʒu
	float fHeight = 0.0f;			// �Ώۂ̍���

	if (LineOuterProductXZ(Vtx[0], Vtx[1], pos) >= 0 &&
		LineOuterProductXZ(Vtx[1], Vtx[2], pos) >= 0 &&
		LineOuterProductXZ(Vtx[2], Vtx[0], pos) >= 0)
	{ // �^�񒆂̋��E�����E���ɋ����ꍇ

		vec1 = Vtx[1] - Vtx[0];
		vec2 = Vtx[2] - Vtx[0];

		// �@�������߂�
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// �@���𐳋K������
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{ // �@����Y��0.0f�ȊO�̏ꍇ

			// ������ݒ肷��
			fHeight = -((((pos.x - Vtx[2].x) * nor.x + (pos.z - Vtx[2].z) * nor.z) - Vtx[2].y) / nor.y);
			fHeight += GetPos().y;
		}

		// ������Ԃ�
		return fHeight;
	}
	else if(LineOuterProductXZ(Vtx[1], Vtx[3], pos) >= 0 &&
		LineOuterProductXZ(Vtx[2], Vtx[1], pos) >= 0 &&
		LineOuterProductXZ(Vtx[3], Vtx[2], pos) >= 0)
	{ // �^�񒆂̋��E����荶���ɋ����ꍇ

		vec1 = Vtx[2] - Vtx[3];
		vec2 = Vtx[1] - Vtx[3];

		// �@�������߂�
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// �@���𐳋K������
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{ // �@����Y��0.0f�ȊO�̏ꍇ

			// ������ݒ肷��
			fHeight = -((((pos.x - Vtx[3].x) * nor.x + (pos.z - Vtx[3].z) * nor.z) - Vtx[3].y) / nor.y);
			fHeight += GetPos().y;
		}

		// ������Ԃ�
		return fHeight;
	}
	else
	{ // ��L�ȊO

		// ������Ԃ�
		return fHeight;
	}
}

//=======================================
// �|�ꒆ�̏�Q���̓����蔻��
//=======================================
bool CSignPole::ToppleCollision(const D3DXVECTOR3 pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 Vtx[2];				// ���_���

	Vtx[0] = D3DXVECTOR3			// ��
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.y)),
		GetPos().y + ((sinf(GetRot().z) * GetFileData().vtxMax.x) + (cosf(GetRot().z) * GetFileData().vtxMin.y)),
		0.0f
	);

	Vtx[1] = D3DXVECTOR3			// ��
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.y)),
		GetPos().y + ((sinf(GetRot().z) * GetFileData().vtxMax.x) + (cosf(GetRot().z) * GetFileData().vtxMax.y)),
		0.0f
	);

	if (Vtx[0].x <= pos.x &&
		Vtx[1].x >= pos.x &&
		Vtx[0].y <= pos.y &&
		Vtx[1].y >= pos.y)
	{ // X������Q���͈͓̔��ɂ���ꍇ

		if (LineOuterProductXY(Vtx[0], Vtx[1], pos) >= 0)
		{ // �O�ς̓����蔻��ɓ�������

		  // ���[�J���ϐ��錾
			D3DXVECTOR3 rot = GetRot();		// �������擾����
			float fRot = 0.0f;				// ����

			// �������Z�o����
			fRot = -atan2f(pos.x - Vtx[0].x, pos.y - Vtx[0].y);

			// ������ݒ肷��
			rot.z = fRot + 0.01f;

			// �������X�V����
			SetRot(rot);

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

//=======================================
// �|�ꒆ�̓����蔻��̑�{
//=======================================
bool CSignPole::ToppleSystem(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;
	CBlock* pBlock = nullptr;
	D3DXVECTOR3 pos;

	// �I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while(pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_BLOCK)
		{ // �I�u�W�F�N�g�̎�ނ��u���b�N�������ꍇ

			// ���f���̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // �u���b�N�� NULL ����Ȃ��ꍇ

				// �ʒu���Z�o����
				pos = D3DXVECTOR3
				(
					pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x,
					pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y,
					0.0f
				);

				if (ToppleCollision(pos) == true)
				{ // �����蔻�肪 true �̏ꍇ

					// true ��Ԃ�
					return true;
				}
			}
		}

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObj->GetNext();
	}

	// false ��Ԃ�
	return false;
}