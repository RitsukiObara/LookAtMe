//===================================
//
// �v���C���[�̃R���g���[���[�w�b�_�[[player_controller.h]
// Author ��������
//
//===================================
#ifndef _PLAYER_CONTROLLER_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_CONTROLLER_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPlayer;		// �v���C���[

//-----------------------------------
// �N���X��`(�v���C���[�̃R���g���[���[)
//-----------------------------------
class CPlayerController
{
public:			// �N�ł��A�N�Z�X�ł���

	CPlayerController();		// �R���X�g���N�^
	~CPlayerController();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);					// ����������
	void Uninit(void);					// �I������
	void Control(CPlayer* pPlayer);		// ���쏈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetSpeed(const float fSpeed);	// ���x�̐ݒ菈��
	float GetSpeedInit(void) const;		// �������x�̎擾����

	// �ÓI�����o�֐�
	static CPlayerController* Create(void);		// ��������

private:		// �N�ł��A�N�Z�X�ł���

	// �����o�֐�
	void ElevationCamera(void);				// �N���n�ʂƃJ�����̓����蔻��
	void CameraControl(CPlayer* pPlayer);	// �J�����̑��쏈��
	void CameraMouse(CPlayer* pPlayer);		// �}�E�X�ł̃J�����̑��쏈��
	void RotMove(CPlayer* pPlayer);			// �����̐ݒ菈��
	void KeyboardMove(CPlayer* pPlayer);	// �L�[�{�[�h�ł̐ݒ菈��
	void Jump(CPlayer* pPlayer);			// �W�����v����
	void Shot(CPlayer* pPlayer);			// �ˌ�����
	void HandGun(CPlayer* pPlayer);			// ���e����
	void ShotGun(CPlayer* pPlayer);			// �U�e����
	void Avoid(CPlayer* pPlayer);			// �������
	void Dagger(CPlayer* pPlayer);			// �_�K�[����

	// �����o�ϐ�
	int m_nShotCount;			// �ˌ��J�E���g
	int m_nWalkCount;			// ���s�J�E���g
	float m_fStickRot;			// �X�e�B�b�N�̌���
	float m_fSpeed;				// ���x
	bool m_bRightShot;			// �E�Ō����ǂ���
};

#endif