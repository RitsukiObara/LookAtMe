//=====================================================
//
// �C�w�b�_�[ [ocean.h]
// Author�F��������
//
//=====================================================
#ifndef _OCEAN_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _OCEAN_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*****************************************************
// �C���N���[�h�t�@�C��
//*****************************************************
#include "mesh_wave.h"

//-----------------------------------------------------
// �N���X��`(�C)
//-----------------------------------------------------
class COcean : public CMeshWave
{
public:		// �N�ł��A�N�Z�X�ł���

	COcean();			// �R���X�g���N�^
	~COcean();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// �f�[�^�̐ݒ菈��

	// �ÓI�����o�֐�
	static COcean* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

};


#endif