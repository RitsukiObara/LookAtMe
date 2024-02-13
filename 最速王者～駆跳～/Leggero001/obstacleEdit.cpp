//===========================================
//
//�I�u�W�F�N�g�̃��C������[EditObject.cpp]
//Author ��������
//
//===========================================
#ifdef _DEBUG	// �f�o�b�O����

#include "main.h"
#include "model.h"
#include "obstacleEdit.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"

#include "player.h"

// �}�N����`
#define EDITOBSTACLE_DELETE_OBJECT_RANGE	(70.0f)		// �I�u�W�F�N�g�����͈̔�
#define EDITOBSTACLE_ADJUSTMENT_MOVE		(4.0f)		// �����p�̈ړ���
#define EDITOBSTACLE_MOVE					(16.0f)		// �ʏ�̈ړ���
#define EDITOBSTACLE_ROT_MOVE				(0.02f)		// �����̈ړ���
#define EDITOBSTACLE_UPDOWN_MOVE			(6.0f)		// �㉺�ړ��̈ړ���
#define EDITOBSTACLE_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// �����p�̏㉺�ړ��̈ړ���
#define EDITOBSTACLE_STREET_LAMP_POS_Z		(70.0f)		// �X����Z���W

//=======================================
// �R���X�g���N�^
//=======================================
CObstacleEdit::CObstacleEdit() : CObstacle(CObject::TYPE_EDIT, CObject::PRIORITY_UI)
{

}

//=======================================
// �f�X�g���N�^
//=======================================
CObstacleEdit::~CObstacleEdit()
{

}

//=======================================
// ����������
//=======================================
HRESULT CObstacleEdit::Init(void)
{
	// ����������
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=======================================
// �I������
//=======================================
void CObstacleEdit::Uninit(void)
{
	// �I������
	CObstacle::Uninit();
}

//=======================================
// �X�V����
//=======================================
void CObstacleEdit::Update(void)
{
	//��ޕύX����
	TypeChange();

	//�ړ�����
	Move();

	//�I�u�W�F�N�g�̏���
	Delete();

	//�I�u�W�F�N�g�̐ݒ菈��
	Set();

	//�I�u�W�F�N�g�̏�񃊃Z�b�g����
	Reset();

	//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
	PlayerPosSet();

	// �G�f�B�b�g�̑���f�o�b�O�\��
	CManager::GetDebugProc()->Print("LCTRL�L�[�F�������L�[\n1�L�[�F��ޑւ�\n9�L�[�F�I�u�W�F�N�g�̍폜\n0�L�[�F�I�u�W�F�N�g�̐ݒu\n"
		"W/A/S/D�L�[�F�㉺���E�ړ�\n��ALT�L�[�F�v���C���[�̏ꏊ�Ɉړ�\n");

	// �G�f�B�b�g�̏��
	CManager::GetDebugProc()->Print("�ʒu�F[%f/%f/%f]\n", GetPos().x, GetPos().y, GetPos().z);
}

//=======================================
// �`�揈��
//=======================================
void CObstacleEdit::Draw(void)
{
	// �G�f�B�b�g�̕`�揈��
	CObstacle::Draw();
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CObstacleEdit::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetFileData(INIT_OBSTACLE);

	// ���̐ݒ菈��
	CObstacle::SetData(pos,TYPE::TYPE_HYDRANT);
}

//=======================================
// ��������
//=======================================
CObstacleEdit* CObstacleEdit::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CObstacleEdit* pEdit = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEdit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEdit = new CObstacleEdit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEdit != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEdit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "��Q���G�f�B�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEdit->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �G�f�B�b�g�̃|�C���^��Ԃ�
	return pEdit;
}

