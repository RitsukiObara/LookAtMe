//=====================================================================================
//
//	�O�Ճw�b�_�[ [orbit.h]
//	Author�F��������
//
//=====================================================================================
#ifndef _ORBIT_H_			// ���̃}�N����`������Ă��Ȃ��ꍇ
#define _ORBIT_H_			// ��d�C���N���[�h�h�~�̃}�N�����`����

//*************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************
#include "object.h"

//-------------------------------------------------------------------------------------
//	�}�N����`
//-------------------------------------------------------------------------------------
#define MAX_ORBIT_VTX		(32)			// �O�Ղ̍ō����_��

//-------------------------------------------------------------------------------------
// �N���X��`(�O��)
//-------------------------------------------------------------------------------------
class COrbit : CObject
{
public:			// �N�ł��A�N�Z�X�ł���

	//-------------------------------------------------------------------------------------
	//	�񋓌^��` (MATRIXPLACE)
	//-------------------------------------------------------------------------------------
	typedef enum
	{
		MATRIXPLACE_BOTTOM = 0,		// ��
		MATRIXPLACE_TOP,			// ��
		MATRIXPLACE_MAX				// ���̗񋓌^�̑���
	}MATRIXPLACE;

	COrbit();						// �R���X�g���N�^
	virtual ~COrbit();				// �f�X�g���N�^

	// �����o�֐�
	virtual HRESULT Init(void) override;	// ����������
	virtual void Uninit(void) override;		// �I������
	virtual void Update(void) override;		// �X�V����
	virtual void Draw(void) override;		// �`�揈��

	void SetData(D3DXMATRIX* mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const int nTexIdx, const TYPE type);				// ���̐ݒ菈��
	void SetData(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const char* pTextureName, const TYPE type);		// ���̐ݒ菈��
	void BindTexture(int nIdx);		// �e�N�X�`���̊��蓖�ď���

	void PosReset(void);			// �ʒu�̃��Z�b�g����

	// �ÓI�����o�֐�
	static COrbit* Create(D3DXMATRIX* mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const int nTexIdx, const TYPE type = TYPE_ORBIT);			// ��������(�e�N�X�`���ԍ�)
	static COrbit* Create(D3DXMATRIX *mtx, const D3DXVECTOR3& pos, const D3DXVECTOR3& OffSet1, const D3DXVECTOR3& OffSet2, const char* pTextureName, const TYPE type = TYPE_ORBIT);		// ��������(�e�N�X�`���̖��O)

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX* m_pMtxParent;						// �e�̃}�g���b�N�X�ւ̃|�C���^
	D3DXVECTOR3 m_aOffset[MATRIXPLACE_MAX];			// ���[�̃I�t�Z�b�g
	D3DXCOLOR m_aCol[MATRIXPLACE_MAX];				// ���[�̊�̐F
	D3DXMATRIX m_aMtxWorldPoint[MATRIXPLACE_MAX];	// ���[�̃��[���h�}�g���b�N�X
	D3DXVECTOR3 m_aPosPoint[MAX_ORBIT_VTX];			// �v�Z��̊e���_���W
	D3DXCOLOR m_aColPoint[MAX_ORBIT_VTX];			// �v�Z��̊e���_�J���[
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X
};

#endif