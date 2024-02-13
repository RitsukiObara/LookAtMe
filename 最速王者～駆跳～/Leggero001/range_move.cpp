//=======================================
//
// �͈͈ړ��̃��C������[range_move.cpp]
// Author�F��������
//
//=======================================
#include "range_move.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "debugproc.h"
#include "renderer.h"
#include "useful.h"

//---------------------------------------
// �}�N����`
//---------------------------------------
#define RANGEMOVE_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// �����T�C�Y
#define MOVE_NORMAL			(16.0f)									// �ʏ�̈ړ���
#define MOVE_ADJUSTMENT		(2.0f)									// �������p�̈ړ���
#define SCALING_NORMAL		(8.0f)									// �ʏ�̃T�C�Y�̊g�k��
#define SCALING_ADJUSTMENT	(1.0f)									// �������p�̃T�C�Y�̊g�k��

//==========================================
// �R���X�g���N�^
//==========================================
CRangeMove::CRangeMove() : CObject3D(CObject::TYPE_RANGEMOVE, CObject::PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_state = STATE_ADJUST;						// ���
}

//==========================================
// �f�X�g���N�^
//==========================================
CRangeMove::~CRangeMove()
{

}

//==========================================
// �S�[���̏���������
//==========================================
HRESULT CRangeMove::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_state = STATE_ADJUST;						// ���

	// ������Ԃ�
	return S_OK;
}

//========================================
// �S�[���̏I������
//========================================
void CRangeMove::Uninit(void)
{
	// �I������
	CObject3D::Uninit();
}

//========================================
// �S�[���̍X�V����
//========================================
void CRangeMove::Update(void)
{
	// �ړ�����
	Move();

	// ��Ԃ̐؂�ւ�����
	StateChange();

	switch (m_state)
	{
	case STATE_ADJUST:		// �T�C�Y�������

		// �T�C�Y(X��)�̐ݒ菈��
		SizeX();

		// �T�C�Y(Y��)�̐ݒ菈��
		SizeY();

		// �T�C�Y(�S��)�̐ݒ菈��
		Size();

		// ���_�J���[�̐ݒ菈��
		SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		break;

	case STATE_MOVE:		// �ړ����

		// �͈͈ړ�����
		RangeMove();

		// ���_�J���[�̐ݒ菈��
		SetVtxColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		break;
	}

	// ���_���̐ݒ菈��
	SetVertex();

	// �G�f�B�b�g�̑���f�o�b�O�\��
	CManager::GetDebugProc()->Print("LCTRL�L�[�F�������L�[\n1�L�[�F��ԑւ�(���F�͈͐ݒ�@�ԁF�ړ�)\n4�L�[�F�S�̊g��\n5�L�[�F�S�̏k��\n"
		"6�L�[�F�X�^�C���ύX\nW/A/S/D�L�[�F�㉺���E�ړ�\nU/J�L�[�FX���g��k��\nI/K�L�[�FY���g��k��\n");

	// �G�f�B�b�g�̏��
	CManager::GetDebugProc()->Print("�ʒu�F[%f/%f/%f]\n�T�C�Y�F[%f/%f/%f]\n��ԁF[%d](0:�T�C�Y����/1:�ړ�)\n",
		GetPos().x, GetPos().y, GetPos().z, GetSize().x, GetSize().y, GetSize().z, m_state);
}

//===========================================
// �S�[���̕`�揈��
//===========================================
void CRangeMove::Draw(void)
{
	// �`�揈��
	CObject3D::Draw();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CRangeMove::SetData(const D3DXVECTOR3& pos)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);								// �ʒu�ݒ�
	SetPosOld(pos);								// �O��̈ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �����ݒ�
	SetSize(RANGEMOVE_SIZE);					// �T�C�Y�ݒ�

	// ���_���̐ݒ菈��
	SetVertex();
}

//===========================================
// �ړ�����
//===========================================
void CRangeMove::Move(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			m_move.y = MOVE_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			m_move.y = -MOVE_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			m_move.x = -MOVE_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			m_move.x = MOVE_ADJUSTMENT;
		}
		else
		{ // ��L�ȊO

			// �ړ��ʂ�ݒ肷��
			m_move.x = 0.0f;
			m_move.y = 0.0f;
		}
	}
	else
	{ // ���R���g���[���������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			m_move.y = MOVE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			m_move.y = -MOVE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			m_move.x = -MOVE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			m_move.x = MOVE_NORMAL;
		}
		else
		{ // ��L�ȊO

			// �ړ��ʂ�ݒ肷��
			m_move.x = 0.0f;
			m_move.y = 0.0f;
		}
	}

	// �ʒu�ړ�����
	Pos();
}

//===========================================
// �ʒu�ړ�����
//===========================================
void CRangeMove::Pos(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();			// �ʒu

	// �ړ�����
	pos += m_move;

	// �ʒu��؂�̂Ă���
	pos.x = floorf(pos.x);
	pos.y = floorf(pos.y);

	// �ʒu�̐ݒ菈��
	SetPos(pos);

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//===========================================
// ��Ԃ̐؂�ւ�����
//===========================================
void CRangeMove::StateChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1�L�[���������ꍇ

		// ��Ԃ�؂�ւ���
		m_state = (STATE)((m_state + 1) % STATE_MAX);
	}
}

