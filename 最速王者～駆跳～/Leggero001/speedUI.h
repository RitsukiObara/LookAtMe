//============================================
//
// ���xUI�w�b�_�[[speedUI.h]
// Author�F��������
//
//============================================
#ifndef _SPEEDUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _SPEEDUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"
#include "scene.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define SPEEDUI_MARK_SIZE			(D3DXVECTOR3(50.0f, 50.0f, 0.0f))													// ���xUI�̃}�[�N�̃T�C�Y

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;			// �I�u�W�F�N�g2D

//--------------------------------------------
// �N���X��`(���xUI)
//--------------------------------------------
class CSpeedUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	//--------------------------------------------
	// �񋓌^��`(�|���S���̎��)
	//--------------------------------------------
	typedef enum
	{
		TYPE_GAGE = 0,		// �Q�[�W
		TYPE_METER,			// ���[�^�[
		TYPE_MARK,			// �}�[�N
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	CSpeedUI();				// �R���X�g���N�^
	CSpeedUI(CObject::TYPE type, PRIORITY priority = PRIORITY_UI);				// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CSpeedUI();	// �f�X�g���N�^
	void Box(void);			// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void);		// �X�V����
	virtual void Draw(void);		// �`�揈��
	
	virtual void SetData(void) = 0;				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	CObject2D* GetObject2D(const TYPE type);	// �I�u�W�F�N�g�̎擾����

	// �ÓI�����o�֐�
	static CSpeedUI* Create(const CScene::MODE mode);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SetSpeedMeter(void);			// ���[�^�[�ݒ菈��
	void SizeCorrect(void);				// �T�C�Y�̕␳����

	// �����o�ϐ�
	CObject2D* m_apObject2D[TYPE_MAX];		// �I�u�W�F�N�g�̏��
	float m_fLevel;							// ���x��
	float m_fLevelDest;						// �ڕW�̃��x��
	int m_nOverTopTex;						// �X�[�p�[�u�[�X�g���̃e�N�X�`���̃C���f�b�N�X
	int m_nMeterTex;						// ���[�^�[�̃e�N�X�`���̃C���f�b�N�X�̕ۑ��ϐ�

	// �ÓI�����o�ϐ�
	static const char* m_apTexturename[TYPE_MAX];	// �e�N�X�`���̃p�X��
};

#endif