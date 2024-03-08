//======================================================================================================================
//
// ���b�V���w�b�_�[ [mesh.h]
// Author�F��������
//
//======================================================================================================================
#ifndef _MESH_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _MESH_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//**********************************************************************************************************************
// �C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "object.h"
#include "list_manager.h"

//----------------------------------------------------------------------------------------------------------------------
// �N���X��`(���b�V��)
//----------------------------------------------------------------------------------------------------------------------
class CMesh : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// �\���̒�`(������)
	struct SGrid
	{
		int nWidth;			// ���̕�����
		int nHeight;		// �c�̕�����
	};

	// �񋓌^��`(���)
	enum TYPE
	{
		TYPE_FIELD = 0,		// �t�B�[���h
		TYPE_WALL,			// ��
		TYPE_CYLINDER,		// ��
		TYPE_DOME,			// �h�[��
		TYPE_DONUT,			// �h�[�i�c
		TYPE_TORNADO,		// �g���l�[�h
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	CMesh(CObject::TYPE type, CObject::PRIORITY priority = PRIORITY_BG);			// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CMesh();	// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void DrawProcess(void);			// �`�揈���̒��g

	// ���_�E�C���f�b�N�X�̐ݒ�֌W
	void SetIndex(const int nNumVtx1, const int nNumVtx2);		// �C���f�b�N�X�̐ݒ菈��
	void SetColor(const D3DXCOLOR& col);						// ���_�J���[�̐ݒ菈��

	void BindTexture(int nIdx);		// �e�N�X�`���̊��蓖�ď���

	// �Z�b�g�E�Q�b�g�֌W
	void SetPos(const D3DXVECTOR3& pos);	// �ʒu�̐ݒ菈��
	D3DXVECTOR3 GetPos(void) const;			// �ʒu�̎擾����

	void SetRot(const D3DXVECTOR3& rot);	// �����̐ݒ菈��
	D3DXVECTOR3 GetRot(void) const;			// �����̎擾����

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) const;		// ���_�o�b�t�@�̃|�C���^�̎擾����

	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void) const;		// �C���f�b�N�X�o�b�t�@�̃|�C���^�̎擾����

	void SetType(const TYPE type);			// ��ނ̐ݒ菈��
	TYPE GetType(void) const;				// ��ނ̎擾����

	void SetVtx(const SGrid vtx);			// ���_���̐ݒ菈��
	SGrid GetVtx(void) const;				// ���_���̎擾����

	void SetDivi(const SGrid divi);			// �������̐ݒ菈��
	SGrid GetDivi(void) const;				// �������̎擾����

	void SetNumVtx(void);					// �����_���̐ݒ菈��
	int GetNumVtx(void);					// �����_���̎擾����

	void SetNumIdx(void);					// ���C���f�b�N�X���̐ݒ菈��
	int GetNumIdx(void);					// ���C���f�b�N�X���̎擾����

	void SetLighting(const bool bLighting);	// ���C�e�B���O�̐ݒ菈��

	void SetCulling(const bool bCulling);	// �J�����O�̐ݒ菈��

	// �ÓI�����o�ϐ�
	static void TxtSet(void);				// �e�L�X�g�ǂݍ��ݏ���
	static int TxtField(FILE* pFile);		// �t�B�[���h�̓ǂݍ��ݏ���
	static int TxtWall(FILE* pFile);		// �E�H�[���̓ǂݍ��ݏ���
	static int TxtCylinder(FILE* pFile);	// �V�����_�[�̓ǂݍ��ݏ���
	static int TxtDome(FILE* pFile);		// �h�[���̓ǂݍ��ݏ���
	static int TxtDonut(FILE* pFile);		// �h�[�i�c�̓ǂݍ��ݏ���
	static int TxtTornado(FILE* pFile);		// �g���l�[�h�̓ǂݍ��ݏ���
	static int TxtSphere(FILE* pFile);		// �X�t�B�A�̓ǂݍ��ݏ���

	static CListManager<CMesh*> GetList(void);			// ���X�g�̎擾����

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	D3DXVECTOR3	m_pos;			// �ʒu
	D3DXVECTOR3	m_rot;			// ����
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	TYPE	m_type;				// ���
	SGrid	m_vtx;				// ���_��
	SGrid	m_divi;				// ������
	int		m_nNumVtx;			// �����_��
	int		m_nNumIdx;			// ���C���f�b�N�X��
	int		m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X
	bool	m_bLightOff;		// ���C�e�B���O��
	bool	m_bCullOff;			// �J�����O��

	// �ÓI�����o�ϐ�
	static CListManager<CMesh*> m_list;		// ���X�g
};


#endif