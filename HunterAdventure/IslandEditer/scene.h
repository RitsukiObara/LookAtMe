//============================================
//
// �V�[���w�b�_�[[scene.h]
// Author�F��������
//
//============================================
#ifndef _SCENE_H_			//���̃}�N����`������Ă��Ȃ�������
#define _SCENE_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �V�[���N���X
//--------------------------------------------
class CScene
{
public:

	// �񋓌^��`(���[�h)
	enum MODE
	{
		MODE_LOGO = 0,			// ���S���
		MODE_TITLE,				// �^�C�g�����
		MODE_TUTORIAL,			// �`���[�g���A�����
		MODE_GAME,				// �Q�[�����
		MODE_RESULT,			// ���U���g���
		MODE_RANKING,			// �����L���O���
		MODE_MAX				// ���̗񋓌^�̑���
	};

	CScene();					// �R���X�g���N�^
	virtual ~CScene();			// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void);			// ����������
	virtual void Uninit(void);			// �I������
	virtual void Update(void);			// �X�V����
	virtual void Draw(void);			// �`�揈��

	void SetData(const MODE mode);		// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetMode(const MODE mode);		// ���[�h�̐ݒ菈��
	MODE GetMode(void) const;			// ���[�h�̎擾����

	// �ÓI�����o�֐�
	static CScene* Create(const MODE mode);		// ��������

private:

	// �����o�ϐ�
	MODE m_mode;				// ���[�h
};

#endif