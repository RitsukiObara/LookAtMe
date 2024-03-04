//===================================
//
// �X�R�A�ǉ�UI�w�b�_�[[addscoreUI.h]
// Author ��������
//
//===================================
#ifndef _ADDSCORE_UI_H_
#define _ADDSCORE_UI_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(CAddScoreUI)
//-----------------------------------
class CAddScoreUI : public CBillboard
{
public:

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_COIN = 0,		// �R�C��
		TYPE_TORDLE,		// �^�[�h��
		TYPE_IWAKARI,		// �C���J��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CAddScoreUI();			// �R���X�g���N�^
	~CAddScoreUI();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const TYPE type);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CAddScoreUI* Create(const D3DXVECTOR3& pos, const TYPE type);	// ��������

private:

	// �����o�ϐ�
	TYPE m_type;			// ���
	int m_nLife;			// ����
	float m_fAlpha;			// �����x
};

#endif