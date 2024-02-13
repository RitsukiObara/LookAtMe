//===========================================
//
// �h�����ʂ̃��C������[drum.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "woodbox.h"
#include "collision.h"
#include "useful.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------
#define WOODBOX_GRAVITY			(0.5f)			// �ؔ��̏d��

//==============================
// �R���X�g���N�^
//==============================
CWoodBox::CWoodBox() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_fGravity = 0.0f;			// �d��
}

//==============================
// �f�X�g���N�^
//==============================
CWoodBox::~CWoodBox()
{

}

//==============================
// �ؔ��̏���������
//==============================
HRESULT CWoodBox::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_fGravity = 0.0f;			// �d��

	// �l��Ԃ�
	return S_OK;
}

//========================================
// �ؔ��̏I������
//========================================
void CWoodBox::Uninit(void)
{
	// �I������
	CObstacle::Uninit();
}

//========================================
// �ؔ��̍X�V����
//========================================
void CWoodBox::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	// �d�͏���
	Gravity(&m_fGravity, pos, WOODBOX_GRAVITY);

	if (pos.y <= 0.0f)
	{ // Y���W�� 0.0f �ȉ��ɂȂ����ꍇ

		// �ʒu��␳����
		pos.y = 0.0f;

		// �d�͂�����������
		m_fGravity = 0.0f;
	}

	// �ʒu���X�V����
	SetPos(pos);

	// ��Q�����m�̓����蔻�菈��
	if (collision::ObstacleMutualCollision(this) == true)
	{ // ���n�����ꍇ

		// �d�͂�����������
		m_fGravity = 0.0f;
	}

	if (collision::ObstacleBlockCollision(this) == true)
	{  // ���n�����ꍇ

		// �d�͂�����������
		m_fGravity = 0.0f;
	}

	// �X�V����
	CObstacle::Update();
}

//=====================================
// �ؔ��̕`�揈��
//=====================================
void CWoodBox::Draw(void)
{
	// �`�揈��
	CObstacle::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CWoodBox::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ����������
	SetFileData(CXFile::TYPE_WOODBOX);		// �f�[�^�̐ݒ菈��

	// �S�Ă̒l������������
	m_fGravity = 0.0f;			// �d��

	// ���̐ݒ菈��
	CObstacle::SetData(pos, type);
}