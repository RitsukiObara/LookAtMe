//=======================================
//
//�p�[�e�B�N���̃��C������[Particle.cpp]
//Author ��������
//
//=======================================
#include "Particle.h"

//---------------------------------------
// �R���X�g���N�^
//---------------------------------------
CParticle::CParticle()
{

}

//---------------------------------------
// �f�X�g���N�^
//---------------------------------------
CParticle::~CParticle()
{

}

//===============================
//�p�[�e�B�N���̏���������
//===============================
HRESULT CParticle::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//===============================
//�p�[�e�B�N���̏I������
//===============================
void CParticle::Uninit(void)
{

}

//===============================
//�p�[�e�B�N���̍X�V����
//===============================
void CParticle::Update(void)
{
	
}

//===============================
//�p�[�e�B�N���̕`�揈��
//===============================
void CParticle::Draw(void)
{
	
}

//===============================
// ��������
//===============================
void CParticle::Create(const D3DXVECTOR3& pos, const CEffect::TYPE type)
{
	// ���[�J���|�C���^�錾
	CParticle* pParticle = nullptr;		// �p�[�e�B�N��2D�̃|�C���^

	if (pParticle == nullptr)
	{ // �p�[�e�B�N���� NULL �̏ꍇ

		// ���������m�ۂ���
		pParticle = new CParticle;
	}

	switch (type)
	{
	case CEffect::TYPE_NONE:		// ����

		break;

	case CEffect::TYPE_FIRE:		// ���n

		// �����̏���������
		pParticle->InitFire(pos);

		break;

	case CEffect::TYPE_ITEMGET:		// �A�C�e���l��

		// �A�C�e���l���̏���������
		pParticle->InitItemGet(pos);

		break;

	case CEffect::TYPE_CATCH:		// �͂ݎ�

		// �͂ݎ��̚��̏���������
		pParticle->InitCatch(pos);

		break;

	case CEffect::TYPE_BIGJUMPCHARGE:		// ���W�����v�̃`���[�W

		// ���W�����v�̃`���[�W�̏���������
		pParticle->InitBigJumpCharge(pos);

		break;

	case CEffect::TYPE_LANDDUST:

		// ���n���̏���������
		pParticle->InitLandDust(pos);

		break;

	default:			// ��L�ȊO

		// �x����
		MessageBox(NULL, "�p�[�e�B�N���̎�ނŗ\��O�̐��l�����m���ꂽ�I", "�x���I", MB_ICONWARNING);

		break;
	}

	if (pParticle != nullptr)
	{ // �p�[�e�B�N���� NULL ����Ȃ��ꍇ

		// ���������J������
		delete pParticle;
		pParticle = nullptr;
	}
}

//===============================
// �����p�[�e�B�N������
//===============================
void CParticle::InitFire(const D3DXVECTOR3& pos)
{
	//[�����p�ϐ�]
	D3DXVECTOR3 ShiftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	int nLife = 0;											// �̗�
	float fAngle = 0.0f;									// ����
	float fSpeed = 0.0f;									// ���x
	float fRadius = 0.0f;									// ���a

	for (int nCntPar = 0; nCntPar < 20; nCntPar++)
	{
		// �ʒu�̐ݒ�
		ShiftPos.x = pos.x + (float)((rand() % 101 - 50) / 10.0f);
		ShiftPos.y = pos.y + (float)((rand() % 101 - 50) / 10.0f);
		ShiftPos.z = 0.0f;

		//�p�x��ݒ肷��
		fAngle = (float)(rand() % 629 - 314) / 100.0f;

		//�ړ��ʂ�ݒ肷��
		fSpeed = (float)(rand() % 300) / 100.0f + 4.0f;

		//�ړ��ʂ�ݒ肷��
		move.x = sinf(fAngle) * fSpeed;
		move.y = cosf(fAngle) * fSpeed;

		//���a��ݒ肷��
		fRadius = (float)(rand() % 1000) / 100.0f + 30.0f;

		// �����̐ݒ�
		nLife = rand() % 30 + 7;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create
		(
			ShiftPos,			// �ʒu
			move,				// �ړ���
			nLife,				// ����
			fRadius,			// ���a
			CEffect::TYPE_FIRE	// ���
		);
	}
}

