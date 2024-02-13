//=======================================
//
// �I�[���̃��C������[aura.cpp]
// Author ��������
//
//=======================================
#include "aura.h"
#include "player.h"
#include "manager.h"
#include "texture.h"

//=======================================
// �}�N����`
//=======================================
#define AURA_TEXTURE		"data/TEXTURE/PopSmoke.tga"			// �I�[���̃e�N�X�`��
#define AURA_COL			(D3DXCOLOR(1.0f,1.0f,0.2f,1.0f))	// �I�[���̐F

//=========================
// �R���X�g���N�^
//=========================
CAura::CAura() : CBillboard(CObject::TYPE_AURA, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_nLife = 0;									// ����
	m_fContra = 0.0f;								// ���a�̏k�ފԊu
}

//=========================
// �f�X�g���N�^
//=========================
CAura::~CAura()
{

}

//=========================
// ����������
//=========================
HRESULT CAura::Init(void)
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
void CAura::Uninit(void)
{
	// �I��
	CBillboard::Uninit();
}

//=========================
// �X�V����
//=========================
void CAura::Update(void)
{
	if (CScene::GetPlayer() != nullptr)
	{ // �v���C���[�̏�� NULL ����Ȃ��ꍇ

		// �ړ��ʂ����Z����
		m_move.y = CScene::GetPlayer()->GetMove().y;
	}

	// ���[�J���ϐ���錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	D3DXVECTOR3 size = GetSize();	// �T�C�Y���擾����

	// ���������炵�Ă���
	m_nLife--;

	// �ړ�����
	pos += m_move;

	// �T�C�Y���k�߂�
	size.x -= m_fContra;
	size.y -= m_fContra;

	// �ʒu���X�V����
	SetPos(pos);

	// �T�C�Y���X�V����
	SetSize(size);

	if (m_nLife <= 0)
	{ // �G�t�F�N�g�̎������s������

		// �j������
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
void CAura::Draw(void)
{
	// �`�揈��
	CBillboard::DrawLightOff();
}

//=========================
// ���̐ݒ菈��
//=========================
void CAura::SetData(const D3DXVECTOR3& pos, const int nLife)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);			// �ʒu�ݒ�

	// ������ݒ肷��
	m_nLife = nLife;

	{ // ���a�̐ݒ�

		// ���[�J���ϐ��錾
		float fRadius;		// ���a

		// ���a�������_���ŏo��
		fRadius = (float)(rand() % 10 + 30.0f);

		// �T�C�Y�ݒ�
		SetSize(D3DXVECTOR3(fRadius, fRadius, 0.0f));

		// ���a�̏k�ފԊu��ݒ肷��
		m_fContra = fRadius * (1.0f / m_nLife);			
	}

	// �S�Ă̒l������������
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���

	// ���_���̏�����
	SetVertex();

	// �F�̐ݒ菈��
	SetVtxColor(AURA_COL);

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(AURA_TEXTURE));
}

//=========================
// ��������
//=========================
CAura* CAura::Create(const D3DXVECTOR3& pos, const int nLife)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAura* pAura = nullptr;		// �I�[���̃C���X�^���X�𐶐�

	if (pAura == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pAura = new CAura;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pAura != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pAura->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�I�[���̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pAura->SetData(pos, nLife);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�[���̃|�C���^��Ԃ�
	return pAura;
}