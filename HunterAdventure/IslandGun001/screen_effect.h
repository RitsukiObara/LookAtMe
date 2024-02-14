//===================================
//
// ��ʃG�t�F�N�g�w�b�_�[[screen_effect.h]
// Author ��������
//
//===================================
#ifndef _SCREEN_EFFECT_H_
#define _SCREEN_EFFECT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object2D.h"

//-----------------------------------
// �N���X��`(CScreenEffect)
//-----------------------------------
class CScreenEffect : public CObject2D
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_RED_AROUND = 0,	// ���Ӑ�
		TYPE_BLOOD,				// ��
		TYPE_CONCENTRATE,		// �W����
		TYPE_ALL_RED,			// �S������
		TYPE_BLACK,				// ��
		TYPE_MAX				// ���̗񋓌^�̑���
	};

	CScreenEffect();		// �R���X�g���N�^
	~CScreenEffect();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const TYPE type);			// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֐�
	void SetAlpha(const float fAlpha);		// �����x�̐ݒ菈��
	float GetAlpha(void) const;				// �����x�̎擾����

	// �ÓI�����o�֐�
	static CScreenEffect* Create(const TYPE type);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	TYPE m_type;		// ���
	float m_fAlpha;		// �����x
};

#endif