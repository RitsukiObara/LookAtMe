//===========================================
//
// ���e�Ԃ̃��C������[bang_flower.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "bang_flower.h"
#include "texture.h"
#include "useful.h"

#include "bomb.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\BombFlower.x";		// ���f���̖��O
	const float BOMB_POS_SHIFT = 185.0f;					// ���e�̈ʒu�̂��炷��
	const float BOMB_ADD_HEIGHT = 199.0f;					// ���e�̍����̒ǉ���
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CBangFlower*> CBangFlower::m_list = {};			// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CBangFlower::CBangFlower() : CModel(TYPE_BANGFLOWER, PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_pBomb = nullptr;		// ���e�̏��

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CBangFlower::~CBangFlower()
{

}

//==============================
//���e�Ԃ̏���������
//==============================
HRESULT CBangFlower::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_pBomb = nullptr;		// ���e�̏��

	// �l��Ԃ�
	return S_OK;
}

//========================================
//���e�Ԃ̏I������
//========================================
void CBangFlower::Uninit(void)
{
	if (m_pBomb != nullptr)
	{ // ���e�� NULL ����Ȃ��ꍇ

		// �I������
		m_pBomb->Uninit();
		m_pBomb = nullptr;
	}

	// �I������
	CModel::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//���e�Ԃ̍X�V����
//========================================
void CBangFlower::Update(void)
{
	if (m_pBomb != nullptr)
	{ // ���e�� NULL ����Ȃ��ꍇ

		// �X�V����
		m_pBomb->Update();
	}
}

//=====================================
//���e�Ԃ̕`�揈��
//=====================================
void CBangFlower::Draw(void)
{
	// �`�揈��
	CModel::Draw();

	if (m_pBomb != nullptr)
	{ // ���e�� NULL ����Ȃ��ꍇ

		// �`�揈��
		m_pBomb->Draw();
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CBangFlower::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(rot);					// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	D3DXVECTOR3 posBomb;			// ���e�̈ʒu

	// ���e�̈ʒu��ݒ肷��
	posBomb.x = pos.x + sinf(rot.y + D3DX_PI) * BOMB_POS_SHIFT;
	posBomb.y = pos.y + BOMB_ADD_HEIGHT;
	posBomb.z = pos.z + cosf(rot.y + D3DX_PI) * BOMB_POS_SHIFT;

	// �S�Ă̒l��ݒ肷��
	m_pBomb = CBomb::Create(posBomb, D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f));		// ���e�̐���
}

//=======================================
// �q�b�g����
//=======================================
void CBangFlower::Hit(void)
{
	// �o�E���h��Ԃɂ���
	m_pBomb->SetState(CBomb::STATE_BOUND);

	// ��ނ� ���e �ɕς���
	m_pBomb->SetType(CObject::TYPE_BOMB);

	// ���e���Ǌ�����O��
	m_pBomb = nullptr;
}

//=======================================
// ��������
//=======================================
CBangFlower* CBangFlower::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBangFlower* pBangFlower = nullptr;		// �C���X�^���X�𐶐�

	if (pBangFlower == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ���e�Ԃ𐶐�����
		pBangFlower = new CBangFlower;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBangFlower != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBangFlower->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBangFlower->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���e�Ԃ̃|�C���^��Ԃ�
	return pBangFlower;
}

//=======================================
// ���e�̎擾����
//=======================================
CBomb* CBangFlower::GetBomb(void) const
{
	// ���e�̏���Ԃ�
	return m_pBomb;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CBangFlower*> CBangFlower::GetList(void)
{
	// ���X�g�\����Ԃ�
	return m_list;
}