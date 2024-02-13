//===========================================
//
//�I�u�W�F�N�g�̃��C������[EditObject.cpp]
//Author ��������
//
//===========================================
#ifdef _DEBUG	// �f�o�b�O����

#include "main.h"
#include "model.h"
#include "edit.h"
#include "input.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

#include "block.h"
#include "shadowCircle.h"
#include "camera.h"
#include "debugproc.h"
#include "objectX.h"
#include "player.h"

// �}�N����`
#define EDITOBJ_SELECT_MATERIAL_ALPHA	(1.0f)		// �I�𒆂̃}�e���A���̓����x
#define EDITOBJ_NORMAL_MATERIAL_ALPHA	(0.5f)		// �I�����Ă��Ȃ��}�e���A���̓����x
#define EDITOBJ_DELETE_OBJECT_RANGE		(70.0f)		// �I�u�W�F�N�g�����͈̔�
#define EDITOBJ_ADJUSTMENT_MOVE			(2.0f)		// �����p�̈ړ���
#define EDITOBJ_MOVE					(16.0f)		// �ʏ�̈ړ���
#define EDITOBJ_ROT_MOVE				(0.02f)		// �����̈ړ���
#define EDITOBJ_SCALING					(0.1f)		// �g��k����
#define EDITOBJ_COL_CHANGE_CNT			(5)			// �F���ς��J�E���g
#define EDITOBJ_COL_CONVERSION			(0.01f)		// �}�e���A���̕ω���
#define EDITOBJ_UPDOWN_MOVE				(6.0f)		// �㉺�ړ��̈ړ���
#define EDITOBJ_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// �����p�̏㉺�ړ��̈ړ���
#define EDITOBJ_ADJUSTMENT_ROT_MOVE		(15)		// �����p�̌����̈ړ���

//=======================================
// �R���X�g���N�^
//=======================================
CEdit::CEdit() : CBlock(CObject::TYPE_EDIT, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		m_MatCopy[nCntMat];						// �}�e���A���̃R�s�[
		m_EditMaterial[nCntMat];				// �J�X�^���p�̃}�e���A�����
	}

	m_nCntMaterial = 0;;						// �}�e���A���̔ԍ�
	m_nColorCount = 0;							// �F��ς���Ƃ��̃J�E���g
}

//=======================================
// �f�X�g���N�^
//=======================================
CEdit::~CEdit()
{

}

