//===========================================
//
// �X��(�x��)�̃��C������[streetpole.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "game.h"
#include "streetpole.h"
#include "collpolygon.h"
#include "useful.h"

#include "player.h"
#include "motion.h"
#include "orbit.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------

//==============================
// �R���X�g���N�^
//==============================
CStreetPole::CStreetPole() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{

}

//==============================
// �f�X�g���N�^
//==============================
CStreetPole::~CStreetPole()
{

}

//==============================
// �X��(�x��)�̏���������
//==============================
HRESULT CStreetPole::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
// �X��(�x��)�̏I������
//========================================
void CStreetPole::Uninit(void)
{
	// �I������
	CObstacle::Uninit();
}

//========================================
// �X��(�x��)�̍X�V����
//========================================
void CStreetPole::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	// �X�V����
	CObstacle::Update();
}

//=====================================
// �X��(�x��)�̕`�揈��
//=====================================
void CStreetPole::Draw(void)
{
	// �`�揈��
	CObstacle::Draw();
}

//=====================================
// �q�b�g����
//=====================================
bool CStreetPole::Hit(void)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = CScene::GetPlayer();			// �v���C���[�̏����擾����
	D3DXVECTOR3 pos;								// �v���C���[�̈ʒu
	float fDistance;								// �x���ƃv���C���[�Ƃ̋���

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		if (pPlayer->GetJump() == true &&
			pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_POLEDOWN &&
			pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_HANGING &&
			pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_LAMPJUMP)
		{ // �󒆂��A�|�[�������ԈȊO���A�Ԃ炳�����ԈȊO���A�X���W�����v��ԈȊO�̏ꍇ

			// �v���C���[�̈ʒu���擾����
			pos = pPlayer->GetPos();

			// �I�u�W�F�N�g�̈ʒu�̐ݒ菈��
			pPlayer->GetAction()->SetPosObject(GetPos());

			// �|�[�������Ԃɂ���
			pPlayer->GetAction()->ActionChange(CPlayerAct::ACT_POLEDOWN);

			// ���������߂�
			fDistance = GetPos().x - pos.x;

			// �|�[�����胂�[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_POLEDOWN);

			if (fDistance >= 0.0f)
			{ // �����̒l�� 0.0f �ȏ�̏ꍇ

				// �ʒu��ݒ肷��
				pos.x -= POLE_PLAYER_DIST;
			}
			else
			{ // ��L�ȊO

				// �ʒu��ݒ肷��
				pos.x += POLE_PLAYER_DIST;
			}

			// �ʒu�̐ݒ菈��
			pPlayer->SetPos(pos);
			pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// �O�Ղ̑S���_�̈ʒu�̐ݒ�
			pPlayer->GetOrbit()->PosSet(pos);

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

//=====================================
// ���̐ݒ菈��
//=====================================
void CStreetPole::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ����������
	SetFileData(CXFile::TYPE_STREETPOLE);		// �f�[�^�̐ݒ菈��

	// ���̐ݒ菈��
	CObstacle::SetData(pos, type);
}