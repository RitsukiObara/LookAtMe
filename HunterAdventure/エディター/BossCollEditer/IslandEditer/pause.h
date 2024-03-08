//===========================================
//
// �|�[�Y�w�b�_�[[pause.h]
// Author ��������
//
//===========================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//-------------------------------------------
// �O���錾
//-------------------------------------------
class CObject2D;

//-------------------------------------------
// �N���X��`(�|�[�Y)
//-------------------------------------------
class CPause
{
public:		// �N�ł��A�N�Z�X�ł���

	// �|�[�Y��ʂ̍\�����̒�`
	enum MENU
	{
		MENU_CONTINUE = 0,	// �R���e�j���[
		MENU_RESET,			// ���g���C
		MENU_TITLE,			// �^�C�g���ɖ߂�
		MENU_MAX
	};

	// �|�[�Y��ʂ̃o�b�t�@�̒�`
	enum POLYGON
	{
		POLYGON_BACK = 0,	// �^�����|���S��
		POLYGON_MENU,		// ���j���[
		POLYGON_CONTINUE,	// �R���e�j���[
		POLYGON_RESET,		// ���Z�b�g
		POLYGON_TITLE,		// �^�C�g��
		POLYGON_MAX
	};

	CPause();				// �R���X�g���N�^
	~CPause();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	// �Z�b�g�E�Q�b�g�֌W
	void ChangeDisp(void);			// �`��󋵂̐؂�ւ�����

	// �ÓI�����o�֐�
	static CPause* Create(void);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SizeCorrect(int nIdx);		// �T�C�Y�␳����
	void PauseSelect(void);			// �|�[�Y�̑I������
	bool PauseDecide(void);			// �|�[�Y�̌��菈��
	void PauseAlphaCorrect(void);	// �|�[�Y�̓����x�␳����
	void PauseVertex(void);			// �|�[�Y�̒��_�ݒ菈��

	// �����o�ϐ�
	D3DXCOLOR m_PauseColor;					// �I�𒆂̑I�����̐F
	D3DXVECTOR3 sizeDest;					// �ڕW�̃T�C�Y
	CObject2D* m_apObject[POLYGON_MAX];		// �|���S���̏��
	MENU m_PauseMenu;						// �|�[�Y���j���[
	int m_nPauseCounter;					// �J�E���^�[
	float m_fPauseAlpha;					// �����x��ω�������ϐ�
	bool m_bDisp;							// �`���
};

#endif