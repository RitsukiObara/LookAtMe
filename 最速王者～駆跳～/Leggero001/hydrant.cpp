//===========================================
//
// ���ΐ��̃��C������[obstacle.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "hydrant.h"
#include "collision.h"
#include "sound.h"
#include "useful.h"

#include "manager.h"
#include "input.h"

//==============================
// �R���X�g���N�^
//==============================
CHydrant::CHydrant() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_NONE;			// ���
	m_fGravity = 0.0f;				// �d��
	m_fRotMove = 0.0f;				// �����̈ړ���
}

//==============================
// �f�X�g���N�^
//==============================
CHydrant::~CHydrant()
{

}

//==============================
//���ΐ��̏���������
//==============================
HRESULT CHydrant::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_state = STATE_NONE;			// ���
	m_fGravity = 0.0f;				// �d��
	m_fRotMove = 0.0f;				// �����̈ړ���

	// �l��Ԃ�
	return S_OK;
}

//========================================
//���ΐ��̏I������
//========================================
void CHydrant::Uninit(void)
{
	// �I������
	CObstacle::Uninit();
}

//========================================
//���ΐ��̍X�V����
//========================================
void CHydrant::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	switch (m_state)
	{
	case STATE_NONE:			// �ʏ���

		break;

	case STATE_SMASH:			// ������я��

		// X���̈ړ��ʂ����Z����
		pos.x += 20.0f;

		// Y���̈ړ��ʂ����Z����
		pos.y += 30.0f;

		// �d�͏���
		Gravity(&m_fGravity, pos, 2.5f);

		// �ʒu���X�V����
		SetPos(pos);

		// ���������Z����
		rot.z += m_fRotMove;

		// �����̐��K��
		RotNormalize(&rot.z);

		// �������X�V����
		SetRot(rot);

		// �u���b�N�̓����蔻��
		if (collision::ObstacleBlockCollision(this) == true)
		{ // �u���b�N�̓����蔻��ɓ������ꍇ

			// �j�󉹂�炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_CRUSH);

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		// �ؔ��̓����蔻�菈��
		collision::ObstacleWoodBoxCollision(this);

		if (GetPos().y <= 0.0f)
		{ // ������ 0.0f �ȉ��ɂȂ����ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;
	}

	// �X�V����
	CObstacle::Update();
}

//=====================================
//���ΐ��̕`�揈��
//=====================================
void CHydrant::Draw(void)
{
	// �`�揈��
	CObstacle::Draw();
}

//=====================================
// �q�b�g����
//=====================================
bool CHydrant::Hit(void)
{
	// ������я�Ԃɂ���
	m_state = STATE_SMASH;

	// �����̈ړ��ʂ�ݒ肷��
	m_fRotMove = ((rand() % 500) * 0.001f) + 0.1f;

	// �����蔻��̐ݒ菈��
	SetCollision(false);

	// �A�N�V��������̐ݒ菈��
	SetAttack(false);

	// �U������炷
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ATTACK);

	// true ��Ԃ�
	return true;
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CHydrant::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ����������
	SetFileData(CXFile::TYPE_HYDRANT);		// �f�[�^�̐ݒ菈��

	// �S�Ă̒l��ݒ肷��
	m_state = STATE_NONE;			// ���
	m_fGravity = 0.0f;				// �d��
	m_fRotMove = 0.0f;				// �����̈ړ���

	// ���̐ݒ菈��
	CObstacle::SetData(pos, type);
}