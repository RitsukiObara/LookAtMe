//===================================
//
// �`�B���b�Z�[�W�w�b�_�[[speech_message.h]
// Author ��������
//
//===================================
#ifndef _SPEECH_MESSAGE_H_
#define _SPEECH_MESSAGE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "objectbillboard.h"

//-----------------------------------
// �N���X��`(�`�B���b�Z�[�W)
//-----------------------------------
class CSpeechMessage : public CBillboard
{
public:

	// �񋓌^��`(���)
	enum STATE
	{
		STATE_EXTEND = 0,	// �L�я��
		STATE_SHRINK,		// �k�ݏ��
		STATE_MAX			// ���̐�̏������s��Ȃ�
	};

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_HELP = 0,		// �~���v��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CSpeechMessage();		// �R���X�g���N�^
	~CSpeechMessage();		// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nLife, const TYPE type);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CSpeechMessage* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, const int nLife, const TYPE type);	// ��������

private:

	// �����o�ϐ�
	D3DXVECTOR3 m_sizeDest;	// �ړI�̃T�C�Y
	D3DXVECTOR3 m_sizeCopy;	// �ŏ��̃T�C�Y
	STATE m_state;			// ���
	TYPE m_type;			// ���
	int m_nLife;			// ����
	int m_nStateCount;		// ��Ԃ̃J�E���g
};

#endif