//===================================
//
// �G�f�B�b�g�w�b�_�[[edit.h]
// Author ��������
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "object.h"
#include "enemy.h"
#include "tree.h"
#include "rock.h"
#include "wall.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CModel;			// ���f��

// �N���X��`(����)
class CProcess
{
public:

	CProcess();
	virtual ~CProcess();

	virtual void Process(CModel* m_pModel) = 0;		// ����
	virtual void Delete(CModel* m_pModel) = 0;		// ����
};

//-----------------------------------
// �N���X��`(�G�f�B�b�g)
//-----------------------------------
class CEdit : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�X�^�C��)
	enum STYLE
	{
		STYLE_ENEMY = 0,		// �G
		STYLE_COIN,				// �R�C��
		STYLE_GOLDBONE,			// ���̍�
		STYLE_TREE,				// ��
		STYLE_ROCK,				// ��
		STYLE_BLOCK,			// �u���b�N
		STYLE_BANGFLOWER,		// ���e��
		STYLE_WALL,				// ��
		STYLE_GRASS,			// ��
		STYLE_LAKE,				// ��
		STYLE_MAX				// ���̗񋓌^�̑���
	};

	CEdit();					// �R���X�g���N�^
	~CEdit();					// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;	// ����������
	void Uninit(void) override;		// �I������
	void Update(void) override;		// �X�V����
	void Draw(void) override;		// �`�揈��

	void SetData(void);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CEdit* Create(void);		// ��������

private:		// ���������A�N�Z�X�ł���
	
	// �����o�ϐ�
	CModel* m_pModel;		// ���f���̏��
	CProcess* m_pProcess;	// �����̏��
	STYLE m_style;			// �X�^�C��
};

// �N���X��`(�G�̏���)
class CEnemyProcess : public CProcess
{
public:
	CEnemyProcess();
	~CEnemyProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	CEnemy::TYPE m_type;		// �G�̎��
};

// �N���X��`(�R�C���̏���)
class CCoinProcess : public CProcess
{
public:
	CCoinProcess();
	~CCoinProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// �N���X��`(���̍��̏���)
class CGoldBoneProcess : public CProcess
{
public:
	CGoldBoneProcess();
	~CGoldBoneProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// �N���X��`(�؂̏���)
class CTreeProcess : public CProcess
{
public:
	CTreeProcess();
	~CTreeProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	CTree::TYPE m_type;		// �؂̎��
};

// �N���X��`(��̏���)
class CRockProcess : public CProcess
{
public:
	CRockProcess();
	~CRockProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	CRock::TYPE m_type;		// �؂̎��
};

// �N���X��`(�u���b�N�̏���)
class CBlockProcess : public CProcess
{
public:
	CBlockProcess();
	~CBlockProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// �N���X��`(���e�Ԃ̏���)
class CBangFlowerProcess : public CProcess
{
public:
	CBangFlowerProcess();
	~CBangFlowerProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// �N���X��`(�ǂ̏���)
class CWallProcess : public CProcess
{
public:
	CWallProcess();
	~CWallProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	// �����o�ϐ�
	CWall::TYPE m_type;			// ���
	CWall::ROTTYPE m_rottype;	// �����̎��
};

#endif