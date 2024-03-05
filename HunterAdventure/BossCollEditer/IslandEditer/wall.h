//===================================
//
// �ǃw�b�_�[[wall.h]
// Author ��������
//
//===================================
#ifndef _WALL_H_
#define _WALL_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "model.h"
#include "list_manager.h"

//-----------------------------------
// �N���X��`(��)
//-----------------------------------
class CWall : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_NORMAL = 0,	// �ʏ���
		TYPE_NARROW,		// �ׂ����
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �񋓌^��`(���)
	enum ROTTYPE
	{
		ROTTYPE_FRONT = 0,	// �O����
		ROTTYPE_RIGHT,		// �E����
		ROTTYPE_BACK,		// ������
		ROTTYPE_LEFT,		// ������
		ROTTYPE_MAX			// ���̗񋓌^�̑���
	};

	CWall();				// �R���X�g���N�^
	~CWall();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype);				// ���̐ݒ菈��

	// �Z�b�g�E�Q�b�g�֌W
	D3DXVECTOR3 GetVtxMax(void) const;		// ���_�̍ő�l�̎擾����
	D3DXVECTOR3 GetVtxMin(void) const;		// ���_�̍ŏ��l�̎擾����
	TYPE GetType(void) const;				// ��ނ̎擾����
	ROTTYPE GetRotType(void) const;			// �����̎�ނ̎擾����

	// �ÓI�����o�֐�
	static CWall* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, const TYPE type, const ROTTYPE rottype);		// ��������
	static CListManager<CWall*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void SetVertex(void);		// ���_�̐ݒ菈��

	// �����o�ϐ�
	D3DXVECTOR3 m_vtxMax;		// ���_�̍ő�l
	D3DXVECTOR3 m_vtxMin;		// ���_�̍ŏ��l
	TYPE m_type;				// ���
	ROTTYPE m_rottype;			// �����̎��

	// �ÓI�����o�ϐ�
	static CListManager<CWall*> m_list;		// ���X�g
};

#endif