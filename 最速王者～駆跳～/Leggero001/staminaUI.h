//============================================
//
// �X�^�~�iUI�w�b�_�[[staminaUI.h]
// Author�F��������
//
//============================================
#ifndef _STAMINAUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _STAMINAUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"
#include "scene.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define STAMINAUI_MARK_SIZE			(D3DXVECTOR3(50.0f, 50.0f, 0.0f))		// �X�^�~�iUI�̃}�[�N�̃T�C�Y

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;			// �I�u�W�F�N�g2D
class CAnim;				// �A�j���[�V����

//--------------------------------------------
// �N���X��`(�Q�[���̃X�^�~�iUI)
//--------------------------------------------
class CStaminaUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_HEAL,			// �񕜏��
		STATE_MAX			// ���̗񋓌^�̑���
	}STATE;

	// �񋓌^��`(�X�^�~�iUI�̃|���S��)
	typedef enum
	{
		TYPE_GAGE = 0,		// �Q�[�W
		TYPE_METER,			// ���[�^�[
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	// �񋓌^��`(�e�N�X�`��)
	typedef enum
	{
		TEX_GAGE = 0,		// �Q�[�W
		TEX_METER,			// ���[�^�[
		TEX_MARK,			// �}�[�N
		TEX_MAX				// ���̗񋓌^�̑���
	}TEX;

	CStaminaUI();				// �R���X�g���N�^
	CStaminaUI(CObject::TYPE type, PRIORITY priority = PRIORITY_UI);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CStaminaUI();		// �f�X�g���N�^
	void Box(void);				// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void);		// �X�V����
	virtual void Draw(void);		// �`�揈��

	virtual void SetData(void) = 0;			// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetState(const STATE state);		// ��Ԃ̐ݒ菈��
	STATE GetState(void) const;				// ��Ԃ̎擾����

	CObject2D* GetObject2D(const TYPE type) const;		// �I�u�W�F�N�g2D�̎擾����
	CAnim* GetAnim(void) const;							// �A�j���[�V�����̎擾����
	
	// �ÓI�����o�֐�
	static CStaminaUI* Create(const CScene::MODE mode);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �X�^�~�i���f����
	void StaminaInfluence(void);		// �X�^�~�i���f����
	void SizeSub(void);					// �T�C�Y�̌��Z����
	void CalcStaminaSub(void);			// �X�^�~�i�̌��Z�ʌv�Z����
	void ColorCalc(void);				// �X�^�~�i�̃J���[�v�Z����
	void NoneStateProcess(void);		// �ʏ��Ԃ̏���
	void HealStateProcess(void);		// �񕜏�Ԃ̏���

	// �����o�ϐ�
	CObject2D* m_apObject2D[TYPE_MAX];		// �I�u�W�F�N�g�̏��
	CAnim* m_pAnim;							// �A�j���[�V�����̏��
	float m_fStaminaDest;					// �ڕW�̃X�^�~�i
	float m_fSubStamina;					// �X�^�~�i�̌��Z��
	int m_nFlashCount;						// �_�ŃJ�E���g
	int m_nStateCount;						// ��ԃJ�E���g
	STATE m_state;							// ���
	bool m_bFlash;							// �_�ŏ�

	// �ÓI�����o�ϐ�
	static const char* m_apTexturename[TEX_MAX];	// �e�N�X�`���̃p�X��
};

#endif