//=======================================
// ����������
//=======================================
HRESULT CEdit::Init(void)
{
	// ����������
	if (FAILED(CBlock::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		m_MatCopy[nCntMat];						// �}�e���A���̃R�s�[
		m_EditMaterial[nCntMat];				// �J�X�^���p�̃}�e���A�����
	}

	m_nCntMaterial = 0;;						// �}�e���A���̔ԍ�
	m_nColorCount = 0;							// �F��ς���Ƃ��̃J�E���g

	// ������Ԃ�
	return S_OK;
}

//=======================================
// �I������
//=======================================
void CEdit::Uninit(void)
{
	// �I������
	CBlock::Uninit();
}

//=======================================
// �X�V����
//=======================================
void CEdit::Update(void)
{
	//��ޕύX����
	TypeChange();

	//�ړ�����
	Move();

	//��]����
	Rotation();

	//�I�u�W�F�N�g�̒��p����
	RightAngle();

	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	if (rot.y > D3DX_PI)
	{//3.14f���傫���Ȃ����ꍇ

		//-3.14f�ɕ␳����
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y < -D3DX_PI)
	{//-3.14f��菬�����Ȃ����ꍇ

		//3.14f�ɕ␳����
		rot.y += D3DX_PI * 2;
	}

	// ������ݒ肷��
	SetRot(rot);

	//�I�u�W�F�N�g�̏���
	Delete();

	//�I�u�W�F�N�g�̊g��k������(X��)
	ScaleX();

	//�I�u�W�F�N�g�̊g��k������(Y��)
	ScaleY();

	//�I�u�W�F�N�g�̊g��k������
	Scale();

	//�I�u�W�F�N�g�̏�񃊃Z�b�g����
	Reset();

	//�}�e���A���̃G�f�B�b�g����
	MaterialCustom();

	//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
	PlayerPosSet();

	// �R����ύX����
	CliffChange();

	//�I�u�W�F�N�g�̐ݒ菈��
	Set();

	// �G�f�B�b�g�̑���f�o�b�O�\��
	CManager::GetDebugProc()->Print("LCTRL�L�[�F�������L�[\n1�L�[�F��ޑւ�\n2�L�[�F�����̃��Z�b�g\n3�L�[�F�g�嗦�̃��Z�b�g\n4�L�[�F�S�̊g��\n5�L�[�F�S�̏k��\n"
		"6�L�[�F�X�^�C���ύX\n7�L�[�F�E�R����̕ύX\n8�L�[�F���R����̕ύX\n9�L�[�F�I�u�W�F�N�g�̍폜\n0�L�[�F�I�u�W�F�N�g�̐ݒu\nW/A/S/D�L�[�F�㉺���E�ړ�\nQ/E�L�[�F��]\n�ESHIFT+Q/E�L�[�F15�x��]\n"
		"U/J�L�[�FX���g��k��\nI/K�L�[�FY���g��k��\n��SHIFT+R/F�L�[�FR�l�ύX\n��SHIFT�L�[+T/G�L�[�FG�l�ύX\n��SHIFT�L�[+Y/H�L�[�FB�l�ύX\n"
		"��SHIFT�L�[+V/B/N�L�[�FR/G/B�l�̃��Z�b�g\nSPACE�L�[�F�}�e���A���ύX\n��ALT�L�[�F�v���C���[�̏ꏊ�Ɉړ�\n");

	// �}�e���A���̏�������
	D3DXCOLOR MatCol = m_EditMaterial[m_nCntMaterial].MatD3D.Diffuse;

	// �G�f�B�b�g�̏��
	CManager::GetDebugProc()->Print("�ʒu�F[%f/%f/%f]\n�����F[%f/%f/%f]\n�g�嗦�F[%f/%f/%f]\n�}�e���A���̐F�F[%f/%f/%f]\n(0�Ffalse/1�Ftrue)���R�̔���F%d  �E�R�̔���F%d\n",
		GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z, GetScale().x, GetScale().y, GetScale().z, MatCol.r, MatCol.g, MatCol.b, GetCliffLeft(), GetCliffRight());
}

//=======================================
// �`�揈��
//=======================================
void CEdit::Draw(void)
{
	// �G�f�B�b�g�̕`�揈��
	CBlock::DrawBlock(m_EditMaterial);
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CEdit::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale)
{
	// �t�@�C���̐�������
	SetFileData(CXFile::TYPE_BLOCK_DIRT);

	// ���[�J���ϐ��錾
	D3DXMATERIAL* pMat;		// �}�e���A���̕ϐ�

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)GetFileData().pBuffMat->GetBufferPointer();

	// �S�Ă̒l���N���A����
	for (int nCntMat = 0; nCntMat < (int)GetFileData().dwNumMat; nCntMat++)
	{
		m_MatCopy[nCntMat] = pMat[nCntMat];			// �}�e���A���̃R�s�[
		m_EditMaterial[nCntMat] = pMat[nCntMat];	// �J�X�^���p�̃}�e���A�����
	}

	m_nCntMaterial = 0;;						// �}�e���A���̔ԍ�
	m_nColorCount = 0;							// �F��ς���Ƃ��̃J�E���g

	// ���̐ݒ菈��
	CBlock::SetData(pos, rot, scale, &m_EditMaterial[0], CBlock::TYPE_DIRT, false, false);
}

