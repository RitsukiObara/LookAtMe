//===================================
//
// �A�C�e��������w�b�_�[[item_generator.h]
// Author ��������
//
//===================================
#ifndef _ITEM_GENERATOR_H_			//���̃}�N����`������Ă��Ȃ�������
#define _ITEM_GENERATOR_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CItem;			// �A�C�e��

//-----------------------------------
// �N���X��`(�A�C�e��������)
//-----------------------------------
class CItemGenerator : public CModel
{
public:		// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_SET = 0,			// �ݒ���
		STATE_VIBRATE,			// �U������
		STATE_NONE,				// �ʏ폈��
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CItemGenerator();			// �R���X�g���N�^
	~CItemGenerator();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	void SetData(const D3DXVECTOR3& pos);	// ���̐ݒ菈��

	void Discriminate(void);				// ���ʏ���

	// �ÓI�����o�֐�
	static CItemGenerator* Create(const D3DXVECTOR3& pos);		// ��������

private:	// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CItem* m_pItemAddress;	// �A�C�e���̃A�h���X
	STATE m_state;			// ���
	int m_nStateCount;		// ��ԃJ�E���g
	float m_fRotDest;		// �ڕW�̌���
};

#endif