//============================================
//
// �Q�[���̃v���C���[�w�b�_�[[game_player.h]
// Author�F��������
//
//============================================
#ifndef _GAME_PLAYER_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _GAME_PLAYER_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "player.h"

//--------------------------------------------
// �N���X(�Q�[���̃v���C���[�N���X)
//--------------------------------------------
class CGamePlayer : public CPlayer
{
public:			// �N�ł��A�N�Z�X�ł���

	CGamePlayer();				// �R���X�g���N�^
	~CGamePlayer();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void CollisionStageLeft(void);		// �X�e�[�W�̍��[�̓����蔻��

	// �����o�ϐ�
	D3DXVECTOR3 m_stageLeft;	// �X�e�[�W�̍��[
};

#endif