//=======================================
// ��������
//=======================================
CEdit* CEdit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEdit* pEdit = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEdit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEdit = new CEdit;
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
			MessageBox(NULL, "�G�f�B�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEdit->SetData(pos, rot, scale);
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
void CEdit::Delete(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBlock* pBlock = nullptr;		// �u���b�N�ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g�̏����擾����
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_BLOCK)
		{ // �I�u�W�F�N�g���u���b�N�������ꍇ

			// �u���b�N�̏����擾����
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // �u���b�N�̏�� NULL ����Ȃ��ꍇ

				if (GetPos().x >= pBlock->GetPos().x - EDITOBJ_DELETE_OBJECT_RANGE &&
					GetPos().x <= pBlock->GetPos().x + EDITOBJ_DELETE_OBJECT_RANGE &&
					GetPos().y >= pBlock->GetPos().y - EDITOBJ_DELETE_OBJECT_RANGE &&
					GetPos().y <= pBlock->GetPos().y + EDITOBJ_DELETE_OBJECT_RANGE)
				{ // �͈͓��ɓ������ꍇ

					// �폜�Ώۂɂ���
					pBlock->SetDelete(true);

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9�L�[���������ꍇ

						// �폜����
						pBlock->Uninit();
					}
				}
				else
				{ // �͈͊O�������ꍇ

					// �폜�Ώۂ� false �ɂ���
					pBlock->SetDelete(true);
				}
			}
		}

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObj->GetNext();
	}
}

//=======================================
//��ޕύX����
//=======================================
void CEdit::TypeChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1�L�[���������ꍇ

		// ���[�J���ϐ��錾
		CXFile::TYPE fileType;				// X�t�@�C���̎��
		CBlock::TYPE type = GetType();		// �u���b�N�̎��
		D3DXMATERIAL* pMat;					// �}�e���A���̕ϐ�
		CXFile::SXFile fileData;			// �t�@�C���f�[�^

		// �ݒ�I�u�W�F�N�g�̎�ނ�I������
		type = (CBlock::TYPE)((type + 1) % CBlock::TYPE_MAX);

		// X�t�@�C���̎�ނ�ݒ肷��
		fileType = (CXFile::TYPE)(type + INIT_BLOCK);

		// �t�@�C���̃f�[�^���擾����
		fileData = CXFile::GetXFile(fileType);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)fileData.pBuffMat->GetBufferPointer();

		// �S�Ă̒l���N���A����
		for (int nCntMat = 0; nCntMat < (int)fileData.dwNumMat; nCntMat++)
		{
			m_MatCopy[nCntMat] = pMat[nCntMat];			// �}�e���A���̃R�s�[
			m_EditMaterial[nCntMat] = pMat[nCntMat];	// �J�X�^���p�̃}�e���A�����
		}

		// �ݒ肷��
		SetFileData(fileType);

		// ��ނ̐ݒ菈��
		SetType(type);
	}
}

