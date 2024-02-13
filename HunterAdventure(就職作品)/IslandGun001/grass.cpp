//=======================================
//
// ������[grass.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "game.h"
#include "grass.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "player.h"

//---------------------------------------
// �������O���
//---------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Diabolo.jpg";		// �e�N�X�`��
	const float SUB_LENGTH = 500.0f;			// �X���n�߂钷��
}

//---------------------------------------
// �ÓI�����o�ϐ��錾
//---------------------------------------
CListManager<CGrass*> CGrass::m_list = {};		// ���X�g

//=========================
// �R���X�g���N�^
//=========================
CGrass::CGrass() : CObject3D(CObject::TYPE_GRASS, CObject::PRIORITY_BG)
{
	// �S�Ă̒l���N���A����
	m_LeftUpDest = NONE_D3DXVECTOR3;		// �ړI�̍���̍��W
	m_RightUpDest = NONE_D3DXVECTOR3;		// �ړI�̉E��̍��W

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//=========================
// �f�X�g���N�^
//=========================
CGrass::~CGrass()
{

}

//=========================
// ����������
//=========================
HRESULT CGrass::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CGrass::Uninit(void)
{
	// �I��
	CObject3D::Uninit();

	// ������������
	m_list.Pull(this);
}

//=========================
// �X�V����
//=========================
void CGrass::Update(void)
{
	// ���_���W�̐ݒ菈��
	SetVertexUnder();
}

//=========================
// �`�揈��
//=========================
void CGrass::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// �J�����O�̐ݒ��OFF�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`�揈��
	CObject3D::DrawLightOff();

	// �J�����O�̐ݒ��ON�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=========================
// ���̐ݒ菈��
//=========================
void CGrass::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);					// �ʒu�ݒ�
	SetPosOld(pos);					// �ʒu�ݒ�
	SetRot(rot);					// �����ݒ�
	SetSize(size);					// �T�C�Y�ݒ�

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

	// ���_���W�̐ݒ菈��
	SetVertexUnder();
}

//=========================
// ��������
//=========================
CGrass* CGrass::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CGrass* pGrass = nullptr;	// ���̃C���X�^���X�𐶐�

	if (pGrass == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pGrass = new CGrass;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pGrass != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pGrass->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pGrass->SetData(pos, rot, size);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���̃|�C���^��Ԃ�
	return pGrass;
}

//=========================
// ���X�g�̎擾����
//=========================
CListManager<CGrass*> CGrass::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}
//
////=========================
//// ���_���W�̐ݒ菈��
////=========================
//void CGrass::SetVertexPos(void)
//{
//	if (CGame::GetPlayer() != nullptr)
//	{ // �v���C���[�� NULL ����Ȃ��ꍇ
//
//		D3DXVECTOR3 posPlayer = CGame::GetPlayer()->GetPos();	// �v���C���[�̈ʒu
//		D3DXVECTOR3 pos = GetPos();		// �ʒu
//		float fRotY = GetRot().y;		// Y���̌���
//		D3DXVECTOR3 size = GetSize();	// �T�C�Y
//		float fRotTop = 0.0f;			// ��̌���
//		float fLength = 0.0f;			// ����
//
//		// �㒸�_�̌�����ݒ肷��
//		fRotTop = atan2f(pos.x - posPlayer.x, pos.z - posPlayer.z);
//
//		// ������ݒ肷��
//		fLength = sqrtf((posPlayer.x - pos.x) * (posPlayer.x - pos.x) + (posPlayer.z - pos.z) * (posPlayer.z - pos.z));
//
//		D3DXVECTOR3 posLeftUp;			// ����̍��W
//		D3DXVECTOR3 posRightUp;			// �E��̍��W
//		D3DXVECTOR3 posLeftDown;		// �����̍��W
//		D3DXVECTOR3 posRightDown;		// �E���̍��W
//
//		// �����̍��W��ݒ肷��
//		posLeftDown.x = -size.x;
//		posLeftDown.y = 0.0f;
//		posLeftDown.z = 0.0f;
//
//		// �E���̍��W��ݒ肷��
//		posRightDown.x = size.x;
//		posRightDown.y = 0.0f;
//		posRightDown.z = 0.0f;
//
//		if (fLength <= 140.0f)
//		{ // ���̋����܂ŋ߂Â����ꍇ
//
//			// ����̍��W��ݒ肷��
//			m_LeftUpDest.x = posLeftDown.x + sinf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//			m_LeftUpDest.y = 40.0f;
//			m_LeftUpDest.z = posLeftDown.z + cosf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//
//			// �E��̍��W��ݒ肷��
//			m_RightUpDest.x = posRightDown.x + sinf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//			m_RightUpDest.y = 40.0f;
//			m_RightUpDest.z = posRightDown.z + cosf(fRotTop + fRotY + (GetMatrix()._11 - GetMatrix()._13)) * size.y;
//		}
//		else
//		{ // ��L�ȊO
//
//			// ����̍��W��ݒ肷��
//			m_LeftUpDest.x = -size.x;
//			m_LeftUpDest.y = size.y;
//			m_LeftUpDest.z = 0.0f;
//
//			// �E��̍��W��ݒ肷��
//			m_RightUpDest.x = size.x;
//			m_RightUpDest.y = size.y;
//			m_RightUpDest.z = 0.0f;
//		}
//
//		// �ϓ��ȕ␳����
//		useful::Correct(m_LeftUpDest.x, &m_LeftUp.x, 0.15f);
//		useful::Correct(m_LeftUpDest.y, &m_LeftUp.y, 0.15f);
//		useful::Correct(m_LeftUpDest.z, &m_LeftUp.z, 0.15f);
//
//		// �ϓ��ȕ␳����
//		useful::Correct(m_RightUpDest.x, &m_RightUp.x, 0.15f);
//		useful::Correct(m_RightUpDest.y, &m_RightUp.y, 0.15f);
//		useful::Correct(m_RightUpDest.z, &m_RightUp.z, 0.15f);
//
//		// ���_���W�̐ݒ菈��
//		SetVertexHardCoding(m_LeftUp, m_RightUp, posLeftDown, posRightDown);
//	}
//}