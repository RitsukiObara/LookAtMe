//============================================
//
// �`���[�g���A���̃v���C���[�w�b�_�[[tutorial_player.h]
// Author�F��������
//
//============================================
#ifndef _TUTORIAL_PLAYER_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_PLAYER_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "player.h"

//--------------------------------------------
// �����^�C�~���O
//--------------------------------------------
class CPushTiming;			// �����^�C�~���O

//--------------------------------------------
// �N���X(�`���[�g���A���v���C���[�N���X)
//--------------------------------------------
class CTutorialPlayer : public CPlayer
{
public:			// �N�ł��A�N�Z�X�ł���

	CTutorialPlayer();		// �R���X�g���N�^
	~CTutorialPlayer();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const CScene::MODE mode);		// ���̐ݒ菈��

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void CollisionStageLeft(void);		// �X�e�[�W�̍��[�Ƃ̓����蔻��

	// �����o�ϐ�
	CPushTiming* m_pPushTiming;		// �����^�C�~���O�̃|�C���^
};

#endif