//=======================================
//�ړ�����
//=======================================
void CEdit::Move(void)
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
			pos.y += EDITOBJ_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= EDITOBJ_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= EDITOBJ_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += EDITOBJ_ADJUSTMENT_MOVE;
		}
	}
	else
	{ // ���R���g���[���������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.y += EDITOBJ_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= EDITOBJ_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= EDITOBJ_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += EDITOBJ_MOVE;
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//=======================================
//��]����
//=======================================
void CEdit::Rotation(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();			// �������擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_RSHIFT) == true)
	{ // ��CTRL�L�[�������Ă���ꍇ

		// �����𔲂���
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Q�L�[���������ꍇ

		// ������ς���
		rot.y += EDITOBJ_ROT_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{ // E�L�[���������ꍇ

		// ������ς���
		rot.y -= EDITOBJ_ROT_MOVE;
	}

	// �����̐ݒ菈��
	SetRot(rot);
}

//=======================================
//�I�u�W�F�N�g�̐ݒ菈��
//=======================================
void CEdit::Set(void)
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

		for (int nCount = 0; nCount < (int)GetFileData().dwNumMat; nCount++)
		{
			// �����x�����ɖ߂�
			m_EditMaterial[nCount].MatD3D.Ambient.a = m_MatCopy[nCount].MatD3D.Ambient.a;
			m_EditMaterial[nCount].MatD3D.Diffuse.a = m_MatCopy[nCount].MatD3D.Diffuse.a;
		}

		// �u���b�N�̐ݒ菈��
		CBlock::Create(GetPos(), GetRot(), GetScale(), &m_EditMaterial[0], GetType(), GetCliffRight(), GetCliffLeft());
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(X��)
//=======================================
void CEdit::ScaleX(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 scale = GetScale();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			scale.x += EDITOBJ_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			scale.x -= EDITOBJ_SCALING;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			scale.x += EDITOBJ_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			scale.x -= EDITOBJ_SCALING;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetScale(scale);
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void CEdit::ScaleY(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 scale = GetScale();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			scale.y += EDITOBJ_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			scale.y -= EDITOBJ_SCALING;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			scale.y += EDITOBJ_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			scale.y -= EDITOBJ_SCALING;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetScale(scale);
}

//=======================================
//�I�u�W�F�N�g�̊g��k������
//=======================================
void CEdit::Scale(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 scale = GetScale();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			scale.x += EDITOBJ_SCALING;
			scale.y += EDITOBJ_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			scale.x -= EDITOBJ_SCALING;
			scale.y -= EDITOBJ_SCALING;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ
		if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			scale.x += EDITOBJ_SCALING;
			scale.y += EDITOBJ_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_5) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			scale.x -= EDITOBJ_SCALING;
			scale.y -= EDITOBJ_SCALING;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetScale(scale);
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void CEdit::Reset(void)
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
//�}�e���A���̃G�f�B�b�g����
//=======================================
void CEdit::MaterialCustom(void)
{
	D3DMATERIAL9 *pMatEdit;		//�}�e���A���f�[�^�ւ̃|�C���^

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
	{ // SPACE�L�[���������ꍇ

		// �}�e���A����ύX����
		m_nCntMaterial = (m_nCntMaterial + 1) % (int)(GetFileData().dwNumMat);
	}

	for (int nCntMat = 0; nCntMat < (int)GetFileData().dwNumMat; nCntMat++)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMatEdit = &m_EditMaterial[nCntMat].MatD3D;

		if (nCntMat == m_nCntMaterial)
		{ // �I�𒆂̃}�e���A���������ꍇ

			// �����x�� 1.0f �ɐݒ肷��
			pMatEdit->Diffuse.a = 1.0f;

			if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
			{ // ���V�t�g�L�[���������ꍇ

				if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
				{ // R�L�[���������ꍇ

					// �J�E���g�����Z����
					m_nColorCount++;

					if (m_nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.r += EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Ambient.r > 1.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.r = 0.0f;
						}

						pMatEdit->Diffuse.r += EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Diffuse.r > 1.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Diffuse.r = 0.0f;
						}
					}
				}
				else if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
				{//F�L�[���������ꍇ

					//�J�E���g�����Z����
					m_nColorCount++;

					if (m_nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.r -= EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Ambient.r < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.r = 1.0f;
						}

						pMatEdit->Diffuse.r -= EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Diffuse.r < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Diffuse.r = 1.0f;
						}
					}
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
				{//T�L�[���������ꍇ

					//�J�E���g�����Z����
					m_nColorCount++;

					if (m_nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//G�l�𑝂₷
						pMatEdit->Ambient.g += EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Ambient.g > 1.0f)
						{//G�l�����E�𒴂�����

							//G�l��0�ɖ߂�
							pMatEdit->Ambient.g = 0.0f;
						}

						//G�l�𑝂₷
						pMatEdit->Diffuse.g += EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Diffuse.g > 1.0f)
						{//G�l�����E�𒴂�����

							//G�l��0�ɖ߂�
							pMatEdit->Diffuse.g = 0.0f;
						}
					}
				}
				else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
				{//G�L�[���������ꍇ

					//�J�E���g�����Z����
					m_nColorCount++;

					if (m_nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.g -= EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Ambient.g < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.g = 1.0f;
						}

						//G�l�𑝂₷
						pMatEdit->Diffuse.g -= EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Diffuse.g < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Diffuse.g = 1.0f;
						}
					}
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
				{//Y�L�[���������ꍇ

					//�J�E���g�����Z����
					m_nColorCount++;

					if (m_nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//B�l�𑝂₷
						pMatEdit->Ambient.b += EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Ambient.b > 1.0f)
						{//B�l�����E�𒴂�����

							//B�l��0�ɖ߂�
							pMatEdit->Ambient.b = 0.0f;
						}

						//B�l�𑝂₷
						pMatEdit->Diffuse.b += EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Diffuse.b > 1.0f)
						{//B�l�����E�𒴂�����

							//B�l��0�ɖ߂�
							pMatEdit->Diffuse.b = 0.0f;
						}
					}
				}
				else if (CManager::GetInputKeyboard()->GetPress(DIK_H) == true)
				{//H�L�[���������ꍇ

					//�J�E���g�����Z����
					m_nColorCount++;

					if (m_nColorCount % EDITOBJ_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.b -= EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Ambient.b < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.b = 1.0f;
						}

						pMatEdit->Diffuse.b -= EDITOBJ_COL_CONVERSION;

						if (pMatEdit->Diffuse.b < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Diffuse.b = 1.0f;
						}
					}
				}

				//�A�h���X������
				D3DMATERIAL9 *pMatOriginal = &m_MatCopy[nCntMat].MatD3D;

				if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
				{//V�L�[���������ꍇ

					//R�l�̃��Z�b�g
					pMatEdit->Ambient.r = pMatOriginal->Ambient.r;
					pMatEdit->Diffuse.r = pMatOriginal->Diffuse.r;
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_B) == true)
				{//B�L�[���������ꍇ

					//G�l�̃��Z�b�g
					pMatEdit->Ambient.g = pMatOriginal->Ambient.g;
					pMatEdit->Diffuse.g = pMatOriginal->Diffuse.g;
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_N) == true)
				{//N�L�[���������ꍇ

					//B�l�̃��Z�b�g
					pMatEdit->Ambient.b = pMatOriginal->Ambient.b;
					pMatEdit->Diffuse.b = pMatOriginal->Diffuse.b;
				}
			}
		}
		else
		{ // �I�����Ă���}�e���A���ȊO

			// �����x�� 0.5f �ɐݒ肷��
			pMatEdit->Diffuse.a = 0.5f;
		}
	}
}

