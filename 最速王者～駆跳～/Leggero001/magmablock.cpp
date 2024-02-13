//===========================================
//
// �}�O�}�u���b�N�̃��C������[magmablock.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "magmablock.h"

//==============================
// �R���X�g���N�^
//==============================
CMagmaBlock::CMagmaBlock() : CBlock(CObject::TYPE_BLOCK, CObject::PRIORITY_BLOCK)
{

}

//==============================
// �f�X�g���N�^
//==============================
CMagmaBlock::~CMagmaBlock()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CMagmaBlock::Init(void)
{
	// �t�@�C���̐ݒ菈��
	SetFileData(CXFile::TYPE_BLOCK_MAGMA);

	if (FAILED(CBlock::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CMagmaBlock::Uninit(void)
{
	// �I������
	CBlock::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CMagmaBlock::Update(void)
{
	// �X�V����
	CBlock::Update();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CMagmaBlock::Draw(void)
{
	// �`�揈��
	CBlock::Draw();
}