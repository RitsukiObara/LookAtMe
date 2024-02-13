//===================================
//
// ��Q���w�b�_�[[obstacle.h]
// Author ��������
//
//===================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �N���X��`(��Q��)
//-----------------------------------
class CObstacle : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	//-----------------------------------
	// ���
	//-----------------------------------
	typedef enum
	{
		TYPE_HYDRANT = 0,			// ���ΐ�
		TYPE_SIGNPOLE,				// �W��
		TYPE_DRUM,					// �h������
		TYPE_WOODBOX,				// �ؔ�
		TYPE_LAMP,					// �X��(����)
		TYPE_POLE,					// �X��(�x��)
		TYPE_MAX					// ���̗񋓌^�̑���
	}TYPE;

	CObstacle();															// �R���X�g���N�^
	CObstacle(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CObstacle();													// �f�X�g���N�^
	void Box(void);															// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);			// ����������
	virtual void Uninit(void);			// �I������
	virtual void Update(void);			// �X�V����
	virtual void Draw(void);			// �`�揈��

	virtual bool Hit(void);				// �q�b�g����

	virtual void SetData(const D3DXVECTOR3& pos, const TYPE type);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const TYPE type);		// ��ނ̐ݒ菈��
	TYPE GetType(void) const;			// ��ނ̎擾����

	void SetCollision(const bool bCollision);		// �����蔻��̐ݒ菈��
	bool GetCollision(void) const;					// �����蔻��̎擾����

	void SetAttack(const bool bAttack);				// �A�N�V�����̐ݒ菈��
	bool GetAttack(void) const;						// �A�N�V�����̎擾����

	void SetRange(const bool bRange);	// �U���͈͓����ǂ����̐ݒ菈��
	bool GetRange(void) const;			// �U���͈͓����ǂ����̎擾����

// �f�o�b�O����
#ifdef _DEBUG

	bool GetDelete(void) const;			// �����󋵂̎擾����
	void SetDelete(const bool bDelete);	// �����󋵂̐ݒ菈��

#endif 

	// �ÓI�����o�֐�
	static CObstacle* Create(const D3DXVECTOR3& pos, const TYPE type);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void ShadowPosSet(void);			// �e�̈ʒu�ݒ菈��

	// �����o�ϐ�
	TYPE m_type;						// ���
	int m_nShadowIdx;					// �e�̃C���f�b�N�X
	bool m_bCollision;					// �����蔻�肪�@�\���邩
	bool m_bAttack;						// �U���o���邩�ǂ���
	bool m_bRangeWithin;				// �U���͈͓����ǂ���

// �f�o�b�O����
#ifdef _DEBUG

	bool m_bDelete;			// ������
#endif 

};

#endif