//=======================================
//�I�u�W�F�N�g�̒��p����
//=======================================
void CEdit::RightAngle(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// ����

	if (CManager::GetInputKeyboard()->GetPress(DIK_RSHIFT) == true)
	{ // ��CTRL�L�[�������Ă���ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q) == true)
		{ // Q�L�[���������ꍇ

			// ������ς���
			rot.y += D3DXToRadian(EDITOBJ_ADJUSTMENT_ROT_MOVE);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_E) == true)
		{ // E�L�[���������ꍇ

			// ������ς���
			rot.y -= D3DXToRadian(EDITOBJ_ADJUSTMENT_ROT_MOVE);
		}
	}

	// �����̐ݒ菈��
	SetRot(rot);
}

//=======================================
//�I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
//=======================================
void CEdit::PlayerPosSet(void)
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

//=======================================
// �R�߂܂�̕ω�����
//=======================================
void CEdit::CliffChange(void)
{
	// ���[�J���ϐ��錾
	bool bRight = GetCliffRight();		// �E�R����̎擾
	bool bLeft = GetCliffLeft();		// ���R����̎擾

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_7) == true)
	{ // 7�L�[���������ꍇ

		// �E�R�̔����ς���
		bRight = bRight ? false : true;
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_8) == true)
	{ // 8�L�[���������ꍇ

		// ���R�̔����ς���
		bLeft = bLeft ? false : true;
	}

	// �R�̔����ݒ肷��
	SetCliffRight(bRight);
	SetCliffLeft(bLeft);
}

#endif