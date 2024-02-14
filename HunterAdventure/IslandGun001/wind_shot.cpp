//===========================================
//
// ���U���̃��C������[wind_shot.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "wind_shot.h"
#include "texture.h"
#include "useful.h"

#include "game.h"
#include "player.h"
#include "objectElevation.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* TORNADO_TEXTURE = "data\\TEXTURE\\Wind.png";		// �e�N�X�`��
	const float TORNADO_WIDTH = 100.0f;			// �g���l�[�h�̕�
	const float TORNADO_HEIGHT = 200.0f;		// �g���l�[�h�̍���
	const int TORNADO_VORTEX = 6;				// �g���l�[�h�̒i��
	const float TORNADO_SHIFT = 100.0f;			// �g���l�[�h�̂��炷��
	const float TORNADO_CIRCUM = 100.0f;		// �g���l�[�h�̉~��
	const CMesh::SGrid TORNADO_DIVI = { 8,1 };	// �g���l�[�h�̕�����

	const float HOMING_CORRECT = 0.03f;			// �z�[�~���O���̌����̕␳�W��
	const float HOMING_SPEED = 25.0f;			// �z�[�~���O���̑��x
	const float GRAVITY = -25.0f;				// �d��
	const float ADD_ROT = 0.05f;				// �����̉��Z��
}

// �ÓI�����o�ϐ�
CListManager<CWindShot*> CWindShot::m_list = {};				// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CWindShot::CWindShot() : CMeshTornado(CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_state = STATE_ATTACK;			// ���
	m_fRotMove = 0.0f;				// �ړ�����
	m_fAlpha = 1.0f;				// �����x

	// ��ނ�ݒ肷��
	CObject::SetType(TYPE_WINDSHOT);

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CWindShot::~CWindShot()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CWindShot::Init(void)
{
	if (FAILED(CMeshTornado::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CWindShot::Uninit(void)
{
	// �I������
	CMeshTornado::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CWindShot::Update(void)
{
	switch (m_state)
	{
	case CWindShot::STATE_ATTACK:

		// �U����ԏ���
		Attack();

		break;

	case CWindShot::STATE_DELETE:

		// �����x�����Z����
		m_fAlpha -= 0.05f;

		// ���_�J���[�̐ݒ菈��
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));

		if (m_fAlpha <= 0.0f)
		{ // �����x��0�ȉ��ɂȂ����ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �����ړ�����
	RotMove();

	// �N���n�ʂƂ̓����蔻��
	ElevationCollision();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CWindShot::Draw(void)
{
	// �`�揈��
	CMeshTornado::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CWindShot::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	CMeshTornado::SetData
	(
		pos,
		NONE_D3DXVECTOR3,
		TORNADO_WIDTH,
		TORNADO_HEIGHT,
		TORNADO_VORTEX,
		TORNADO_SHIFT,
		TORNADO_CIRCUM,
		TORNADO_DIVI
	);

	BindTexture(CManager::Get()->GetTexture()->Regist(TORNADO_TEXTURE));		// �e�N�X�`���̊��蓖�ď���
	SetLighting(true);		// ���C�e�B���O�̐ݒ菈��
	SetCulling(true);		// �J�����O�̐ݒ菈��

	// ���_���W�̐ݒ菈��
	SetVertex();

	// �S�Ă̒l��ݒ肷��
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_state = STATE_ATTACK;			// ���
	m_fRotMove = 0.0f;				// �ړ�����
	m_fAlpha = 1.0f;				// �����x
}

//=======================================
// ��������
//=======================================
CWindShot* CWindShot::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CWindShot* pWind = nullptr;		// �C���X�^���X�𐶐�

	if (pWind == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pWind = new CWindShot;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pWind != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pWind->SetData(pos);

		// ����������
		if (FAILED(pWind->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���̃|�C���^��Ԃ�
	return pWind;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CWindShot*> CWindShot::GetList(void)
{
	// ���X�g�̏���Ԃ�
	return m_list;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CWindShot::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CWindShot::STATE CWindShot::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �N���n�ʂƂ̓����蔻��
//=======================================
void CWindShot::ElevationCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	float fHeight = 0.0f;			// ����
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;	// �擪�̏���
	CElevation* pElevEnd = nullptr;	// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pElev = list.GetTop();

		// �����̒l���擾����
		pElevEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �����蔻������
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				pos.y = fHeight;
			}

			if (pElev == pElevEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pElev = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �ʒu���X�V����
	SetPos(pos);
}

//=======================================
// �����ړ�����
//=======================================
void CWindShot::RotMove(void)
{
	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ���������Z����
	rot.y += ADD_ROT;

	// �����̐��K��
	useful::RotNormalize(&rot.y);

	// ������K�p����
	SetRot(rot);
}

//=======================================
// �U����ԏ���
//=======================================
void CWindShot::Attack(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		D3DXVECTOR3 posPlayer = pPlayer->GetPos();
		D3DXVECTOR3 pos = GetPos();
		float fRotDest = atan2f(posPlayer.x - pos.x, posPlayer.z - pos.z);

		// �����̕␳����
		useful::RotCorrect(fRotDest, &m_fRotMove, HOMING_CORRECT);

		// �ړ��ʂ�ݒ肷��
		m_move.x = sinf(m_fRotMove) * HOMING_SPEED;
		m_move.y = GRAVITY;
		m_move.z = cosf(m_fRotMove) * HOMING_SPEED;

		// �ړ�����
		pos += m_move;

		// �ʒu��K�p����
		SetPos(pos);
	}
}