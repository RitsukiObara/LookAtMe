#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
// ��Q���G�f�B�b�g�w�b�_�[[obstacleEdit.h]
// Author ��������
//
//===================================
#ifndef _OBSTACLEEDIT_H_
#define _OBSTACLEEDIT_H_

#include "main.h"
#include "obstacle.h"

//------------------------------------------
// �N���X��`(�G�f�B�b�g)
//------------------------------------------
class CObstacleEdit : public CObstacle
{
public:			// �N�ł��A�N�Z�X�ł���

	CObstacleEdit();		// �R���X�g���N�^
	~CObstacleEdit();		// �f�X�g���N�^

	//�v���g�^�C�v�錾
	HRESULT Init(void);		// �I�u�W�F�N�g�̏���������
	void Uninit(void);		// �I�u�W�F�N�g�̏I������
	void Update(void);		// �I�u�W�F�N�g�̍X�V����
	void Draw(void);		// �I�u�W�F�N�g�̕`�揈��

	void SetData(const D3DXVECTOR3& pos);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CObstacleEdit* Create(const D3DXVECTOR3& pos);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void TypeChange(void);		// ��ޕύX����
	void Move(void);			// �ړ�����
	void Set(void);				// �I�u�W�F�N�g�̐ݒ菈��
	void Delete(void);			// �I�u�W�F�N�g�̏���
	void Reset(void);			// �I�u�W�F�N�g�̏�񃊃Z�b�g����
	void PlayerPosSet(void);	// �I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
};

#endif

#endif