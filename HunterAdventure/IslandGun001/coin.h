//===================================
//
// �����w�b�_�[[coin.h]
// Author ��������
//
//===================================
#ifndef _COIN_H_
#define _COIN_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(����)
//-----------------------------------
class CCoin : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ��� 
		STATE_GET,			// �擾���
		STATE_DEATH,		// ���S���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CCoin();					// �R���X�g���N�^
	~CCoin();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos);			// ���̐ݒ菈��
	void Hit(void);					// �q�b�g����

	// �Z�b�g�E�Q�b�g�֐�
	STATE GetState(void) const;		// ��Ԃ̎擾����

	// �ÓI�����o�֐�
	static CCoin* Create(const D3DXVECTOR3& pos);	// ��������

	static CListManager<CCoin*> GetList(void);		// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Get(void);			// �擾��ԏ���
	void Cycle(void);		// ��]����

	// �����o�ϐ�
	STATE m_state;			// ���
	int m_nDeathCount;		// ���S�J�E���g
	float m_fCycleSpeed;	// ��]���x
	float m_fHeightDest;	// �ڕW�̍���

	// �ÓI�����o�ϐ�
	static CListManager<CCoin*> m_list;		// ���X�g
};

#endif