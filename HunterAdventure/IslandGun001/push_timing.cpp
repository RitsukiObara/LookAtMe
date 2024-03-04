//============================================
//
// �����^�C�~���O�̕\������[push_timing.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "push_timing.h"
#include "input.h"
#include "texture.h"

//--------------------------------------------
// �������O���
//--------------------------------------------
namespace
{
	const char* TEXTURE[CPushTiming::TYPE_MAX] =		// �e�N�X�`��
	{
		"data\\TEXTURE\\Platform\\PushPadA.png",		// A�{�^���̃e�N�X�`��
		"data\\TEXTURE\\Platform\\PushKeySpace.png",	// SPACE�L�[�̃e�N�X�`��
	};
	const D3DXVECTOR3 TYPE_SIZE[CPushTiming::TYPE_MAX] =	// �T�C�Y
	{
		D3DXVECTOR3(1.0f, 1.0f, 0.0f),		// A�{�^���̃T�C�Y
		D3DXVECTOR3(2.0f, 1.0f, 0.0f),		// SPACE�L�[�̃T�C�Y
	};
	const int ANIM_PATTERN = 2;			// �A�j���[�V�����p�^�[��
}

//===========================================
// �R���X�g���N�^
//===========================================
CPushTiming::CPushTiming() : CBillboardAnim(CObject::TYPE_PUSHTIMING,CObject::PRIORITY_UI)
{

}

//===========================================
// �f�X�g���N�^
//===========================================
CPushTiming::~CPushTiming()
{

}

//===========================================
// ����������
//===========================================
HRESULT CPushTiming::Init(void)
{
	if (FAILED(CBillboardAnim::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CPushTiming::Uninit(void)
{
	// �I��
	CBillboardAnim::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CPushTiming::Update(void)
{
	// �X�V����
	CBillboardAnim::Update();
}

//===========================================
// �`�揈��
//===========================================
void CPushTiming::Draw(void)
{
	// �`�揈��
	CBillboardAnim::DrawLightOff(false);
}

//===========================================
// �ݒ菈��
//===========================================
void CPushTiming::SetData(const D3DXVECTOR3& pos, const float fSize, const TYPE type, const int nInterval)
{
	// ���̐ݒ菈��
	SetPos(pos);						// �ʒu
	SetPosOld(pos);						// �O��̈ʒu
	SetSize(fSize * TYPE_SIZE[type]);	// �T�C�Y
	SetEnableLookY(true);				// Y����

	// �A�j���[�V�����̐ݒ菈��
	SetAnim(nInterval, ANIM_PATTERN);

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`�����̐ݒ菈��
	SetVtxAnim(GetTexPattern(), GetPattern());

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));
}

//===========================================
// ��������
//===========================================
CPushTiming* CPushTiming::Create(const D3DXVECTOR3& pos, const float fSize, const TYPE type, const int nInterval)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPushTiming* pPushTiming = nullptr;			// �����^�C�~���O�̃C���X�^���X�𐶐�

	if (pPushTiming == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pPushTiming = new CPushTiming;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pPushTiming != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pPushTiming->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pPushTiming->SetData(pos, fSize, type, nInterval);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �����^�C�~���O�̃|�C���^��Ԃ�
	return pPushTiming;
}

//===========================================
// ��ނ̐ݒ菈��
//===========================================
void CPushTiming::SetType(const TYPE type)
{
	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE[type]));

	// �T�C�Y�̐ݒ菈��
	SetSize(TYPE_SIZE[type]);

	// ���_���W�̐ݒ菈��
	SetVertex();
}