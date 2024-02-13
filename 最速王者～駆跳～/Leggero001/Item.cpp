//=======================================
//
//�A�C�e���̃��C������[Item.cpp]
//Author ��������
//
//=======================================
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "Item.h"
#include "sound.h"
#include "useful.h"

#include "Particle.h"
#include "player.h"

//---------------------------------------
// �}�N����`
//---------------------------------------
#define ITEM_ROT_MOVE		(0.05f)			// �A�C�e���̌����̈ړ���

//==========================================
// �R���X�g���N�^
//==========================================
CItem::CItem() : CModel(CObject::TYPE_ITEM, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_APPEAR;							// ���
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̈ړ���
}

//==========================================
// �f�X�g���N�^
//==========================================
CItem::~CItem()
{

}

//==========================================
// �A�C�e���̏���������
//==========================================
HRESULT CItem::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_state = STATE_APPEAR;							// ���
	m_rotmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����̈ړ���

	// ������Ԃ�
	return S_OK;
}

//========================================
// �A�C�e���̏I������
//========================================
void CItem::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//========================================
// �A�C�e���̍X�V����
//========================================
void CItem::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// ����
	D3DXVECTOR3 scale = GetScale();	// �g�嗦

	switch (m_state)
	{
	case STATE_APPEAR:		// �o�����

		// �g�嗦�����Z����
		scale.x += 0.1f;
		scale.y += 0.1f;
		scale.z += 0.1f;

		if (scale.x >= NONE_SCALE.x)
		{ // �g�嗦�� 1.0f �𒴂����ꍇ

			// �g�嗦��␳����
			scale = NONE_SCALE;

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}

		break;

	case STATE_NONE:		// �ʏ���

		// �����̈ړ��ʂ����Z����
		rot.y += m_rotmove.y;

		// �����̐��K��
		RotNormalize(&rot.y);

		break;
	}

	// �����X�V����
	SetRot(rot);			// ����
	SetScale(scale);		// �g�嗦
}

//===========================================
// �A�C�e���̕`�揈��
//===========================================
void CItem::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//===========================================
// �q�b�g����
//===========================================
void CItem::Hit(void)
{
	// �p�[�e�B�N���̐�������
	CParticle::Create(D3DXVECTOR3(GetPos().x, GetPos().y + GetFileData().vtxMax.y, GetPos().z), CEffect::TYPE_ITEMGET);

	// �h�����N�Q�b�g����炷
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DRINKGET);

	// �X�^�~�i�̉񕜏���
	HealingStamina();

	// �A�C�e���̔j������
	Uninit();
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CItem::SetData(const D3DXVECTOR3& pos, const STATE state)
{
	// �S�Ă̒l������������
	m_state = state;										// ���
	m_rotmove = D3DXVECTOR3(0.0f, ITEM_ROT_MOVE, 0.0f);		// �����̈ړ���

	// �X�N���[���̐ݒ菈��
	SetPos(pos);			// �ʒu�ݒ�
	SetPosOld(pos);			// �O��̈ʒu�ݒ�
	SetRot(ITEM_ROT);		// �����ݒ�

	switch (m_state)
	{
	case STATE_APPEAR:		// �o�����

		// �g�嗦�ݒ�
		SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		break;

	default:				// ��L�ȊO

		// �g�嗦�ݒ�
		SetScale(NONE_SCALE);

		break;
	}

	SetFileData(CXFile::TYPE_ENERGYITEM);	// �f�[�^�̐ݒ菈��
}

//===========================================
// ��������
//===========================================
CItem* CItem::Create(const D3DXVECTOR3& pos, STATE state)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CItem* pItem = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pItem == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pItem = new CItem;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pItem != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pItem->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�A�C�e���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pItem->SetData(pos, state);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pItem;
}

//===========================================
// �X�^�~�i�̉񕜏���
//===========================================
void CItem::HealingStamina(void)
{
	// ���[�J���ϐ��錾
	CPlayer* pPlayer = nullptr;		// �v���C���[�̃|�C���^

	// �v���C���[�̏����擾����
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr)
	{ // �v���C���[�̏�� NULL ����Ȃ��ꍇ

		// �X�^�~�i�̌v�Z����
		pPlayer->CalcStamina(PLAYER_MAX_STAMINA);
	}
}