//===================================
//
//�A�C�e���w�b�_�[[Item.h]
//Author ��������
//
//===================================
#ifndef _ITEM_H_			//���̃}�N����`������Ă��Ȃ�������
#define _ITEM_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"

//-----------------------------------
// �}�N����`
//-----------------------------------
#define ITEM_ROT		(D3DXVECTOR3(0.0f,0.0f,0.5f))			// �A�C�e���̌X��

//-----------------------------------
// �N���X��`(�A�C�e��)
//-----------------------------------
class CItem : public CModel
{
public:		// �N�ł��A�N�Z�X�ł���

	// ���
	typedef enum
	{
		STATE_APPEAR = 0,		// �o������
		STATE_NONE,				// �ʏ���
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CItem();			// �R���X�g���N�^
	~CItem();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��
	void Hit(void);					// �q�b�g����

	void SetData(const D3DXVECTOR3& pos, const STATE state);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CItem* Create(const D3DXVECTOR3& pos, STATE state);		// ��������

private:	// ���������A�N�Z�X�ł���

	// �����o�֐�
	void HealingStamina(void);					// �X�^�~�i�̉񕜏���

	// �����o�ϐ�
	STATE m_state;								// ���
	D3DXVECTOR3 m_rotmove;						// �ړ���
};

#endif