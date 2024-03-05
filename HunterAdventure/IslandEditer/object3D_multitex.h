//===================================
//
// �}���`�e�N�X�`���̃I�u�W�F�N�g3D�w�b�_�[[object3D_multitex.h]
// Author ��������
//
//===================================
#ifndef _OBJECT3D_MULTITEX_H_
#define _OBJECT3D_MULTITEX_H_

#include "object.h"
#include "manager.h"
#include "renderer.h"

//=================================================================================================================================================
// ���ꂼ��̕`����@
//=================================================================================================================================================
class CTextureStyle
{
public:
	virtual ~CTextureStyle() = default;		// �f�X�g���N�^
	virtual void DispStart(void) = 0;		// �`�揉�߂̏���

	// �`��I���̏���
	void DispEnd(void)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� (�e�N�X�`���̃A���t�@�u�����h�̐ݒ�)
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// �N���X(�}���`�e�N�X�`��3D�|���S���N���X)
//--------------------------------------------
class CObject3DMultiTex : public CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	// �񋓌^��`(�`��̎��)
	enum TYPE
	{
		TYPE_NONE = 0,					// �����
		TYPE_PULL_OUT_WHITE,			// �^����(�w�i��)
		TYPE_BACKGROUND_WHITE,			// �w�i�؂蔲��(�w�i��)
		TYPE_PULL_OUT_BLACK,			// �^����(�w�i��)
		TYPE_BACKGROUND_BLACK,			// �w�i�؂蔲��(�w�i��)
		TYPE_LIGHT_BLACK,				// �P���e�N�X�`��(���w�i)
		TYPE_NEGA_BLACK,				// �l�K�e�N�X�`��(���w�i)
		TYPE_MONO_TRANS,				// �����w�i(������) 
		TYPE_PULL_OUT_SP,				// �^����(���S����)
		TYPE_LIGHT_PULL_OUT,			// �^�������P���e�N�X�`��
		TYPE_LIGHT_SP,					// �P���e�N�X�`��(���S����)
		TYPE_BACKGROUND_SP,				// �w�i�؂蔲��(���S����)
		TYPE_NEGA_SP,					// �l�K�e�N�X�`��(���S����)
		TYPE_BACKGROUND_NEGA,			// �l�K�e�N�X�`��(�w�i�؂蔲��)
		TYPE_FUSION,					// ���̏���
		TYPE_MAX						// ���̗񋓌^�̑���
	};

	CObject3DMultiTex(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CObject3DMultiTex();			// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void DrawLightOff(void);				// �`�揈��(���C�e�B���O����)

	void BindTexture(const int nIdx, const int nCnt);	// �e�N�X�`���̊��蓖�ď���

	// ���_����n
	void SetVertex(void);									// ���_���̐ݒ�
	void SetVtxColor(const D3DXCOLOR& col);					// ���_�J���[�̐ݒ菈��
	void SetVtxTextureWidth(const D3DXVECTOR2& size, int nCnt = 0);		// ���_�e�N�X�`���̐ݒ菈��(�c�̈��Ԋu�̃e�N�X�`���z��)
	void SetVtxTextureHeight(const D3DXVECTOR2& size, int nCnt = 0);	// ���_�e�N�X�`���̐ݒ菈��(���̈��Ԋu�̃e�N�X�`���z��)
	void SetVtxTextureAnim(const D3DXVECTOR2& anim, int nCnt = 0);		// ���_�e�N�X�`���̐ݒ菈��(�A�j���[�V������)

	// �Z�b�g�E�Q�b�g�֌W
	void SetPos(const D3DXVECTOR3& pos);	// �ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) const;			// �ʒu�擾����

	void SetPosOld(const D3DXVECTOR3& pos);	// �O��̈ʒu�ݒ菈��
	D3DXVECTOR3 GetPosOld(void) const;		// �O��̈ʒu�擾����

	void SetRot(const D3DXVECTOR3& rot);	// �����ݒ菈��
	D3DXVECTOR3 GetRot(void) const;			// �����擾����

	void SetSize(const D3DXVECTOR3& size);	// �T�C�Y�ݒ菈��
	D3DXVECTOR3 GetSize(void) const;		// �T�C�Y�擾����

	D3DXMATRIX GetMatrix(void) const;		// �}�g���b�N�X�̎擾����
	D3DXMATRIX* GetMatrixPoint(void);		// �}�g���b�N�X�|�C���^�̎擾����

	void SetType(const TYPE type);			// ��ނ̐ݒ菈��

	int GetTexIdx(const int nCnt) const;	// �e�N�X�`���̃C���f�b�N�X�̎擾����

	// �ÓI�����o�֐�
	static CObject3DMultiTex* Create(const CObject::TYPE type, const PRIORITY priority, const TYPE typeTex = TYPE_NONE);		// ��������

private:

	// �����o�ϐ�
	CTextureStyle* m_pTexStyle;					// �e�N�X�`���`��̏��
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_posOld;						// �O��̈ʒu
	D3DXVECTOR3 m_rot;							// ����
	D3DXVECTOR3 m_size;							// �T�C�Y
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�̃|�C���^
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X
	int m_aTexIdx[2];							// �e�N�X�`���̃C���f�b�N�X
};

//--------------------------------------------
// �N���X��`(�^�����w�i��)
//--------------------------------------------
class CPulloutWhite : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_LERP);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// �N���X��`(�w�i�؂蔲��(�w�i��))
//--------------------------------------------
class CBackGroundWhite : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// �N���X��`(�^�����w�i��)
//--------------------------------------------
class CPulloutBlack : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// �N���X��`(�w�i�؂蔲��(�w�i��))
//--------------------------------------------
class CBackGroundBlack : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// �N���X��`(�P���e�N�X�`��(���w�i))
//--------------------------------------------
class CLightBlack : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// �N���X��`(�l�K�e�N�X�`��(���w�i))
//--------------------------------------------
class CNegaBlack : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// �N���X��`(�����w�i(������))
//--------------------------------------------
class CMonoTrans : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// �N���X��`(�^����(���S����))
//--------------------------------------------
class CPulloutSpecial : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// �N���X��`(�^�������P���e�N�X�`��)
//--------------------------------------------
class CLightPullout : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_PREMODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// �N���X��`(�P���e�N�X�`��(���S����))
//--------------------------------------------
class CLightSpecial : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
};

//--------------------------------------------
// �N���X��`(�w�i�؂蔲��(���S����))
//--------------------------------------------
class CBackGroundSpecial : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEMP);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_LERP);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	}
};

//--------------------------------------------
// �N���X��`(�l�K�e�N�X�`��(���S����))
//--------------------------------------------
class CNegaSpecial : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// �N���X��`(�l�K�e�N�X�`��(�w�i�؂蔲��))
//--------------------------------------------
class CBackGroundNega : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEMP);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

//--------------------------------------------
// �N���X��`(����)
//--------------------------------------------
class CFusion : public CTextureStyle
{
public:
	// �`�揉�߂̏���
	void DispStart(void) override
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

		// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
		pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	}
};

#endif