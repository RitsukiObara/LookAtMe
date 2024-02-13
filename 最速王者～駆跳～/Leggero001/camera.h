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

	// �񋓌^��`(�J�����̎��)
	typedef enum
	{
		TYPE_NONE = 0,		// ����
		TYPE_UPWARD,		// �����
		TYPE_PRECEDENCE,	// ��s�ǐ�
		TYPE_ZOOMOUT,		// �Y�[���A�E�g
		TYPE_ZOOMIN,		// �Y�[���C��
		TYPE_STARTSTAGE,	// �X�^�[�g�J����(�X�e�[�W)
		TYPE_STARTRUN,		// �X�^�[�g�J����(����p)
		TYPE_STARTBACK,		// �X�^�[�g�J����(�߂�)
		TYPE_LAND,			// ���n
		TYPE_WALLKICK,		// �ǃL�b�N
		TYPE_BOOST,			// �u�[�X�g�J����
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	CCamera();		// �R���X�g���N�^
	~CCamera();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Set(void);					// �ݒ菈��

	void Rotation(void);			// ��荞�ݏ���
	void Reset(void);				// ���̃��Z�b�g����

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const TYPE type);	// ��ނ̐ݒ菈��
	TYPE GetType(void) const;		// ��ނ̎擾����

	void SetPosV(const D3DXVECTOR3 posV);		// ���_�̈ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosV(void) const;			// ���_�̈ʒu�̎擾����

	void SetPosR(const D3DXVECTOR3 posR);		// �����_�̈ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPosR(void) const;			// �����_�̈ʒu�̎擾����

	void SetRot(const D3DXVECTOR3& rot);		// �����̐ݒ菈��
	D3DXVECTOR3 GetRot(void) const;				// �����̎擾����

	void SetDistance(const float fDis);			// �����̐ݒ菈��
	float GetDistance(void) const;				// �����̎擾����

	void ChangeControl(void);					// �J�����̑���󋵂̐؂�ւ�����

private:		// ���������A�N�Z�X�o����

	// �����o�֐�
	void Move(void);				// �ړ�����
	void MovePosV(void);			// ���_���쏈��
	void MovePosR(void);			// �����_���쏈��
	void MoveRange(void);			// �������쏈��
	void Chase(void);				// �ǐՏ���
	void Control(void);				// ���쏈��
	void PosSet(void);				// �ʒu�̐ݒ菈��
	void TypeProcess(void);			// ��ނ̍X�V����

	// ��ނɂ�鏈��
	void Upward(void);				// ���������
	void Precedence(void);			// ��s�ǐՏ���
	void ZoomOut(void);				// �Y�[���A�E�g����
	void ZoomIn(void);				// �Y�[���C������
	void StartStage(void);			// �X�e�[�W���e�̏���
	void StartRun(void);			// ����p�̏���
	void StartBack(void);			// �����̏���
	void Land(void);				// ���n����
	void WallKick(void);			// �ǃL�b�N�̏���
	void Boost(void);				// �u�[�X�g�J�����̏���

	// ��ނɂ��ݒ菈��
	void SetUpward(void);			// ������J�����̐ݒ菈��
	void SetPrecedence(void);		// ��s�ǐՃJ�����̐ݒ菈��
	void SetZoomOut(void);			// �Y�[���A�E�g�J�����̐ݒ菈��
	void SetZoomIn(void);			// �Y�[���C���J�����̐ݒ菈��
	void SetStartStage(void);		// �X�e�[�W���e�J�����̐ݒ菈��
	void SetStartRun(void);			// ����p�J�����̐ݒ菈��
	void SetStartBack(void);		// �����J�����̐ݒ菈��
	void SetLand(void);				// ���n�J�����̐ݒ菈��
	void SetWallKick(void);			// �ǃL�b�N�̐ݒ菈��
	void SetBoost(void);			// �u�[�X�g�J�����̐ݒ菈��

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
	TYPE m_type;					// ���
	float m_rotDest;				// �ړI�̌���
	float m_Dis;					// ����
	float m_DisDest;				// �ړI�̋���
	int m_nSwingCount;				// �h��̃J�E���g
	bool m_bControl;				// �����
};

#endif