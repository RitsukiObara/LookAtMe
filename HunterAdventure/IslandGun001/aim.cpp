//=======================================
//
// �G�C���̃��C������[aim.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "aim.h"
#include "texture.h"
#include "collision.h"
#include "debugproc.h"
#include "useful.h"

#include "enemy.h"
#include "balloon_spawner.h"
#include "balloon.h"

//---------------------------------------
// �������O���
//---------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Aim.png";		// �e�N�X�`��
	const float RADIUS = 80.0f;		// ���a
	const D3DXCOLOR HIT_COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// �G�ɏƏ��������Ă��鎞�̐F
	const float NONE_LENGTH = 1500.0f;					// ���ɂ��������Ă��Ȃ�����
	const float HIT_VECTOR_PERMISSION = 0.05f;			// �G�C�������킹��Ƃ��̃x�N�g���̋��e�͈�
}

//=========================
// �R���X�g���N�^
//=========================
CAim::CAim() : CBillboard(CObject::TYPE_NONE, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_posPlayer = NONE_D3DXVECTOR3;		// �v���C���[�̈ʒu
	m_fLength = NONE_LENGTH;			// ����
	m_bColl = false;					// �����蔻���
}

//=========================
// �f�X�g���N�^
//=========================
CAim::~CAim()
{

}

//=========================
// ����������
//=========================
HRESULT CAim::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CAim::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CAim::Update(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = CManager::Get()->GetCamera()->GetRot();

	// �ړI�̎��_��ݒ肷��
	pos.x = m_posPlayer.x + sinf(rot.y) * sinf(rot.x) * m_fLength;
	pos.y = m_posPlayer.y + cosf(rot.x) * m_fLength;
	pos.z = m_posPlayer.z + cosf(rot.y) * sinf(rot.x) * m_fLength;

	// �ʒu��K�p����
	SetPos(pos);

	// �����蔻�菈��
	Collision();

	// ���_���W�̐ݒ菈��
	SetVertex();
}

//=========================
// �`�揈��
//=========================
void CAim::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff(false);
}

//=========================
// ���̐ݒ菈��
//=========================
void CAim::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(0.0f);			// ����
	SetSize(D3DXVECTOR3(RADIUS, RADIUS, 0.0f));	// �T�C�Y
	SetAngle();				// �����̐ݒ菈��
	SetLength();			// �����̐ݒ菈��
	SetEnableLookY(true);	// Y����

	// �S�Ă̒l��ݒ肷��
	m_posPlayer = NONE_D3DXVECTOR3;		// �v���C���[�̈ʒu
	m_fLength = NONE_LENGTH;			// ����
	m_bColl = false;					// �����蔻���

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//=========================
// ��������
//=========================
CAim* CAim::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAim* pAim = nullptr;	// �G�C���̃C���X�^���X�𐶐�

	if (pAim == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pAim = new CAim;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pAim != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pAim->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pAim->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �G�C���̃|�C���^��Ԃ�
	return pAim;
}

//=========================
// �v���C���[�̈ʒu�̐ݒ菈��
//=========================
void CAim::SetPosPlayer(const D3DXVECTOR3& pos)
{
	// �v���C���[�̈ʒu��ݒ肷��
	m_posPlayer = pos;
}

//=========================
// �����蔻�菈��
//=========================
void CAim::Collision(void)
{
	if (EnemyCollision() == true ||
		BalloonCollision() == true)
	{ // �����ɓ��������ꍇ

		// �F��t����
		SetVtxColor(HIT_COL);
	}
	else
	{ // ��L�ȊO

		// ����������������
		m_fLength = NONE_LENGTH;

		// �F��ʏ�ɖ߂�
		SetVtxColor(NONE_D3DXCOLOR);
	}
}

