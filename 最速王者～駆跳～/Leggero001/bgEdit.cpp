//===========================================
//
//�I�u�W�F�N�g�̃��C������[EditObject.cpp]
//Author ��������
//
//===========================================
#ifdef _DEBUG	// �f�o�b�O����

#include "main.h"
#include "model.h"
#include "bgEdit.h"
#include "bgmodel.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"

// �}�N����`
#define EDITBG_SELECT_MATERIAL_ALPHA	(1.0f)		// �I�𒆂̃}�e���A���̓����x
#define EDITBG_NORMAL_MATERIAL_ALPHA	(0.5f)		// �I�����Ă��Ȃ��}�e���A���̓����x
#define EDITBG_DELETE_OBJECT_RANGE		(70.0f)		// �I�u�W�F�N�g�����͈̔�
#define EDITBG_ADJUSTMENT_MOVE			(4.0f)		// �����p�̈ړ���
#define EDITBG_MOVE						(16.0f)		// �ʏ�̈ړ���
#define EDITBG_ROT_MOVE					(0.02f)		// �����̈ړ���
#define EDITBG_SCALING					(0.02f)		// �g��k����
#define EDITBG_COL_CHANGE_CNT			(5)			// �F���ς��J�E���g
#define EDITBG_COL_CONVERSION			(0.01f)		// �}�e���A���̕ω���
#define EDITBG_UPDOWN_MOVE				(6.0f)		// �㉺�ړ��̈ړ���
#define EDITBG_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// �����p�̏㉺�ړ��̈ړ���
#define EDITBG_ADJUSTMENT_ROT_MOVE		(15)		// �����p�̌����̈ړ���
#define EDITBG_POS_Z					(500.0f)	// �w�i�I�u�W�F�N�g��Z�ʒu

//=======================================
// �R���X�g���N�^
//=======================================
CBgEdit::CBgEdit() : CModel(CObject::TYPE_EDIT, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		m_MatCopy[nCntMat];						// �}�e���A���̃R�s�[
		m_EditMaterial[nCntMat];				// �J�X�^���p�̃}�e���A�����
	}

	m_nCntMaterial = 0;;						// �}�e���A���̔ԍ�
	m_nColorCount = 0;							// �F��ς���Ƃ��̃J�E���g
	m_type = CXFile::TYPE_ENERGYITEM;			// ���
}

//=======================================
// �f�X�g���N�^
//=======================================
CBgEdit::~CBgEdit()
{

}

//=======================================
// ����������
//=======================================
HRESULT CBgEdit::Init(void)
{
	// ����������
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		m_MatCopy[nCntMat];						// �}�e���A���̃R�s�[
		m_EditMaterial[nCntMat];				// �J�X�^���p�̃}�e���A�����
	}

	m_nCntMaterial = 0;;						// �}�e���A���̔ԍ�
	m_nColorCount = 0;							// �F��ς���Ƃ��̃J�E���g
	m_type = CXFile::TYPE_ENERGYITEM;			// ���

	// ������Ԃ�
	return S_OK;
}

//=======================================
// �I������
//=======================================
void CBgEdit::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//=======================================
// �X�V����
//=======================================
void CBgEdit::Update(void)
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

	//�I�u�W�F�N�g�̐ݒ菈��
	Set();

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

	// �G�f�B�b�g�̑���f�o�b�O�\��
	CManager::GetDebugProc()->Print("LCTRL�L�[�F�������L�[\n1�L�[�F��ޑւ�\n2�L�[�F�����̃��Z�b�g\n3�L�[�F�g�嗦�̃��Z�b�g\n4�L�[�F�S�̊g��\n5�L�[�F�S�̏k��\n"
		"9�L�[�F�I�u�W�F�N�g�̍폜\n0�L�[�F�I�u�W�F�N�g�̐ݒu\nW/A/S/D�L�[�F�㉺���E�ړ�\n��SHIFT+W/S�L�[�F�c�ړ�\nQ/E�L�[�F��]\n��SHIFT+Q/E�L�[�F15�x��]\n"
		"U/J�L�[�FX���g��k��\nI/K�L�[�FY���g��k��\n��SHIFT+R/F�L�[�FR�l�ύX\n��SHIFT�L�[+T/G�L�[�FG�l�ύX\n��SHIFT�L�[+Y/H�L�[�FB�l�ύX\n"
		"��SHIFT�L�[+V/B/N�L�[�FR/G/B�l�̃��Z�b�g\nSPACE�L�[�F�}�e���A���ύX\n��ALT�L�[�F�v���C���[�̏ꏊ�Ɉړ�\n");

	// �}�e���A���̏�������
	D3DXCOLOR MatCol = m_EditMaterial[m_nCntMaterial].MatD3D.Diffuse;

	// �G�f�B�b�g�̏��
	CManager::GetDebugProc()->Print("�ʒu�F[%f/%f/%f]\n�����F[%f/%f/%f]\n�g�嗦�F[%f/%f/%f]\n�}�e���A���̐F�F[%f/%f/%f]\n",
		GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z, GetScale().x, GetScale().y, GetScale().z, MatCol.r, MatCol.g, MatCol.b);
}

