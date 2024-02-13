#ifdef _DEBUG	// �f�o�b�O����
//===================================
//
// �G�f�B�b�g�w�b�_�[[EditObject.h]
// Author ��������
//
//===================================
#ifndef _EDITOBJECT_H_
#define _EDITOBJECT_H_

#include "main.h"
#include "block.h"

//------------------------------------------
// �N���X��`(�G�f�B�b�g)
//------------------------------------------
class CEdit : public CBlock
{
public:			// �N�ł��A�N�Z�X�ł���

	CEdit();			// �R���X�g���N�^
	~CEdit();			// �f�X�g���N�^

	//�v���g�^�C�v�錾
	HRESULT Init(void);		// �I�u�W�F�N�g�̏���������
	void Uninit(void);		// �I�u�W�F�N�g�̏I������
	void Update(void);		// �I�u�W�F�N�g�̍X�V����
	void Draw(void);		// �I�u�W�F�N�g�̕`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);		// ����

	// �ÓI�����o�֐�
	static CEdit* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);	// ��������

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
	void PlayerPosSet(void);	// �I�u�W�F�N�g�̃v���C���[�ʒu�ړ�
	void CliffChange(void);		// �R�߂܂�̕ω�����

	// �����o�ϐ�
	D3DXMATERIAL m_MatCopy[MAX_MATERIAL];					// �}�e���A���̃R�s�[
	D3DXMATERIAL m_EditMaterial[MAX_MATERIAL];				// �J�X�^���p�̃}�e���A�����
	int m_nCntMaterial;										// �}�e���A���̔ԍ�
	int m_nColorCount;										// �F��ς���Ƃ��̃J�E���g
};

#endif

#endif