//===============================
// �A�C�e���l������
//===============================
void CParticle::InitItemGet(const D3DXVECTOR3& pos)
{
	//[�����p�ϐ�]
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	int nLife = 0;											// �̗�
	float fAngle = 0.0f;									// ����
	float fSpeed = 0.0f;									// ���x
	float fRadius = 0.0f;									// ���a

	for (int nCntPar = 0; nCntPar < 20; nCntPar++)
	{
		//�p�x��ݒ肷��
		fAngle = (float)(rand() % 629 - 314) / 100.0f;

		//�ړ��ʂ�ݒ肷��
		fSpeed = (float)(rand() % 500) / 100.0f + 8.0f;

		//�ړ��ʂ�ݒ肷��
		move.x = sinf(fAngle) * fSpeed;
		move.y = cosf(fAngle) * fSpeed;

		//���a��ݒ肷��
		fRadius = (float)(rand() % 800) / 100.0f + 20.0f;

		// �����̐ݒ�
		nLife = rand() % 25 + 8;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create
		(
			pos,					// �ʒu
			move,					// �ړ���
			nLife,					// ����
			fRadius,				// ���a
			CEffect::TYPE_ITEMGET	// ���
		);
	}
}

//===============================
// �͂ݎ�(������)�̐���
//===============================
void CParticle::InitCatch(const D3DXVECTOR3& pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 EffectPos, move;
	int nLife;

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		// �G�t�F�N�g�̈ʒu��ݒ肷��
		EffectPos.x = pos.x;
		EffectPos.y = pos.y + rand() % 15 + 7;
		EffectPos.z = pos.z + rand() % 41 - 20;

		// �ړ��ʂ�ݒ肷��
		move.x = (float)(rand() % 7 - 3);
		move.y = (float)(rand() % 7 + 1);
		move.z = (float)(rand() % 7 - 3);

		// ������ݒ肷��
		nLife = rand() % 40;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create(EffectPos, move, nLife, 70.0f, CEffect::TYPE_CATCH);
	}
}

//===============================
// ���W�����v�̃`���[�W�̐���
//===============================
void CParticle::InitBigJumpCharge(const D3DXVECTOR3& pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 EffectPos, move;		// �ʒu�A�ړ���
	int nLife;							// ����
	float fRadius;						// ���a

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		// ������ݒ肷��
		nLife = rand() % 40;

		// �ړ��ʂ�ݒ肷��
		move.x = (float)(rand() % 11 - 5);
		move.y = (float)(rand() % 11 - 5);
		move.z = (float)(rand() % 11 - 5);

		// �G�t�F�N�g�̈ʒu��ݒ肷��
		EffectPos.x = -move.x * nLife;
		EffectPos.y = -move.y * nLife;
		EffectPos.z = -move.z * nLife;

		// ���a��ݒ肷��
		fRadius = (float)(rand() % 200 * 0.1f) + 10.0f;

		// �G�t�F�N�g�̐ݒ菈��
		CEffect::Create(pos + EffectPos, move, nLife, fRadius, CEffect::TYPE_BIGJUMPCHARGE);
	}
}

//===============================
// ���n���̐���
//===============================
void CParticle::InitLandDust(const D3DXVECTOR3& pos)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 EffectPos;		// �G�t�F�N�g�̈ʒu
	D3DXVECTOR3 EffectMove;		// �G�t�F�N�g�̈ړ���
	int nLife;					// ����
	D3DXVECTOR3 randRot;		// �����_���̌���

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		// ������ݒ肷��
		randRot.x = (float)((rand() % 629 - 314) * 0.01f);
		EffectPos.x = pos.x + sinf(randRot.x) * 20.0f;

		EffectPos.y = pos.y;

		// ������ݒ肷��
		randRot.z = (float)((rand() % 629 - 314) * 0.01f);
		EffectPos.z = pos.z + cosf(randRot.z) * 20.0f;

		for (int nCntDust = 0; nCntDust < 4; nCntDust++)
		{
			EffectMove.x = sinf(randRot.x) * (rand() % 3 + 10);
			EffectMove.y = (float)(rand() % 3 + 1);
			EffectMove.z = cosf(randRot.z) * (rand() % 3 + 10);

			// ������ݒ肷��
			nLife = rand() % 20 + 10;

			// �G�t�F�N�g�̐�������
			CEffect::Create(EffectPos, EffectMove, nLife, 50.0f, CEffect::TYPE_LANDDUST);
		}
	}
}