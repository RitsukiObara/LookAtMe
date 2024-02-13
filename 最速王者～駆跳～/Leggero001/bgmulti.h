//============================================
//
// ���d�w�i�w�b�_�[[bgmulti.h]
// Author�F��������
//
//============================================
#ifndef _BGMULTI_H_			//���̃}�N����`������Ă��Ȃ�������
#define _BGMULTI_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object2D.h"
#include "scrollobject.h"

//--------------------------------------------
// �N���X(���d�w�i�N���X)
//--------------------------------------------
class CBgMulti : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// �w�i�̎��
	typedef enum
	{
		TYPE_BACK = 0,		// �w�i
		TYPE_LIGHT,			// ��
		TYPE_STAR,			// ����
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	CBgMulti();				// �R���X�g���N�^
	~CBgMulti();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	//�@�Z�b�g�E�Q�b�g�֌W
	CScroll* GetBgMulti(int nIdx) const;	// ���d�w�i�̎擾����

	void SetPos(const D3DXVECTOR3& pos);	// �ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPos(void) const;			// �ʒu�̎擾����

	// �ÓI�����o�֐�
	static CBgMulti* Create(void);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CScroll *m_apBgMulti[TYPE_MAX];							// ���d�w�i�̏��

	// �ÓI�����o�ϐ�
	static const char *m_apTextureName[TYPE_MAX];			// �e�N�X�`���̖��O
	static const D3DXVECTOR2 m_aSpeedInfo[TYPE_MAX];		// �X�s�[�h�̏��
};

#endif