//===================================
//
// �v���C���[�̍s���w�b�_�[[player_action.h]
// Author ��������
//
//===================================
#ifndef _PLAYER_ACTION_H_			// ���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_ACTION_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPlayer;		// �v���C���[

//-----------------------------------
// �N���X��`(�v���C���[�̍s��)
//-----------------------------------
class CPlayerAction
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�s��)
	enum ACTION
	{
		ACTION_NONE = 0,	// �ʏ���
		ACTION_SHOT,		// �ˌ����
		ACTION_DAGGER,		// �_�K�[���
		ACTION_DODGE,		// ������
		ACTION_SHOTGUN,		// �U�e(J+�e)���
		ACTION_SWOOP,		// �}�~��(J+�_�K�[)���
		ACTION_RELOAD,		// �����[�h���
		ACTION_MAX			// ���̗񋓌^�̑���
	};

	CPlayerAction();		// �R���X�g���N�^
	~CPlayerAction();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);					// ����������
	void Uninit(void);					// �I������
	void Update(CPlayer* pPlayer);		// �X�V����

	// �Z�b�g�E�Q�b�g�֐�
	void SetMoveRecoil(const D3DXVECTOR3& move);	// �ړ��ʂ̐ݒ菈��
	D3DXVECTOR3 GetMoveRecoil(void) const;			// �ړ��ʂ̎擾����

	void SetAction(const ACTION action);			// �s���̐ݒ菈��
	ACTION GetAction(void) const;					// �s���̎擾����

	void SetDodgeRot(const float fRot);				// �����������̐ݒ菈��

	void SetDodgeInterval(const int nInterval);		// ����C���^�[�o���̐ݒ菈��

	void SetEnableDodgeUse(const bool bUse);		// ����g�p�\��Ԃ̐ݒ菈��
	bool IsDodgeUse(void) const;					// ����g�p�\�󋵂̎擾����

	void SetEnableRecoil(const bool bRecoil);		// �����󋵂̐ݒ菈��
	bool IsRecoil(void) const;						// �����󋵂̎擾����

	// �ÓI�����o�֐�
	static CPlayerAction* Create(void);		// ��������

private:		// �N�ł��A�N�Z�X�ł���

	// �����o�֐�
	void NoneProcess(void);					// �ʏ��ԏ���
	void ShotProcess(CPlayer* pPlayer);		// �ˌ���ԏ���
	void DaggerPrecess(CPlayer* pPlayer);	// �_�K�[��ԏ���
	void DodgeProcess(CPlayer* pPlayer);	// �����ԏ���
	void ShotgunProcess(CPlayer* pPlayer);	// �U�e(J+�e)���
	void SwoopProcess(CPlayer* pPlayer);	// �}�~����ԏ���
	void ReloadProcess(CPlayer* pPlayer);	// �����[�h����

	// �����o�ϐ�
	D3DXVECTOR3 m_moveRecoil;	// �����̈ړ���
	ACTION m_action;			// �s��
	int m_nActionCount;			// �s���J�E���g
	int m_nDodgeInterval;		// ����̃C���^�[�o���J�E���g
	float m_fDodgeRot;			// ����������
	bool m_bDodgeUse;			// ����g�p�\��
	bool m_bRecoil;				// ������
};

#endif