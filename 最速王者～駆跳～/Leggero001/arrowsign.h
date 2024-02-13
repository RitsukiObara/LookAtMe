//===================================
//
// ���Ŕw�b�_�[[arrowsign.h]
// Author ��������
//
//===================================
#ifndef _ARROWSIGN_H_
#define _ARROWSIGN_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CModel;			// ���f��

//-----------------------------------
// �N���X��`(���Ŕ�)
//-----------------------------------
class CArrowSign : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	typedef enum
	{
		TYPE_POLE = 0,		// �|�[��
		TYPE_ARROW,			// ���
		TYPE_MAX			// ���̗񋓌^�̑���
	}TYPE;

	CArrowSign();															// �R���X�g���N�^
	CArrowSign(CObject::TYPE type, PRIORITY priority = PRIORITY_BLOCK);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CArrowSign();													// �f�X�g���N�^
	void Box(void);															// �R���X�g���N�^�̔�

	// �����o�֐�
	virtual HRESULT Init(void);		// ����������
	virtual void Uninit(void);		// �I������
	virtual void Update(void);		// �X�V����
	virtual void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CArrowSign* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SetDataPole(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// ���̏��̐ݒ菈��
	void SetDataArrow(const D3DXVECTOR3& rot);										// ���̏��̐ݒ菈��

	// �����o�ϐ�
	CModel* m_apArrow[TYPE_MAX];		// ���f���̏��
};

#endif