//=====================================================
//
// ���b�V���̔g�w�b�_�[ [mesh_wave.h]
// Author�F��������
//
//=====================================================
#ifndef _MESH_WAVE_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_WAVE_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "mesh_field.h"

//-----------------------------------------------------
// �N���X��`(���b�V���̔g)
//-----------------------------------------------------
class CMeshWave : public CMeshField
{
public:		// �N�ł��A�N�Z�X�ł���

	CMeshWave(CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMeshWave();			// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed);		// �f�[�^�̐ݒ菈��

	// �ÓI�����o�֐�
	static CMeshWave* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const float fWidth, const float fDepth, const SGrid& Divi, const float fWaveHeight, const float fCycle, const float fSpeed, const PRIORITY priority);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SetWave(void);		// �g�̐ݒ菈��

	// �����o�ϐ�
	float m_fWaveRot;		// �g�̌���
	float m_fWaveHeight;	// �g�̍���
	float m_fCycle;			// �g�̎���
	float m_fSpeed;			// �g�̑��x
};


#endif