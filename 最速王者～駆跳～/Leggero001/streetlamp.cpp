//===========================================
//
// �X��(����)�̃��C������[streetlamp.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "streetlamp.h"
#include "manager.h"
#include "game.h"
#include "Effect.h"
#include "Particle.h"
#include "player.h"
#include "motion.h"
#include "renderer.h"
#include "sound.h"
#include "useful.h"

#include "orbit.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------

//==============================
// �R���X�g���N�^
//==============================
CStreetLamp::CStreetLamp() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{

}

//==============================
// �f�X�g���N�^
//==============================
CStreetLamp::~CStreetLamp()
{

}

//==============================
// �X��(����)�̏���������
//==============================
HRESULT CStreetLamp::Init(void)
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
// �X��(����)�̏I������
//========================================
void CStreetLamp::Uninit(void)
{
	// �I������
	CObstacle::Uninit();
}

//========================================
// �X��(����)�̍X�V����
//========================================
void CStreetLamp::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �O��̈ʒu��ݒ肷��
	SetPosOld(pos);

	// �v���C���[�̏����擾����
	CPlayer* pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		if (pPlayer->GetStreetLamp() == this)
		{ // �Ԃ牺�����Ă���X���ƈ�v�����ꍇ

			// ������ݒ肷��
			rot.x = (fabsf(pPlayer->GetRot().x) - (D3DX_PI * 0.5f)) * 0.1f;
		}
		else
		{ // ��L�ȊO

			// ������ݒ肷��
			rot.x = 0.0f;
		}
	}
	else
	{ // ��L�ȊO

		// ������ݒ肷��
		rot.x = 0.0f;
	}

	// �������X�V����
	SetRot(rot);

	// �X�V����
	CObstacle::Update();
}

//=====================================
// �X��(����)�̕`�揈��
//=====================================
void CStreetLamp::Draw(void)
{
	// �v���C���[�̏����擾����
	CPlayer* pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		if (pPlayer->GetStreetLamp() == this)
		{ // �Ԃ牺�����Ă���X���ƈ�v�����ꍇ

			// �f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);					//Z�e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);						//Z�e�X�g�̗L��/�����ݒ�

			// �`�揈��
			CModel::Draw(0.8f);

			// Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z�e�X�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);				// Z�e�X�g�̗L��/�����ݒ�
		}
		else
		{ // ��L�ȊO

			// �`�揈��
			CObstacle::Draw();
		}
	}
	else
	{ // ��L�ȊO

		// �`�揈��
		CObstacle::Draw();
	}
}

//=====================================
// �v���C���[�̃L���b�`����
//=====================================
void CStreetLamp::PlayerCatch(D3DXVECTOR3* pos)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = nullptr;		// �v���C���[�̃|�C���^
	float fRotDest = 0.0f;			// �ڕW�̌���

	// �v���C���[�̏����擾����
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�� NULL ����Ȃ��ꍇ

		// �ʒu��ݒ肷��
		pos->x = GetPos().x;
		pos->y = GetPos().y - PLAYER_HEIGHT;
		pos->z = 0.0f;

		if (pPlayer->GetAction()->GetAct() != CPlayerAct::ACT_HANGING)
		{ // �s���󋵂��Ԃ牺�����Ԃ���Ȃ��ꍇ

			// �s���󋵂�ݒ肷��
			pPlayer->GetAction()->ActionChange(CPlayerAct::ACT_HANGING);

			// �Ԃ牺���胂�[�V������ݒ肷��
			pPlayer->GetMotion()->Set(CPlayer::MOTIONTYPE_HANGING);

			// �ڕW�̌�����ݒ肷��
			fRotDest = pPlayer->GetMove().x * 0.15f;

			if (fRotDest >= D3DX_PI * 0.5f)
			{ // �ڕW�̌����������𒴂��Ă����ꍇ

				// �ڕW�̌�����␳����
				fRotDest = D3DX_PI * 0.5f;
			}

			if (fRotDest <= -D3DX_PI * 0.5f)
			{ // �ڕW�̌����������𒴂��Ă����ꍇ

				// �ڕW�̌�����␳����
				fRotDest = -D3DX_PI * 0.5f;
			}

			if (fRotDest == 0.0f)
			{ // �ڕW�̌����� 0.0f �̏ꍇ

				// �ڕW�̌����ɉ��Z����
				fRotDest += 0.01f;
			}

			// �����̐��K��
			RotNormalize(&fRotDest);

			// �����̈ړ��ʂ�ݒ肷��
			pPlayer->GetAction()->SetRotDest(fRotDest);

			if (fRotDest >= 0.0f)
			{ // �ڕW�̌������̏ꍇ

				// �E�����󋵂̐ݒ菈��
				pPlayer->SetRight(true);
			}
			else
			{ // ��L�ȊO

				// �E�����󋵂̐ݒ菈��
				pPlayer->SetRight(false);
			}
		}

		// �I�u�W�F�N�g�̈ʒu�̐ݒ菈��
		pPlayer->GetAction()->SetPosObject(D3DXVECTOR3(GetPos().x, GetPos().y + 15.0f, GetPos().z));

		// �X���̏��ݒ菈��
		pPlayer->SetStreetLamp(this);

		// �O�Ղ̑S���_�̈ʒu�̐ݒ�
		pPlayer->GetOrbit()->PosSet(pPlayer->GetPos());

		// ���̐ݒ菈��
		CParticle::Create(GetPos(), CEffect::TYPE_CATCH);

		// �X���͂މ���炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_LAMPCATCH);
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CStreetLamp::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ����������
	SetFileData(CXFile::TYPE_STREETLAMP);		// �f�[�^�̐ݒ菈��

	CXFile::SXFile file = GetFileData();

	// �ő�l�𔽉f����
	file.vtxMax.x = CXFile::GetXFile(CXFile::TYPE_STREETPOLE).vtxMax.x;

	// �ŏ��l�𔽉f����
	file.vtxMin.x = CXFile::GetXFile(CXFile::TYPE_STREETPOLE).vtxMin.x;

	// �f�[�^�̐ݒ菈��
	SetFileData(file);

	// ���̐ݒ菈��
	CObstacle::SetData(pos, type);
}