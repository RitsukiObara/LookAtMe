//============================================
//
// �w�i���f���w�b�_�[[bgmodel.h]
// Author�F��������
//
//============================================
#ifndef _BGMODEL_H_			//���̃}�N����`������Ă��Ȃ�������
#define _BGMODEL_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"
#include "objectX.h"

//--------------------------------------------
// �N���X(�G�N���X)
//--------------------------------------------
class CBgModel : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBgModel();				// �R���X�g���N�^
	~CBgModel();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBgModel* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const D3DXMATERIAL* pMat, const CXFile::TYPE type);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�ϐ�
	CXFile::TYPE m_type;				// ���
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	// �}�g���b�N�X�ւ̃|�C���^
};

#endif