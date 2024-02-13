//============================================
//
// �J�����͈̔͂̃��C������[camera_range.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "input.h"
#include "cameraEdit.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define MOVE_NORMAL		(16.0f)			// �ʏ�ړ���
#define MOVE_ADJUST		(1.0f)			// �������p�ړ���
#define SCALE_NORMAL	(8.0f)			// �ʏ�g�k��
#define SCALE_ADJUST	(1.0f)			// �������p�g�k��

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CCameraEdit::c_apTypeName[CCamera::TYPE_STARTSTAGE] =			// ��ނ̖��O
{
	nullptr,				// �ʏ�J����(��O)
	"������J����",			// ������J����
	"��s�ǐՃJ����",		// ��s�ǐՃJ����
	"�Y�[���A�E�g�J����",	// �Y�[���A�E�g�J����
	"�Y�[���C���J����",		// �Y�[���C���J����
};

//===========================================
// �R���X�g���N�^
//===========================================
CCameraEdit::CCameraEdit() : CCameraRange(TYPE_EDIT, CObject::PRIORITY_BG)
{
	// �R���X�g���N�^�̔�
	Box();
}

//===========================================
// �I�[�o�[���[�h�R���X�g���N�^
//===========================================
CCameraEdit::CCameraEdit(CObject::TYPE type, PRIORITY priority) : CCameraRange(type, priority)
{
	// �R���X�g���N�^�̔�
	Box();
}

//===========================================
// �f�X�g���N�^
//===========================================
CCameraEdit::~CCameraEdit()
{

}

//===========================================
// �R���X�g���N�^�̔�
//===========================================
void CCameraEdit::Box(void)
{

}

//===========================================
// ����������
//===========================================
HRESULT CCameraEdit::Init(void)
{
	if (FAILED(CCameraRange::Init()))
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
void CCameraEdit::Uninit(void)
{
	// �I��
	CCameraRange::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CCameraEdit::Update(void)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = GetPos();			// �e�̈ʒu

	// �O��̈ʒu�̐ݒ菈��
	SetPosOld(pos);

	// ��ޕύX����
	TypeChange();

	// �ړ�����
	Move();

	// �g��k������(X��)
	ScaleX();

	// �g��k������(Y��)
	ScaleY();

	// �g��k������
	Scale();

	// ��������
	Delete();

	// �v���C���[�ʒu�ړ�����
	PlayerPosSet();

	// ���_�̐ݒ菈��
	SetVertex();

	// �ݒ菈��
	Set();

	// �G�f�B�b�g�̑���f�o�b�O�\��
	CManager::GetDebugProc()->Print("LCTRL�L�[�F�������L�[\n1�L�[�F��ޑւ�\n2�L�[�F�S�̊g��\n3�L�[�F�S�̏k��\n6�L�[�F�X�^�C���ύX\n9�L�[�F�I�u�W�F�N�g�̍폜\n0�L�[�F�I�u�W�F�N�g�̐ݒu\n"
		"W/A/S/D�L�[�F�㉺���E�ړ�\nU/J�L�[�FX���g��k��\nI/K�L�[�FY���g��k��\n��ALT�L�[�F�v���C���[�̏ꏊ�Ɉړ�\n");

	// �G�f�B�b�g�̏��
	CManager::GetDebugProc()->Print("�ʒu�F[%f/%f/%f]\n�T�C�Y�F[%f/%f/%f]\n��ށF[%s]\n",
		GetPos().x, GetPos().y, GetPos().z, GetSize().x, GetSize().y, GetSize().z, c_apTypeName[GetType()]);
}

//===========================================
// �`�揈��
//===========================================
void CCameraEdit::Draw(void)
{
// �f�o�b�O�p
#ifdef _DEBUG

	// �`�揈��
	CCameraRange::Draw();

#endif

}

//===========================================
// ��������
//===========================================
CCameraEdit* CCameraEdit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CCameraEdit* pCameraRange = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pCameraRange == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pCameraRange = new CCameraEdit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pCameraRange != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pCameraRange->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�J�����͈̔͂̃G�f�B�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pCameraRange->SetData(pos, size, type);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �J�����͈̔͂̃|�C���^��Ԃ�
	return pCameraRange;
}

