//============================================
//
// �����^�C�~���O�\���w�b�_�[[push_timing.h]
// Author�F��������
//
//============================================
#ifndef _PUSH_TIMING_H_			//���̃}�N����`������Ă��Ȃ�������
#define _PUSH_TIMING_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "objectbillboard.h"

//--------------------------------------------
// �N���X(�����^�C�~���O�N���X)
//--------------------------------------------
class CPushTiming : public CBillboardAnim
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_PAD_A = 0,		// A�{�^��
		TYPE_KEY_SPACE,		// SPACE�L�[
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CPushTiming();			// �R���X�g���N�^
	~CPushTiming();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const TYPE type);					// �ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	void SetType(const TYPE type);		// ��ނ̐ݒ菈��

	// �ÓI�����o�֐�
	static CPushTiming* Create(const D3DXVECTOR3& pos, const TYPE type);	// ��������

private:		// ���������A�N�Z�X�ł���

};

#endif