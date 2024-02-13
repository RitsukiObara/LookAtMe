#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
// �w�i�G�f�B�b�g�w�b�_�[[bgEdit.h]
// Author ��������
//
//===================================
#ifndef _BGEDIT_H_
#define _BGEDIT_H_

#include "main.h"
#include "model.h"

//------------------------------------------
// �N���X��`(�G�f�B�b�g)
//------------------------------------------
class CBgEdit : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CBgEdit();			// �R���X�g���N�^
	~CBgEdit();			// �f�X�g���N�^

	//�v���g�^�C�v�錾
	HRESULT Init(void);		// �I�u�W�F�N�g�̏���������
	void Uninit(void);		// �I�u�W�F�N�g�̏I������
	void Update(void);		// �I�u�W�F�N�g�̍X�V����
	void Draw(void);		// �I�u�W�F�N�g�̕`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile fileData);		// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CBgEdit* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile fileData);	// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void TypeChange(void);		// ��ޕύX����
	void Move(void);			// �ړ�����
	void Rotation(void);		// ��]����
	void Set(void);				// �I�u�W�F�N�g�̐ݒ菈��
	void Delete(void);			// �I�u�W�F�N�g�̏���
	void ScaleX(void);			// �I�u�W�F�N�g�̊g��k������(X��)
	void ScaleY(void);			// �I�u�W�F�N�g�̊g��k������(Y��)
	void Scale(void);			// �I�u�W�F�N�g�̊g��k������
	void Reset(void);			// �I�u�W�F�N�g�̏�񃊃Z�b�g����
	void MaterialCustom(void);	// �}�e���A���̃G�f�B�b�g����
	void RightAngle(void);		// �I�u�W�F�N�g�̒��p����

	// �����o�ϐ�
	CXFile::TYPE m_type;							// ���
	D3DXMATERIAL m_MatCopy[MAX_MATERIAL];			// �}�e���A���̃R�s�[
	D3DXMATERIAL m_EditMaterial[MAX_MATERIAL];		// �J�X�^���p�̃}�e���A�����
	int m_nCntMaterial;								// �}�e���A���̔ԍ�
	int m_nColorCount;								// �F��ς���Ƃ��̃J�E���g
};

#endif

#endif