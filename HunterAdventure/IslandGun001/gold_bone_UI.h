//============================================
//
// ���̍�UI�w�b�_�[[gold_bone_UI.h]
// Author�F��������
//
//============================================
#ifndef _GOLD_BONE_UI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _GOLD_BONE_UI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define MAX_BONE_MARK		(3)		// ���}�[�N�̍ő�l

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��

//--------------------------------------------
// �N���X��`(���̍�UI)
//--------------------------------------------
class CGoldBoneUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,			// �����
		STATE_EXTEND,			// �g����
		STATE_SHRINK,			// �k�����
		STATE_STOP,				// ��~���
		STATE_MAX				// ���̗񋓌^�̑���
	};

	struct SBoneMark
	{
		CObject2D* pShadow;		// �e
		CObject2D* pMark;		// �}�[�N
		STATE state;			// ���
	};

	CGoldBoneUI();				// �R���X�g���N�^
	~CGoldBoneUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��

	void GetGoldBone(void);			// ���̍��̃Q�b�g����

	// �ÓI�����o�֐�
	static CGoldBoneUI* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	SBoneMark m_aBoneMark[MAX_BONE_MARK];		// ���̍��̏��
};

#endif