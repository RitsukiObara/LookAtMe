//===================================
//
// ��s�@�w�b�_�[[airplane.h]
// Author ��������
//
//===================================
#ifndef _AIRPLANE_H_
#define _AIRPLANE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPropeller;			// �v���y��
class CPlayer;				// �v���C���[

//-----------------------------------
// �N���X��`(��s�@)
//-----------------------------------
class CAirplane : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_MOVE = 0,		// �ړ����
		STATE_FALL,			// �ė����
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CAirplane();			// �R���X�g���N�^
	~CAirplane();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Update(CPlayer* pPlayer);	// �X�V����(�v���C���[�Ǌ��Ŏg������)
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& posDest);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����
	D3DXVECTOR3 GetPosDest(void) const;		// �ړI�̈ʒu�̎擾����

	// �ÓI�����o�֐�
	static CAirplane* Create(const D3DXVECTOR3& posDest);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void Move(CPlayer* pPlayer);	// �ړ�����
	void Fall(void);				// �ė�����

	// �����o�ϐ�
	CPropeller* m_pPropeller;		// �v���y���̏��
	D3DXVECTOR3 m_posDest;			// �ړI�̈ʒu
	D3DXVECTOR3 m_move;				// �ړ���
	STATE m_state;					// ���
	int m_nFallCount;				// �ė��J�E���g
};

#endif