//===========================================
// ��ޕύX����
//===========================================
void CCameraEdit::TypeChange(void)
{
	// ���[�J���ϐ��錾
	CCamera::TYPE type;		// ���

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1�L�[���������ꍇ

		// ��ނ��擾����
		type = GetType();

		// ��ނ�ς���
		type = (CCamera::TYPE)((type + 1) % CCamera::TYPE_STARTSTAGE);

		// ��ނ̐ݒ菈��
		SetType(type);
	}

	if (GetType() == CCamera::TYPE_NONE)
	{ // ��ނ� �ʏ�J���� �̏ꍇ

		// ��ނ��擾����
		type = GetType();

		// ��ނ�ύX����
		type = (CCamera::TYPE)(type + 1);

		//�@��ނ��X�V����
		SetType(type);
	}
}

//===========================================
// �ړ�����
//===========================================
void CCameraEdit::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.y += MOVE_ADJUST;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= MOVE_ADJUST;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= MOVE_ADJUST;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += MOVE_ADJUST;
		}
	}
	else
	{ // ���R���g���[���������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.y += MOVE_NORMAL;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= MOVE_NORMAL;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= MOVE_NORMAL;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += MOVE_NORMAL;
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//===========================================
// �g��k������(X��)
//===========================================
void CCameraEdit::ScaleX(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			size.x += SCALE_ADJUST;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			size.x -= SCALE_ADJUST;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			size.x += SCALE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			size.x -= SCALE_NORMAL;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetSize(size);
}

//===========================================
// �g��k������(Y��)
//===========================================
void CCameraEdit::ScaleY(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			size.y += SCALE_ADJUST;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			size.y -= SCALE_ADJUST;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			size.y += SCALE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			size.y -= SCALE_NORMAL;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetSize(size);
}

//===========================================
// �g��k������
//===========================================
void CCameraEdit::Scale(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			size.x += SCALE_ADJUST;
			size.y += SCALE_ADJUST;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			size.x -= SCALE_ADJUST;
			size.y -= SCALE_ADJUST;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_2) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			size.x += SCALE_NORMAL;
			size.y += SCALE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			size.x -= SCALE_NORMAL;
			size.y -= SCALE_NORMAL;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetSize(size);
}

//===========================================
// �I�u�W�F�N�g�̐ݒ菈��
//===========================================
void CCameraEdit::Set(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0�L�[���������ꍇ

		// �J�����͈̔͂̐ݒ菈��
		CCameraRange::Create(GetPos(), GetSize(), GetType());
	}
}

//===========================================
// �I�u�W�F�N�g�̏���
//===========================================
void CCameraEdit::Delete(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CCameraRange* pCamera = nullptr;		// �u���b�N�ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_BG);

	while(pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_CAMERARANGE)
		{ // �I�u�W�F�N�g���J�����͈̔͂������ꍇ

			// �u���b�N�̏����擾����
			pCamera = dynamic_cast<CCameraRange*>(pObj);

			if (pCamera != nullptr)
			{ // �u���b�N�̏�� NULL ����Ȃ��ꍇ

				if (RectangleCollisionXY
				(
					GetPos(),				// �ʒu
					pCamera->GetPos(),		// �J�����͈̔͂̈ʒu
					GetSize(),				// �ő�l
					pCamera->GetSize(),		// �J�����͈̔͂̍ő�l
					-GetSize(),				// �ŏ��l
					-pCamera->GetSize()		// �J�����͈̔͂̍ŏ��l
				) == true)
				{ // ��`�̓����蔻�肪 true �������ꍇ

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9�L�[���������ꍇ

						// �폜����
						pCamera->Uninit();
					}
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}
}

//===========================================
// �I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
//===========================================
void CCameraEdit::PlayerPosSet(void)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = nullptr;	// �v���C���[�ւ̃|�C���^

	// �v���C���[�̏����擾����
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr &&
		CManager::GetInputKeyboard()->GetTrigger(DIK_LMENU) == true)
	{ // ���� ALT�L�[�������ꂽ�ꍇ

		// �v���C���[�̈ʒu�Ɉʒu���ړ�
		SetPos(pPlayer->GetPos());
	}
}