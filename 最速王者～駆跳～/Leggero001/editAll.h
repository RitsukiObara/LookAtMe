#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
// �S�G�f�B�b�g�w�b�_�[[editAll.h]
// Author ��������
//
//===================================
#ifndef _EDITALL_H_
#define _EDITALL_H_

//******************************************
// �C���N���[�h�t�@�C��
//******************************************
#include "main.h"

//------------------------------------------
// �O���錾
//------------------------------------------
class CEdit;			// �u���b�N�G�f�B�b�g
class CBgEdit;			// �w�i�G�f�B�b�g
class CObstacleEdit;	// ��Q���G�f�B�b�g
class CCameraEdit;		// �J�����G�f�B�b�g
class CRangeMove;		// �͈͈ړ�

//------------------------------------------
// �N���X��`(�G�f�B�b�g)
//------------------------------------------
class CEditAll
{
public:			// �N�ł��A�N�Z�X�ł���

	// �X�^�C��
	typedef enum
	{
		STYLE_BLOCK = 0,	// �u���b�N(�X�e�[�W)
		STYLE_BGOBJECT,		// �w�i�I�u�W�F�N�g
		STYLE_OBSTACLE,		// ��Q��
		STYLE_CAMERA,		// �J����
		STYLE_RANGEMOVE,	// �͈͈ړ�
		STYLE_MAX			// ���̗񋓌^�̑���
	}STYLE;

	CEditAll();				// �R���X�g���N�^
	~CEditAll();			// �f�X�g���N�^

	//�v���g�^�C�v�錾
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �Z�b�g�E�Q�b�g�֌W
	STYLE GetStyle(void) const;		// �X�^�C���̎擾����

	// �ÓI�����o�֐�
	static CEditAll* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void StyleChange(void);				// �X�^�C���̐ݒ菈��
	void RangeMoveChange(void);			// �͈͈ړ��̐ݒ菈��
	D3DXVECTOR3 GetEditPos(void);		// �ʒu�̎擾����
	void AllDelete(void);				// �|�C���^�S��������
	void Set(const D3DXVECTOR3& pos);	// �ݒ菈��

	void SetItem(void);				// �A�C�e���̐ݒ菈��
	void DeleteItem(void);			// �A�C�e���̏�������
	void SetItemGenerator(void);	// �A�C�e��������̐ݒ菈��
	void DeleteItemGenerator(void);	// �A�C�e��������̏�������

	// �����o�ϐ�
	STYLE m_style;			// �X�^�C��
	STYLE m_saveStyle;		// �ۑ��p�X�^�C��

	// �ÓI�����o�ϐ�
	static CEdit* m_pEdit;						// �G�f�B�b�g�̃|�C���^
	static CBgEdit* m_pBgEdit;					// �w�i�G�f�B�b�g�̃|�C���^
	static CObstacleEdit* m_pObstacleEdit;		// ��Q���G�f�B�b�g�̃|�C���^
	static CCameraEdit* m_pCameraEdit;			// �J�����G�f�B�b�g�̃|�C���^
	static CRangeMove* m_pRangeMove;			// �͈͈ړ��̃|�C���^
};

#endif

#endif