//===========================================
// �T�C�Y(X��)�̒�������
//===========================================
void CRangeMove::SizeX(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();			// �T�C�Y���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ��SHIFT�L�[�������Ă���ꍇ

		// ���̐�̏������s��Ȃ�
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			size.x += SCALING_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			size.x -= SCALING_ADJUSTMENT;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			size.x += SCALING_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			size.x -= SCALING_NORMAL;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetSize(size);
}

//===========================================
// �T�C�Y(Y��)�̒�������
//===========================================
void CRangeMove::SizeY(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			size.y += SCALING_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			size.y -= SCALING_ADJUSTMENT;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			size.y += SCALING_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			size.y -= SCALING_NORMAL;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetSize(size);
}

//===========================================
// �T�C�Y(�S��)�̒�������
//===========================================
void CRangeMove::Size(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			size.x += SCALING_ADJUSTMENT;
			size.y += SCALING_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			size.x -= SCALING_ADJUSTMENT;
			size.y -= SCALING_ADJUSTMENT;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			size.x += SCALING_NORMAL;
			size.y += SCALING_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_5) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			size.x -= SCALING_NORMAL;
			size.y -= SCALING_NORMAL;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetSize(size);
}

//===========================================
// �͈͂̈ړ�����
//===========================================
void CRangeMove::RangeMove(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posTarget;			// �^�[�Q�b�g�̈ʒu
	D3DXVECTOR3 VtxMax;				// ���_�̍ő�l
	D3DXVECTOR3 VtxMin;				// ���_�̍ŏ��l
	CObject* pObj = nullptr;		// ���݂̃I�u�W�F�N�g�̃|�C���^
	CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g�̃|�C���^
	CObject3D* pObj3D = nullptr;	// 3D�I�u�W�F�N�g�̃|�C���^
	CModel* pModel = nullptr;		// ���f���̃|�C���^

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// �I�u�W�F�N�g��������
		pObj = GetTop((CObject::PRIORITY)(nCntPriority));

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�� NULL ����Ȃ������

			// ���̃I�u�W�F�N�g��������
			pObjNext = pObj->GetNext();

			if (pObj->GetType() == CObject::TYPE_BGOBJECT ||
				pObj->GetType() == CObject::TYPE_BLOCK ||
				pObj->GetType() == CObject::TYPE_OBSTACLE ||
				pObj->GetType() == CObject::TYPE_ITEM ||
				pObj->GetType() == CObject::TYPE_ITEMGENERATOR)
			{ // ���̎�ނ̏ꍇ

				// ���f���̏����擾����
				pModel = dynamic_cast<CModel*>(pObj);

				if (pModel != nullptr)
				{ // ���f���̏�� NULL ����Ȃ��ꍇ

					// �e����ݒ肷��
					posTarget = pModel->GetPos();				// �^�[�Q�b�g�̈ʒu
					VtxMax = pModel->GetFileData().vtxMax;		// ���_�̍ő�l
					VtxMin = pModel->GetFileData().vtxMin;		// ���_�̍ŏ��l

					// �����蔻�菈��
					pos = Hit(posTarget, VtxMax, VtxMin);

					// �ʒu�̐ݒ菈��
					pModel->SetPos(pos);
				}
			}
			else if(pObj->GetType() == CObject::TYPE_CAMERARANGE)
			{ // ���̎�ނ̏ꍇ

				// 3D�|���S���̏����擾����
				pObj3D = dynamic_cast<CObject3D*>(pObj);

				if (pObj3D != nullptr)
				{ // 3D�|���S���̏�� NULL ����Ȃ��ꍇ

					// �e����ݒ肷��
					posTarget = pObj3D->GetPos();		// �^�[�Q�b�g�̈ʒu
					VtxMax = pObj3D->GetSize();			// ���_�̍ő�l
					VtxMin = -pObj3D->GetSize();		// ���_�̍ŏ��l

					// �����蔻�菈��
					pos = Hit(posTarget, VtxMax, VtxMin);

					// �ʒu�̐ݒ菈��
					pObj3D->SetPos(pos);
				}
			}

			// ���̃I�u�W�F�N�g��������
			pObj = pObjNext;
		}
	}
}

//===========================================
// �����蔻�菈��
//===========================================
D3DXVECTOR3 CRangeMove::Hit(D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 TargetPos = pos;		// �Ώۂ̈ʒu

	if (RectangleCollisionXY
	(
		GetPos(),		// �ʒu
		TargetPos,		// �Ώۂ̈ʒu
		GetSize(),		// �ő�l
		VtxMax,			// �Ώۂ̍ő�l
		-GetSize(),		// �ŏ��l
		VtxMin			// �Ώۂ̍ŏ��l
	) == true)
	{ // ��`�̓����蔻��ɓ��������ꍇ

		// �ړ�����
		TargetPos += m_move;
	}

	// �ʒu��Ԃ�
	return TargetPos;
}

//===========================================
// ��������
//===========================================
CRangeMove* CRangeMove::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CRangeMove* pRangeMove = nullptr;	// �t�B�[���h�̃C���X�^���X�𐶐�

	if (pRangeMove == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pRangeMove = new CRangeMove;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pRangeMove != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pRangeMove->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�S�[���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pRangeMove->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �S�[���̃|�C���^��Ԃ�
	return pRangeMove;
}