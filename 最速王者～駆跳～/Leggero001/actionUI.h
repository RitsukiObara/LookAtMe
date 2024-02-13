//============================================
//
// �A�N�V����UI�w�b�_�[[actionUI.h]
// Author�F��������
//
//============================================
#ifndef _ACTIONUI_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _ACTIONUI_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;

//--------------------------------------------
// �N���X��`(�A�N�V����UI)
//--------------------------------------------
class CActionUI : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�|���S��)
	typedef enum
	{
		POLY_GROUND = 0,		// �w�i
		POLY_ACT,				// �A�N�V������
		POLY_MAX				// ���̗񋓌^�̑���
	}POLY;

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_WALLKICK = 0,		// �ǃL�b�N
		TYPE_LANDTURBO,			// ���n�^�[�{
		TYPE_SUPERJUMP,			// ���W�����v
		TYPE_LAMPJUMP,			// �X���W�����v
		TYPE_ATTACK,			// �U��
		TYPE_MAX				// ���̗񋓌^�̑���
	}TYPE;

	// �񋓌^��`(���)
	typedef enum
	{
		STATE_APPEAR = 0,		// �o�����
		STATE_DISP,				// �\�����
		STATE_SMALLER,			// �k�����
		STATE_MAX				// ���̗񋓌^�̑���
	}STATE;

	CActionUI();				// �R���X�g���N�^
	~CActionUI();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �I������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	void SetData(const TYPE type, const int nScore);	// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CActionUI* Create(const TYPE type, const int nScore);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SizeChange(const int nCnt);	// �T�C�Y�ω�����
	void AppearChange(const int nCnt);	// �o����ԕω�����
	void DispChange(void);				// �\����ԕω�����
	void Rotation(const int nCnt);		// �����ω�����

	void GroundBound(void);				// �w�i�̃o�E���h����
	void GroundChange(void);			// �w�i�̏�ԕω�����
	void GroundDispProcess(void);		// �w�i�̕\����Ԏ�����

	// �����o�ϐ�
	CObject2D* m_apActUI[POLY_MAX];		// �A�N�V����UI�̏��
	D3DXVECTOR3 m_sizeDest;				// �ڕW�̃T�C�Y
	D3DXVECTOR3 m_GroundMove;			// �w�i�̃T�C�Y�̈ړ���
	STATE m_state;						// ���
	int m_nStateCount;					// ��ԃJ�E���g
	int m_nScore;						// ���_

	// �ÓI�����o�ϐ�
	static const char* m_apTexturename[TYPE_MAX];	// �e�N�X�`���̃p�X��
	static const D3DXVECTOR3 m_aSize[TYPE_MAX];		// ���ꂼ��̑傫��
};

#endif