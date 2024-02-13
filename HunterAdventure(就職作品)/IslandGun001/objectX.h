//============================================================
//
// X�t�@�C���w�b�_�[ [xfile.h]
// Author�F��������
//
//============================================================
#ifndef _XFILE_H_	// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _XFILE_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//------------------------------------------------------------
// �}�N����`
//------------------------------------------------------------
#define INIT_VTX_MIN	(D3DXVECTOR3( 9999.0f,  9999.0f,  9999.0f))		// ���f���̍ŏ��̒��_���W�̏����l
#define INIT_VTX_MAX	(D3DXVECTOR3(-9999.0f, -9999.0f, -9999.0f))		// ���f���̍ő�̒��_���W�̏����l
#define INIT_SIZE		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))					// ���f���T�C�Y�̏����l
#define MAX_XFILE		(1024)											// �ǂݍ��߂郂�f���̍ő吔

//------------------------------------------------------------
// �N���X��`(X�t�@�C��)
//------------------------------------------------------------
class CXFile
{
public:			// �N�ł��A�N�Z�X�ł���

	//************************************************************
	//	�\���̒�` (SXFile)
	//************************************************************
	struct SXFile
	{
		LPD3DXMESH	 pMesh;						// ���b�V�� (���_���) �ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;					// �}�e���A���ւ̃|�C���^
		DWORD		 dwNumMat;					// �}�e���A���̐�
		D3DXVECTOR3	 vtxMin;					// �ŏ��̒��_���W
		D3DXVECTOR3	 vtxMax;					// �ő�̒��_���W
		D3DXVECTOR3	 collsize;					// �����蔻��̃T�C�Y
		D3DXVECTOR3* vtxPos;					// ���_�̈ʒu
		float		 fRadius;					// ���a
		int			 nNumFace;					// �ʂ̐�
		int			 m_nTexIdx[MAX_MATERIAL];	// �e�N�X�`���̃C���f�b�N�X
		bool		 bEmpty;					// �󔒏�
	};

	CXFile();			// �R���X�g���N�^
	~CXFile();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// X�t�@�C���̏���������
	void Unload(void);		// X�t�@�C���̏I������

	SXFile Regist(const char* filename);		// X�t�@�C���̓o�^����
	HRESULT Load(void);		// ���[�h����

	// �ÓI�����o�֐�
	static CXFile* Create(void);		// ��������

private:

	// �����o�֐�
	HRESULT LoadXFile(const char* filename, SXFile* pXFile);	// x�t�@�C���̓ǂݍ���
	void SetCollision(SXFile* pXFile);			// �����蔻��̍쐬
	HRESULT LoadTexture(SXFile* pXFile);		// �e�N�X�`���̓ǂݍ���

	// �����o�ϐ�
	SXFile m_apModel[MAX_XFILE];						// ���f���̏��
	char m_apModelName[MAX_XFILE][MAX_STRING];	// ���f���̖��O
};

#endif