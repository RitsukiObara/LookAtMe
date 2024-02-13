//============================================
//
// �R���{�w�b�_�[[combo.h]
// Author�F��������
//
//============================================
#ifndef _COMBO_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _COMBO_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define COMBO_DIGIT			(3)		// �R���{�̌���

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;			// �I�u�W�F�N�g2D
class CNumber;				// �ԍ�

//--------------------------------------------
// �N���X��`(���l)
//--------------------------------------------
class CCombo : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_WORD = 0,		// ���t
		TYPE_MULTI,			// �~�}�[�N
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �\���̒�`(�R���{�̒l)
	typedef struct
	{
		CNumber* pNumber;	// �����̏��
		bool bDisp;			// �`���
	}SCombo;

	CCombo();				// �R���X�g���N�^
	~CCombo();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �ÓI�����o�֐�
	static CCombo* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Calculation(void);				// �v�Z����
	void DispDecide(void);				// �`�攻�菈��

	// �����o�ϐ�
	CObject2D* m_apObject2D[TYPE_MAX];	// 2D�|���S���̏��
	SCombo m_aNumber[COMBO_DIGIT];		// ���l�̏��
	int m_nCombo;						// �R���{��
};

#endif