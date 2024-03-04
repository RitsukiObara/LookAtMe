//===================================
//
// �`���[�g���A���}�b�v�w�b�_�[[tutorial_map.h]
// Author ��������
//
//===================================
#ifndef _TUTORIAL_MAP_H_
#define _TUTORIAL_MAP_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CMeshWall;			// ���b�V���E�H�[��
class CMeshField;			// ���b�V���t�B�[���h

//-----------------------------------
// �N���X��`(�`���[�g���A���}�b�v)
//-----------------------------------
class CTutorialMap : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	CTutorialMap();			// �R���X�g���N�^
	~CTutorialMap();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CTutorialMap* Create(void);		// ��������

	// �萔��`
	static const int NUM_WALL = 3;		// �ǂ̑���

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CMeshWall* m_apWall[NUM_WALL];	// �ǂ̏��
};

#endif