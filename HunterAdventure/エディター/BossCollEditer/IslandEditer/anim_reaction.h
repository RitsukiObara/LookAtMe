//============================================
//
// �A�j�����A�N�V�����w�b�_�[[anim_reaction.h]
// Author�F��������
//
//============================================
#ifndef _ANIM_REACTION_H_			//���̃}�N����`������Ă��Ȃ�������
#define _ANIM_REACTION_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// �N���X(�A�j�����A�N�V�����N���X)
//--------------------------------------------
class CAnimReaction : public CBillboardAnim
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_EXPLOSION = 0,		// ����
		TYPE_GUNEXPLOSION,		// ���򔚔�
		TYPE_SPLASH,			// ��
		TYPE_WATEREXPLOSION,	// ������
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	CAnimReaction();		// �R���X�g���N�^
	~CAnimReaction();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CAnimReaction* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type, const int nCount, const int nLife);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	TYPE m_type;		// ���
	int m_nLife;		// ����
};

#endif