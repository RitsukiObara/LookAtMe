//=======================================
//
// �{�X�̃o���A�̃��C������[boss_barrier.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "boss_barrier.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// �������O���
//=======================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\BossBarrier.png";		// �e�̃e�N�X�`��
	const D3DXVECTOR3 INIT_RADIUS = D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �������a
	const int MAX_LIFE = 65;				// ����
}

//=========================
// �R���X�g���N�^
//=========================
CBossBarrier::CBossBarrier() : CBillboard(CObject::TYPE_BOSSBARRIER, CObject::PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_nLife = MAX_LIFE;		// ����
}

//=========================
// �f�X�g���N�^
//=========================
CBossBarrier::~CBossBarrier()
{

}

//=========================
// ����������
//=========================
HRESULT CBossBarrier::Init(void)
{
	if (FAILED(CBillboard::Init()))
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
void CBossBarrier::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CBossBarrier::Update(void)
{
	// ���������Z����
	m_nLife--;

	if (m_nLife <= 0)
	{ // �����������Ȃ����ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();
}

//=========================
// �`�揈��
//=========================
void CBossBarrier::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff(true);
}

//=========================
// ���̐ݒ菈��
//=========================
void CBossBarrier::SetData(const D3DXVECTOR3& pos)
{
	// ���̐ݒ菈��
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(0.0f);			// ����
	SetSize(INIT_RADIUS);	// �T�C�Y
	SetAngle();				// ����
	SetLength();			// ����
	SetEnableLookY(true);	// Y����

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

	// �S�Ă̒l��ݒ肷��
	m_nLife = MAX_LIFE;		// ����
}

//=========================
// ��������
//=========================
CBossBarrier* CBossBarrier::Create(const D3DXVECTOR3& pos)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CBossBarrier* pBarrier = nullptr;	// �{�X�̃C���X�^���X�𐶐�

	if (pBarrier == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pBarrier = new CBossBarrier;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pBarrier != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pBarrier->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pBarrier->SetData(pos);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �{�X�̃|�C���^��Ԃ�
	return pBarrier;
}