//=========================
// �G�Ƃ̓����蔻��
//=========================
bool CAim::EnemyCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();					// �ʒu
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;	// �G�̈ʒu
	float fHeightEnemy = 0.0f;					// �G�̍���
	D3DXVECTOR3 vecBullet = NONE_D3DXVECTOR3;	// �e���̃x�N�g��
	D3DXVECTOR3 vecEnemy = NONE_D3DXVECTOR3;	// �v���C���[����G�Ɍ����Ẵx�N�g��
	float fLength = 0.0f;						// ����
	CListManager<CEnemy*> list = CEnemy::GetList();		// �G�̃��X�g
	CEnemy* pEnemy = nullptr;		// �擪�̓G
	CEnemy* pEnemyEnd = nullptr;	// �����̒l
	int nIdx = 0;					// �G�̔ԍ�

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pEnemy = list.GetTop();

		// �����̒l���擾����
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �G�̈ʒu���擾����
			posEnemy = pEnemy->GetPos();

			// �G�̍������擾����
			fHeightEnemy = pEnemy->GetCollSize().y;

			// �ʒu�𒆐S�ɂ���
			posEnemy.y += fHeightEnemy * 0.5f;

			// �e���̃x�N�g�����Z�o����
			vecBullet = pos - m_posPlayer;

			// �G�ւ̃x�N�g�����Z�o����
			vecEnemy = posEnemy - m_posPlayer;

			// �x�N�g���𐳋K������
			D3DXVec3Normalize(&vecBullet, &vecBullet);
			D3DXVec3Normalize(&vecEnemy, &vecEnemy);

			// ������ݒ肷��
			fLength = sqrtf((posEnemy.x - m_posPlayer.x) * (posEnemy.x - m_posPlayer.x) + (posEnemy.z - m_posPlayer.z) * (posEnemy.z - m_posPlayer.z));

			if (vecBullet.x + HIT_VECTOR_PERMISSION >= vecEnemy.x &&
				vecBullet.x - HIT_VECTOR_PERMISSION <= vecEnemy.x &&
				vecBullet.y + HIT_VECTOR_PERMISSION >= vecEnemy.y &&
				vecBullet.y - HIT_VECTOR_PERMISSION <= vecEnemy.y &&
				vecBullet.z + HIT_VECTOR_PERMISSION >= vecEnemy.z &&
				vecBullet.z - HIT_VECTOR_PERMISSION <= vecEnemy.z &&
				fLength <= NONE_LENGTH)
			{ // �G���ː���ɂ���ꍇ

				// �������Z�o����
				m_fLength = fLength;

				// true ��Ԃ�
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pEnemy = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}

//=========================
// ���D�Ƃ̓����蔻��
//=========================
bool CAim::BalloonCollision(void)
{
	// ���[�J���ϐ��錾
	CBalloon* pBalloon = nullptr;				// ���D
	D3DXVECTOR3 pos = GetPos();					// �ʒu
	D3DXVECTOR3 posBalloon = NONE_D3DXVECTOR3;	// �G�̈ʒu
	D3DXVECTOR3 vecBullet = NONE_D3DXVECTOR3;	// �e���̃x�N�g��
	D3DXVECTOR3 vecBalloon = NONE_D3DXVECTOR3;	// �v���C���[����G�Ɍ����Ẵx�N�g��
	float fLength = 0.0f;						// ����

	CListManager<CBalloonSpawner*> list = CBalloonSpawner::GetList();		// �G�̃��X�g
	CBalloonSpawner* pSpawner = nullptr;		// �擪�̓G
	CBalloonSpawner* pSpawnerEnd = nullptr;		// �����̒l
	int nIdx = 0;					// �G�̔ԍ�

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pSpawner = list.GetTop();

		// �����̒l���擾����
		pSpawnerEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// ���D�̏����擾
			pBalloon = pSpawner->GetBalloon();

			if (pBalloon != nullptr)
			{ // ���D�� NULL ����Ȃ��ꍇ

				// �G�̈ʒu���擾����
				posBalloon = pBalloon->GetPos();

				// �e���̃x�N�g�����Z�o����
				vecBullet = pos - m_posPlayer;

				// �G�ւ̃x�N�g�����Z�o����
				vecBalloon = posBalloon - m_posPlayer;

				// �x�N�g���𐳋K������
				D3DXVec3Normalize(&vecBullet, &vecBullet);
				D3DXVec3Normalize(&vecBalloon, &vecBalloon);

				// ������ݒ肷��
				fLength = sqrtf((posBalloon.x - m_posPlayer.x) * (posBalloon.x - m_posPlayer.x) + (posBalloon.z - m_posPlayer.z) * (posBalloon.z - m_posPlayer.z));

				if (vecBullet.x + HIT_VECTOR_PERMISSION >= vecBalloon.x &&
					vecBullet.x - HIT_VECTOR_PERMISSION <= vecBalloon.x &&
					vecBullet.y + HIT_VECTOR_PERMISSION >= vecBalloon.y &&
					vecBullet.y - HIT_VECTOR_PERMISSION <= vecBalloon.y &&
					vecBullet.z + HIT_VECTOR_PERMISSION >= vecBalloon.z &&
					vecBullet.z - HIT_VECTOR_PERMISSION <= vecBalloon.z &&
					fLength <= NONE_LENGTH)
				{ // �G���ː���ɂ���ꍇ

					// �������Z�o����
					m_fLength = fLength;

					// true ��Ԃ�
					return true;
				}
			}

			if (pSpawner == pSpawnerEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pSpawner = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// false ��Ԃ�
	return false;
}