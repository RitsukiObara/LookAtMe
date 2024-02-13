//===================================
//
// �p�[�e�B�N���w�b�_�[[Particle.h]
// Author ��������
//
//===================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Effect.h"

//-----------------------------------
// �N���X��`(CParticle)
//-----------------------------------
class CParticle
{
public:			// �N�ł��A�N�Z�X�o����

	CParticle();							// �R���X�g���N�^
	~CParticle();							// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�֐�
	static void Create(const D3DXVECTOR3& pos, const CEffect::TYPE type);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �p�[�e�B�N����������
	void InitFire(const D3DXVECTOR3& pos);				// ���n�̐���
	void InitItemGet(const D3DXVECTOR3& pos);			// �A�C�e���l���̐���
	void InitCatch(const D3DXVECTOR3& pos);				// �͂ݎ�(������)�̐���
	void InitBigJumpCharge(const D3DXVECTOR3& pos);		// ���W�����v�̃`���[�W�̐���
	void InitLandDust(const D3DXVECTOR3& pos);			// ���n���̐���
};

#endif