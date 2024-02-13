//============================================
//
// �c�苗�����m�w�b�_�[[announce.h]
// Author�F��������
//
//============================================
#ifndef _ANNOUNCE_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _ANNOUNCE_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"

//--------------------------------------------
// �O���錾
//--------------------------------------------
class CObject2D;		// 2D�|���S��
class CAnim;			// �A�j���[�V����

//--------------------------------------------
// �N���X��`(���m)
//--------------------------------------------
class CAnnounce : public CObject
{
public:				// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�|���S���̎��)
	typedef enum
	{
		TEX_ROAD = 0,			// ���̂�
		TEX_GOAL,				// �S�[��
		TEX_PLAYER,				// �v���C���[
		TEX_MAX					// ���̗񋓌^�̑���
	}TEX;

	// �񋓌^��`(�|���S���̎��)
	typedef enum
	{
		POLY_ROAD = 0,			// ���̂�
		POLY_GOAL,				// �S�[��
		POLY_MAX				// ���̗񋓌^�̑���
	}POLY;

	CAnnounce();				// �R���X�g���N�^
	~CAnnounce();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(void);		// ���̐ݒ菈��

	// �ÓI�����o�ϐ�
	static CAnnounce* Create(void);		// ��������

private:			// ���������A�N�Z�X�ł���

	// �����o�֐�
	void DistanceMeasure(void);			// �������菈��
	void PlayerMove(void);				// �v���C���[�̈ړ�����

	// �����o�ϐ�
	CObject2D* m_apObject2D[POLY_MAX];	// �ԍ��̏��
	CAnim* m_pPlayer;					// �v���C���[
	float m_fTotalDist;		// �����̋���
	int m_nDist;			// ����

	// �ÓI�����o�ϐ�
	static const char* c_apTextureName[TEX_MAX];	// �e�N�X�`���̖��O
};

#endif