//=======================================
// �`�揈��
//=======================================
void CBgEdit::Draw(void)
{
	// �G�f�B�b�g�̕`�揈��
	CModel::DrawBlock(m_EditMaterial);
}

//=======================================
// ���̐ݒ菈��
//=======================================
void CBgEdit::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile fileData)
{
	// ���[�J���ϐ��錾
	D3DXMATERIAL* pMat;		// �}�e���A���̕ϐ�

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)fileData.pBuffMat->GetBufferPointer();

	// �S�Ă̒l���N���A����
	for (int nCntMat = 0; nCntMat < (int)fileData.dwNumMat; nCntMat++)
	{
		m_MatCopy[nCntMat] = pMat[nCntMat];			// �}�e���A���̃R�s�[
		m_EditMaterial[nCntMat] = pMat[nCntMat];	// �J�X�^���p�̃}�e���A�����
	}

	m_nCntMaterial = 0;;						// �}�e���A���̔ԍ�
	m_nColorCount = 0;							// �F��ς���Ƃ��̃J�E���g
	m_type = CXFile::TYPE_ENERGYITEM;			// ���

	// ���̐ݒ�
	SetPos(D3DXVECTOR3(pos.x, pos.y, EDITBG_POS_Z));			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(rot);			// ����
	SetScale(scale);		// �g�嗦
	SetFileData(fileData);	// ���f���̃f�[�^
}

//=======================================
// ��������
//=======================================
CBgEdit* CBgEdit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile fileData)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBgEdit* pEdit = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEdit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEdit = new CBgEdit;
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
			MessageBox(NULL, "�w�i�G�f�B�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEdit->SetData(pos, rot, scale, fileData);
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
void CBgEdit::Delete(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�ւ̃|�C���^
	CBgModel* pBgModel = nullptr;		// �u���b�N�ւ̃|�C���^

	// �擪�̃I�u�W�F�N�g�̏����擾����
	pObj = GetTop(PRIORITY_BG);

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_BGOBJECT)
		{ // �I�u�W�F�N�g���w�i�I�u�W�F�N�g�������ꍇ

			// �u���b�N�̏����擾����
			pBgModel = dynamic_cast<CBgModel*>(pObj);

			if (pBgModel != nullptr)
			{ // �u���b�N�̏�� NULL ����Ȃ��ꍇ

				if (GetPos().x >= pBgModel->GetPos().x - EDITBG_DELETE_OBJECT_RANGE &&
					GetPos().x <= pBgModel->GetPos().x + EDITBG_DELETE_OBJECT_RANGE &&
					GetPos().y >= pBgModel->GetPos().y - EDITBG_DELETE_OBJECT_RANGE &&
					GetPos().y <= pBgModel->GetPos().y + EDITBG_DELETE_OBJECT_RANGE)
				{ // �͈͓��ɓ������ꍇ

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9�L�[���������ꍇ

						// �폜����
						pBgModel->Uninit();
					}
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
void CBgEdit::TypeChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1�L�[���������ꍇ

		// ���[�J���ϐ��錾
		D3DXMATERIAL* pMat;					// �}�e���A���̕ϐ�
		CXFile::SXFile fileData;			// �t�@�C���f�[�^

		// �ݒ�I�u�W�F�N�g�̎�ނ�I������
		m_type = (CXFile::TYPE)((m_type + 1) % CXFile::TYPE_MAX);

		// �t�@�C���̃f�[�^���擾����
		fileData = CXFile::GetXFile(m_type);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)fileData.pBuffMat->GetBufferPointer();

		// �S�Ă̒l���N���A����
		for (int nCntMat = 0; nCntMat < (int)fileData.dwNumMat; nCntMat++)
		{
			m_MatCopy[nCntMat] = pMat[nCntMat];			// �}�e���A���̃R�s�[
			m_EditMaterial[nCntMat] = pMat[nCntMat];	// �J�X�^���p�̃}�e���A�����
		}

		// �ݒ肷��
		SetFileData(m_type);


	}
}

//=======================================
//�ړ�����
//=======================================
void CBgEdit::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ���V�t�g�L�[��������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.y += EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.y -= EDITBG_ADJUSTMENT_MOVE;
		}
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.z += EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.z -= EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += EDITBG_ADJUSTMENT_MOVE;
		}
	}
	else
	{ // ���R���g���[���������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // W�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.z += EDITBG_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // S�L�[���������ꍇ

			// �ʒu����O�ɐi�߂�
			pos.z -= EDITBG_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // A�L�[���������ꍇ

			// �ʒu�����ɐi�߂�
			pos.x -= EDITBG_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // D�L�[���������ꍇ

			// �ʒu���E�ɐi�߂�
			pos.x += EDITBG_MOVE;
		}
	}

	// �ʒu�̐ݒ菈��
	SetPos(pos);
}

