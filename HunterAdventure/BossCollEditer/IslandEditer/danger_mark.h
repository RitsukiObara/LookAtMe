//===================================
//
// �댯�}�[�N�w�b�_�[[danger_mark.h]
// Author ��������
//
//===================================
#ifndef _DANGER_MARK_H_
#define _DANGER_MARK_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CDangerMark)
//-----------------------------------
class CDangerMark : public CBillboard
{
public:

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_YELLOWJAGGY = 0,	// ���F�̃M�U�M�U
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	CDangerMark();			// �R���X�g���N�^
	~CDangerMark();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CDangerMark* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const D3DXCOLOR& col, const TYPE type);		// ��������

private:

	// �����o�ϐ�
	D3DXCOLOR m_col;		// �F
	float m_fAlphaDest;		// �ړI�̓����x
	bool m_bDisp;			// �`���
};

#endif