//============================================
//
// ����UI�w�b�_�[[lifeUI.h]
// Author�F��������
//
//============================================
#ifndef _LIFE_UI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _LIFE_UI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��

//--------------------------------------------
// �N���X��`(����UI)
//--------------------------------------------
class CLifeUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�|���S��)
	enum POLY
	{
		POLY_GROUND = 0,	// ���n
		POLY_METER,			// ���[�^�[
		POLY_WIPE,			// ���C�v
		POLY_MAX			// ���̗񋓌^�̑���
	};

	// �\���̒�`(�A�C�R���̓_��)
	struct SIconFlash
	{
		D3DXCOLOR col;		// �A�C�R���̐F
		bool bAdd;			// ���Z��
	};

	CLifeUI();				// �R���X�g���N�^
	~CLifeUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const int nLife);	// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetLife(const int nLife);	// �����̐ݒ菈��
	int GetLife(void) const;		// �����̎擾����

	// �ÓI�����o�֐�
	static CLifeUI* Create(const int nLife);	// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void IconFlash(void);		// �A�C�R���̓_�ŏ���

	// �����o�ϐ�
	CObject2D* m_apObject2D[POLY_MAX];		// �|���S���̏��
	SIconFlash m_iconflash;		// �A�C�R���̓_�Ŋ֌W
	int m_nLife;				// ����
	float m_fMeterDest;			// �ړI�̃��[�^�[
	float m_fMeter;				// ���[�^�[
};

#endif