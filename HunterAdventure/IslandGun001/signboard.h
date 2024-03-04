//===================================
//
// �Ŕw�b�_�[[signboard.h]
// Author ��������
//
//===================================
#ifndef _SIGNBOARD_H_
#define _SIGNBOARD_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPushTiming;		// �{�^���\��
class CSignboardExpl;	// �Ŕ̐���

//-----------------------------------
// �N���X��`(�Ŕ�)
//-----------------------------------
class CSignboard : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_JUMP = 0,		// �W�����v�̐���
		TYPE_MOVE,			// �ړ��̐���
		TYPE_SHOT,			// �ˌ��̐���
		TYPE_CAMERAMOVE,	// �J�����ړ��̐���
		TYPE_DAGGER,		// �_�K�[�̐���
		TYPE_DODGE,			// ����̐���
		TYPE_SHOTGUN,		// �U�e�̐���
		TYPE_SWOOP,			// �}�~���̐���
		TYPE_GAME,			// �Q�[���̐���
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CSignboard();			// �R���X�g���N�^
	~CSignboard();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);					// ���̐ݒ菈��
	void Explain(void);				// �����ڍs����

	// �Z�b�g�E�Q�b�g�֌W
	void SetExplain(CSignboardExpl* pExpl);		// �����̐ݒ菈��
	void SetEnableDisp(const bool bDisp);		// �`��󋵂̐ݒ菈��
	bool IsDisp(void) const;					// �`��󋵂̎擾����

	// �ÓI�����o�֐�
	static CSignboard* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);		// ��������

	static CListManager<CSignboard*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CPushTiming* m_pButton;		// �{�^���̏��
	CSignboardExpl* m_pExplain;	// �����̏��
	TYPE m_type;				// ���
	bool m_bDisp;				// �`���

	// �ÓI�����o�ϐ�
	static CListManager<CSignboard*> m_list;		// ���X�g
};

#endif