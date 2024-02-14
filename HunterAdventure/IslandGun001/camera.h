//===================================
//
// �J�����w�b�_�[[camera.h]
// Author ��������
//
//===================================
#ifndef _CAMERA_H_			//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �N���X��`(�J����)
//-----------------------------------
class CCamera
{
public:			// �N�ł��A�N�Z�X�o����

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_NONE = 0,		// �ʏ���
		TYPE_VIBRATE,		// �U�����
		TYPE_BOSSCLOSER,	// �{�X�����
		TYPE_BOSSHOWLING,	// �{�X�Y�����я��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �\���̒�`(�U��)
	struct SVibrate
	{
		TYPE nextType;		// ���̎��
		int nElapseCount;	// �o�߃J�E���g
		int nSwingCount;	// �h��̃J�E���g
		int nSwingRange;	// �h��͈̔�
		int nFinishCount;	// �I���J�E���g
		float fCorrect;		// �␳�W��
		bool bDown;			// ����
	};

	CCamera();		// �R���X�g���N�^
	~CCamera();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Set(void);					// �ݒ菈��

	void Rotation(void);			// ��荞�ݏ���
	void Reset(void);				// ���̃��Z�b�g����

	void SetPosV(const D3DXVECTOR3 posV);		// ���_�̈ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosV(void) const;			// ���_�̈ʒu�̎擾����

	void SetPosR(const D3DXVECTOR3 posR);		// �����_�̈ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosR(void) const;			// �����_�̈ʒu�̎擾����

	void SetRot(const D3DXVECTOR3& rot);		// �����̐ݒ菈��
	D3DXVECTOR3 GetRot(void) const;				// �����̎擾����

	void SetDistance(const float fDis);			// �����̐ݒ菈��
	float GetDistance(void) const;				// �����̎擾����

	void SetViewport(const D3DVIEWPORT9& viewport);		// �r���[�|�[�g�̐ݒ菈��
	D3DVIEWPORT9 GetViewport(void) const;				// �r���[�|�[�g�̐ݒ菈��

	void SetVibrate(const SVibrate vib);		// �U���֌W�̐ݒ菈��
	SVibrate GetVibrate(void) const;			// �U���֌W�̎擾����

	void SetType(const TYPE type);				// ��ނ̐ݒ菈��
	TYPE GetType(void) const;					// ��ނ̎擾����

	void ChangeControl(void);					// �J�����̑���󋵂̐؂�ւ�����

	// �ÓI�����o�֐�
	static CCamera* Create(void);		// ��������

private:		// ���������A�N�Z�X�o����

	// �����o�֐�
	// �L�[�{�[�h����
	void Control(void);				// ���쏈��
	void Move(void);				// �ړ�����
	void MovePosV(void);			// ���_���쏈��
	void MovePosR(void);			// �����_���쏈��
	void MoveRange(void);			// �������쏈��

	// �}�E�X����
	void MouseControl(void);		// �}�E�X�ł̃J�������쏈��
	void MousePosR(void);			// �����_���쏈��
	void MousePosV(void);			// ���_���쏈��
	void MousePosMove(void);		// ���_�E���n�_���쏈��

	void Chase(void);				// �ǐՏ���
	void Vibrate(void);				// �U������
	void BossCloser(void);			// �{�X��菈��
	void BossHowling(void);			// �{�X�Y�����я���
	void PosSet(void);				// �ʒu�̐ݒ菈��
	void TypeProcess(void);			// �J�����̎�ނ��Ƃ̏���

	// �����o�ϐ�
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posVDest;			// �ړI�̎��_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_posRDest;			// �ړI�̒����_
	D3DXVECTOR3 m_VecU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// ����
	D3DVIEWPORT9 m_viewport;		// �r���[�|�[�g
	SVibrate m_vibrate;				// �U���֌W
	TYPE m_type;					// ���
	float m_Dis;					// ����
	bool m_bControl;				// �����
};

#endif