//=======================================
//�I�u�W�F�N�g�̏���
//=======================================
void CObstacleEdit::Delete(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CObstacle* pObstacle = nullptr;		// �u���b�N�ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while(pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_OBSTACLE)
		{ // �I�u�W�F�N�g���u���b�N�������ꍇ

			// �u���b�N�̏����擾����
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // �u���b�N�̏�� NULL ����Ȃ��ꍇ

				if (GetPos().x >= pObstacle->GetPos().x - EDITOBSTACLE_DELETE_OBJECT_RANGE &&
					GetPos().x <= pObstacle->GetPos().x + EDITOBSTACLE_DELETE_OBJECT_RANGE &&
					GetPos().y >= pObstacle->GetPos().y - EDITOBSTACLE_DELETE_OBJECT_RANGE &&
					GetPos().y <= pObstacle->GetPos().y + EDITOBSTACLE_DELETE_OBJECT_RANGE)
				{ // �͈͓��ɓ������ꍇ

					// �폜�Ώۂɂ���
					pObstacle->SetDelete(true);

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9�L�[���������ꍇ

						// �폜����
						pObstacle->Uninit();
					}
				}
				else
				{ // �͈͊O�������ꍇ

					// �폜�Ώۂ� false �ɂ���
					pObstacle->SetDelete(true);
				}
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}
}

//=======================================
//��ޕύX����
//=======================================
void CObstacleEdit::TypeChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1�L�[���������ꍇ

		// ���[�J���ϐ��錾
		CXFile::TYPE fileType;				// X�t�@�C���̎��
		CObstacle::TYPE type = GetType();	// �u���b�N�̎��
		CXFile::SXFile fileData;			// �t�@�C���f�[�^

		// �ݒ�I�u�W�F�N�g�̎�ނ�I������
		type = (CObstacle::TYPE)((type + 1) % CObstacle::TYPE_MAX);

		// X�t�@�C���̎�ނ�ݒ肷��
		fileType = (CXFile::TYPE)(type + INIT_OBSTACLE);

		// �t�@�C���̃f�[�^���擾����
		fileData = CXFile::GetXFile(fileType);

		// �ݒ肷��
		SetFileData(fileType);

		// ��ނ̐ݒ菈��
		SetType(type);
	}

	if (GetType() == TYPE_DRUM)
	{ // ��ނ��u�h�����ʁv�̏ꍇ

		// ������ݒ肷��
		SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	}
	else if (GetType() == TYPE_LAMP)
	{ // ��ނ��u�X��(����)�v�̏ꍇ

		// ������ݒ肷��
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}
	else
	{ // ��L�ȊO

		// ������ݒ肷��
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (GetType() == TYPE::TYPE_POLE ||
		GetType() == TYPE::TYPE_LAMP)
	{ // ��ނ��X���֌W�̏ꍇ

		// �ʒu��ݒ肷��
		SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, EDITOBSTACLE_STREET_LAMP_POS_Z));
	}
	else
	{ // ��L�ȊO

		// �ʒu��ݒ肷��
		SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, 0.0f));
	}
}

//=======================================
//�ړ�����
//=======================================
void CObstacleEdit::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ���V�t�g�L�[��������Ă����ꍇ

		// �����𔲂���
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.y += EDITOBSTACLE_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= EDITOBSTACLE_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= EDITOBSTACLE_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += EDITOBSTACLE_ADJUSTMENT_MOVE;
		}
	}
	else
	{ // ���R���g���[���������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.y += EDITOBSTACLE_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= EDITOBSTACLE_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= EDITOBSTACLE_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += EDITOBSTACLE_MOVE;
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//=======================================
//�I�u�W�F�N�g�̐ݒ菈��
//=======================================
void CObstacleEdit::Set(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	// �ʒu��؂�̂Ă���
	pos.x = floorf(pos.x);
	pos.y = floorf(pos.y);

	// �ʒu�̐ݒ菈��
	SetPos(pos);

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0�L�[���������ꍇ

		// �u���b�N�̐ݒ菈��
		CObstacle::Create(GetPos(), GetType());
	}
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void CObstacleEdit::Reset(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{ // 2�L�[���������ꍇ

		// �������擾����
		D3DXVECTOR3 rot = GetRot();

		// �p�x������������
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �����̐ݒ菈��
		SetRot(rot);
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
	{ // 3�L�[���������ꍇ

		// �g�嗦���擾����
		D3DXVECTOR3 scale = GetScale();

		// �g�嗦������������
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		// �g�嗦�̎擾����
		SetScale(scale);
	}
}

//=======================================
//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
//=======================================
void CObstacleEdit::PlayerPosSet(void)
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

#endif