//=======================================
//��]����
//=======================================
void CBgEdit::Rotation(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();			// �������擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ��CTRL�L�[�������Ă���ꍇ

		// �����𔲂���
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Q�L�[���������ꍇ

		// ������ς���
		rot.y += EDITBG_ROT_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{ // E�L�[���������ꍇ

		// ������ς���
		rot.y -= EDITBG_ROT_MOVE;
	}

	// �����̐ݒ菈��
	SetRot(rot);
}

//=======================================
//�I�u�W�F�N�g�̐ݒ菈��
//=======================================
void CBgEdit::Set(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0�L�[���������ꍇ

		for (int nCount = 0; nCount < (int)GetFileData().dwNumMat; nCount++)
		{
			// �����x�����ɖ߂�
			m_EditMaterial[nCount].MatD3D.Ambient.a = m_MatCopy[nCount].MatD3D.Ambient.a;
			m_EditMaterial[nCount].MatD3D.Diffuse.a = m_MatCopy[nCount].MatD3D.Diffuse.a;
		}

		// �u���b�N�̐ݒ菈��
		CBgModel::Create(GetPos(), GetRot(), GetScale(), &m_EditMaterial[0], m_type);
	}
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(X��)
//=======================================
void CBgEdit::ScaleX(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 scale = GetScale();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			scale.x += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			scale.x -= EDITBG_SCALING;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // U�L�[���������ꍇ

			// X�����g�傷��
			scale.x += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // J�L�[���������ꍇ

			// X�����k������
			scale.x -= EDITBG_SCALING;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetScale(scale);
}

//=======================================
//�I�u�W�F�N�g�̊g��k������(Y��)
//=======================================
void CBgEdit::ScaleY(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 scale = GetScale();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			scale.y -= EDITBG_SCALING;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // I�L�[���������ꍇ

			// Y�����g�傷��
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // K�L�[���������ꍇ

			// Y�����k������
			scale.y -= EDITBG_SCALING;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetScale(scale);
}

//=======================================
//�I�u�W�F�N�g�̊g��k������
//=======================================
void CBgEdit::Scale(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 scale = GetScale();			// �g�嗦���擾����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // ���R���g���[���L�[�������Ă����ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			scale.x += EDITBG_SCALING;
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			scale.x -= EDITBG_SCALING;
			scale.y -= EDITBG_SCALING;
		}
	}
	else
	{ // ���R���g���[���L�[�������Ă��Ȃ��ꍇ
		if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{ // 4�L�[���������ꍇ

			// �g�傷��
			scale.x += EDITBG_SCALING;
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_5) == true)
		{ // 5�L�[���������ꍇ

			// �k������
			scale.x -= EDITBG_SCALING;
			scale.y -= EDITBG_SCALING;
		}
	}

	// �g�嗦�̐ݒ菈��
	SetScale(scale);
}

//=======================================
//�I�u�W�F�N�g�̏�񃊃Z�b�g����
//=======================================
void CBgEdit::Reset(void)
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
void CBgEdit::MaterialCustom(void)
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

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.r += EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.r > 1.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.r = 0.0f;
						}

						pMatEdit->Diffuse.r += EDITBG_COL_CONVERSION;

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

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.r -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.r < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.r = 1.0f;
						}

						pMatEdit->Diffuse.r -= EDITBG_COL_CONVERSION;

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

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//G�l�𑝂₷
						pMatEdit->Ambient.g += EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.g > 1.0f)
						{//G�l�����E�𒴂�����

							//G�l��0�ɖ߂�
							pMatEdit->Ambient.g = 0.0f;
						}

						//G�l�𑝂₷
						pMatEdit->Diffuse.g += EDITBG_COL_CONVERSION;

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

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.g -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.g < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.g = 1.0f;
						}

						//G�l�𑝂₷
						pMatEdit->Diffuse.g -= EDITBG_COL_CONVERSION;

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

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//B�l�𑝂₷
						pMatEdit->Ambient.b += EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.b > 1.0f)
						{//B�l�����E�𒴂�����

							//B�l��0�ɖ߂�
							pMatEdit->Ambient.b = 0.0f;
						}

						//B�l�𑝂₷
						pMatEdit->Diffuse.b += EDITBG_COL_CONVERSION;

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

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20F���Ƃ�

						//R�l�𑝂₷
						pMatEdit->Ambient.b -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.b < 0.0f)
						{//R�l�����E�𒴂�����

							//R�l��0�ɖ߂�
							pMatEdit->Ambient.b = 1.0f;
						}

						pMatEdit->Diffuse.b -= EDITBG_COL_CONVERSION;

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
void CBgEdit::RightAngle(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// ����

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // ��CTRL�L�[�������Ă���ꍇ

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q) == true)
		{ // Q�L�[���������ꍇ

			// ������ς���
			rot.y += D3DXToRadian(EDITBG_ADJUSTMENT_ROT_MOVE);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_E) == true)
		{ // E�L�[���������ꍇ

			// ������ς���
			rot.y -= D3DXToRadian(EDITBG_ADJUSTMENT_ROT_MOVE);
		}
	}

	// �����̐ݒ菈��
	SetRot(rot);
}

#endif