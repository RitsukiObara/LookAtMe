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
#include "drum.h"
#include "sound.h"
#include "useful.h"

#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "collpolygon.h"
#include "collision.h"
#include "fraction.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------
#define DRUM_ROT_MOVE			(0.1f)			// �h�����ʂ̌����̈ړ���
#define DRUM_MOVE				(6.0f)			// �h�����ʂ̈ړ���
#define DRUM_GRAVITY			(2.5f)			// �h�����ʂ̏d��
#define DRUM_SOUND_COUNT		(50)			// �h�����ʂ̃T�E���h����J�E���g

//==============================
// �R���X�g���N�^
//==============================
CDrum::CDrum() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_fRotMove = 0.0f;							// �����̈ړ���
	m_state = STATE_NONE;						// ���
	m_nCollIdx = NONE_COLLIDX;					// �����蔻��|���S���̔ԍ�
	m_nSoundCount = 0;							// �T�E���h�̃J�E���g
}

//==============================
// �f�X�g���N�^
//==============================
CDrum::~CDrum()
{

}

//==============================
// �h�����ʂ̏���������
//==============================
HRESULT CDrum::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_fRotMove = 0.0f;							// �����̈ړ���
	m_state = STATE_NONE;						// ���
	m_nCollIdx = NONE_COLLIDX;					// �����蔻��|���S���̔ԍ�
	m_nSoundCount = 0;							// �T�E���h�̃J�E���g

	// �l��Ԃ�
	return S_OK;
}

//========================================
// �h�����ʂ̏I������
//========================================
void CDrum::Uninit(void)
{
	if (m_nCollIdx != NONE_COLLIDX)
	{ // �|���S���̃C���f�b�N�X�������l����Ȃ��ꍇ

		// �����蔻��|���S���̔j������
		m_nCollIdx = CCollPolygon::Delete(m_nCollIdx);
	}

	// �I������
	CObstacle::Uninit();
}

//========================================
// �h�����ʂ̍X�V����
//========================================
void CDrum::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	switch (m_state)
	{
	case STATE_NONE:

		break;

	case STATE_ROLL:

		if (m_nSoundCount % DRUM_SOUND_COUNT == 0)
		{ // �T�E���h�J�E���g����萔�ɂȂ����ꍇ

			// �h�����ʂ��]���鉹��炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DRUMROLL);
		}

		// �T�E���h�J�E���g�����Z����
		m_nSoundCount++;

		// �ʒu�����Z����
		pos.x += m_move.x;

		// ������ݒ肷��
		rot.z -= m_fRotMove;

		// �����̐��K��
		RotNormalize(&rot.z);

		// �d�͏���
		Gravity(&m_move.y, pos, DRUM_GRAVITY);

		// �ʒu�ƌ������X�V����
		SetPos(pos);
		SetRot(rot);

		// ��Q���ƃu���b�N�̓����蔻��
		if (collision::ObstacleBlockCollision(this) == true)
		{ // �����蔻�肪 true �̏ꍇ

			// �c�̈ړ��ʂ�0�ɂ���
			m_move.y = 0.0f;
		}

		// �ؔ��̓����蔻�菈��
		collision::ObstacleWoodBoxCollision(this);

		// �u���b�N�Ƃ̓����蔻��
		if (collision::ObstacleBlockCrush(this) == true)
		{ // �q�b�g���肪 true �̏ꍇ

			// �j�Ђ̐ݒ菈��
			CFraction::SetObstacle(GetPos(), GetFileData().m_nTexIdx[0], GetMaterial(0).MatD3D.Diffuse);

			// �����n�j�󉹂�炷
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_METALCRUSH);

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;
	}

	// �X�V����
	CObstacle::Update();

	// �ʒu�ƌ����̐ݒ菈��
	CCollPolygon::SetPosRot(m_nCollIdx, pos, D3DXVECTOR3(0.0f,0.0f,0.0f));	
}

//=====================================
// �h�����ʂ̕`�揈��
//=====================================
void CDrum::Draw(void)
{
	// �`�揈��
	CObstacle::Draw();
}

//=====================================
// �q�b�g����
//=====================================
bool CDrum::Hit(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// ��]��Ԃɂ���
	m_state = STATE_ROLL;

	// �ړ��ʂ�ݒ肷��
	m_move.x = DRUM_MOVE;

	// �����̈ړ��ʂ�ݒ肷��
	m_fRotMove = DRUM_ROT_MOVE;

	// ������ݒ肷��
	rot.x = 0.0f;

	// �ʒu��ݒ肷��
	pos.y = pos.y - GetFileData().vtxMin.y;

	// �ʒu�ƌ������X�V����
	SetPos(pos);
	SetRot(rot);

	// �����蔻��̐ݒ菈��
	SetCollision(true);

	// �A�N�V��������̐ݒ菈��
	SetAttack(false);

	// �����蔻��|���S���̔j������
	m_nCollIdx = CCollPolygon::Delete(m_nCollIdx);

	// �ʒu�ƌ������X�V����
	SetPos(pos);
	SetRot(rot);

	// �U������炷
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ATTACK);

	// true ��Ԃ�
	return true;
}

//=====================================
// �q�b�g(���ɔ�΂�)����
//=====================================
void CDrum::HitLeft(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// ��]��Ԃɂ���
	m_state = STATE_ROLL;

	// �ړ��ʂ�ݒ肷��
	m_move.x = -DRUM_MOVE;

	// �����̈ړ��ʂ�ݒ肷��
	m_fRotMove = -DRUM_ROT_MOVE;

	// ������ݒ肷��
	rot.x = 0.0f;

	// �ʒu��ݒ肷��
	pos.y = pos.y - GetFileData().vtxMin.y;

	// �ʒu�ƌ������X�V����
	SetPos(pos);
	SetRot(rot);

	// �����蔻��̐ݒ菈��
	SetCollision(true);

	// �A�N�V��������̐ݒ菈��
	SetAttack(false);

	// �����蔻��|���S���̔j������
	m_nCollIdx = CCollPolygon::Delete(m_nCollIdx);

	// �U������炷
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_ATTACK);

	// �ʒu�ƌ������X�V����
	SetPos(pos);
	SetRot(rot);
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CDrum::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ����������
	SetFileData(CXFile::TYPE_DRUM);		// �f�[�^�̐ݒ菈��

	// �S�Ă̒l������������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_fRotMove = 0.0f;							// �����̈ړ���
	m_state = STATE_NONE;						// ���

	// ���̐ݒ菈��
	CObstacle::SetData(pos, type);

	// �����̏���������
	D3DXVECTOR3 Initrot = GetRot();

	// ����������������
	Initrot.x = D3DX_PI * 0.5f;

	// �����̐ݒ菈��
	SetRot(Initrot);

	// ���[�J���ϐ��錾
	D3DXVECTOR3 VtxMax, VtxMin;

	// ���W�̍ő�l
	VtxMax = D3DXVECTOR3
	(
		GetFileData().vtxMax.x,
		-GetFileData().vtxMin.z,
		0.0f
	);

	// ���W�̍ŏ��l
	VtxMin = D3DXVECTOR3
	(
		GetFileData().vtxMin.x,
		GetFileData().vtxMax.z,
		0.0f
	);

	// �����蔻��|���S���̐�������
	m_nCollIdx = CCollPolygon::Create(GetPos(), VtxMax, VtxMin);
}