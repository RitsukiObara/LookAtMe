//===========================================
//
// ���V�̖؂̃��C������[palm_tree.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "palm_tree.h"
#include "renderer.h"
#include "useful.h"

#include "palm_fruit.h"
#include "texture.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const float FRUIT_SHIFT = 100.0f;		// ���V�̎��̂��炷��
	const float FRUIT_HEIGHT = 630.0f;		// ���V�̎��̍���
	const float FRUIT_ROT = -0.5f;			// ���V�̎��̌���
}

//==============================
// �R���X�g���N�^
//==============================
CPalmTree::CPalmTree() : CTree()
{
	// �S�Ă̒l���N���A����
	m_pPalmFruit = nullptr;		// ���V�̎��̏��
}

//==============================
// �f�X�g���N�^
//==============================
CPalmTree::~CPalmTree()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CPalmTree::Init(void)
{
	if (FAILED(CTree::Init()))
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
void CPalmTree::Uninit(void)
{
	// ���V�̎��� NULL �ɂ���
	m_pPalmFruit = nullptr;

	// �I������
	CTree::Uninit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CPalmTree::Update(void)
{
	// �X�V����
	CTree::Update();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CPalmTree::Draw(void)
{
	// �`�揈��
	CTree::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CPalmTree::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ���̐ݒ菈��
	CTree::SetData(pos, rot, type);

	D3DXVECTOR3 posFruit;		// ���V�̎��̈ʒu
	D3DXVECTOR3 rotFruit;		// ���V�̎��̌���

	// ���V�̎��̈ʒu�ƌ�����ݒ肷��
	posFruit.x = pos.x + sinf(rot.y) * FRUIT_SHIFT;
	posFruit.y = pos.y + FRUIT_HEIGHT;
	posFruit.z = pos.z + cosf(rot.y) * FRUIT_SHIFT;
	rotFruit.x = FRUIT_ROT;
	rotFruit.y = rot.y;
	rotFruit.z = 0.0f;

	// �S�Ă̒l��ݒ肷��
	m_pPalmFruit = CPalmFruit::Create(posFruit, rotFruit);		// ���V�̎��̏��
}

//=====================================
// �q�b�g����
//=====================================
void CPalmTree::Hit(void)
{
	// �q�b�g����
	CTree::Hit();

	if (m_pPalmFruit != nullptr)
	{ //���V�̎��� NULL ����Ȃ��ꍇ

		// ������Ԃɂ���
		m_pPalmFruit->SetState(CPalmFruit::STATE_FALL);

		// ���V�̎��� NULL �ɂ���
		m_pPalmFruit = nullptr;
	}
}