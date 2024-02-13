//=======================================
//
// �V���G�b�g�̃��C������[silhouette.cpp]
// Author ��������
//
//=======================================
#include "silhouette.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include"locus2D.h"

//---------------------------------------
// �}�N����`
//---------------------------------------
#define SILHOUETTE_SIZE			(D3DXVECTOR3(150.0f, 150.0f, 0.0f))						// �V���G�b�g�̃T�C�Y
#define SILHOUETTE_RIGHT_POS	(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 600.0f, 0.0f))		// �V���G�b�g�̉E�̈ʒu
#define SILHOUETTE_LEFT_POS		(D3DXVECTOR3(200.0f, 600.0f, 0.0f))						// �V���G�b�g�̍��̈ʒu
#define SILHOUETTE_DISTANCE		(400.0f)												// �V���G�b�g�̋���
#define SILHOUETTE_APPEAR_COUNT	(40)													// �o������J�E���g��
#define SILHOUETTE_LOCUS_COUNT	(4)														// �c�����o���J�E���g��

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
const char* CSilhouette::m_apTextureFile[TYPE_MAX] = 			// �e�N�X�`���̖��O
{
	"data/TEXTURE/Silhouette000.png",			// ����l�^
	"data/TEXTURE/Silhouette001.png",			// ���z����l�^
	"data/TEXTURE/Silhouette002.png",			// �͂ސl�^
	"data/TEXTURE/Silhouette003.png",			// ��щz����l�^
};

//=========================
// �R���X�g���N�^
//=========================
CSilhouette::CSilhouette() : CObject2D(CObject2D::TYPE_SILHOUETTE, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_state = STATE_APPEAR;		// ���
	m_type = TYPE_RUN;			// ���
	m_nStateCount = 0;			// ��ԃJ�E���g
	m_fCircumRot = 0.0f;		// �~����̌���
	m_fRotMove = 0.0f;			// �����̈ړ���
	m_bRight = true;			// �E�����ǂ���
}

//=========================
// �f�X�g���N�^
//=========================
CSilhouette::~CSilhouette()
{

}

//=========================
// ����������
//=========================
HRESULT CSilhouette::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_state = STATE_APPEAR;		// ���
	m_type = TYPE_RUN;			// ���
	m_nStateCount = 0;			// ��ԃJ�E���g
	m_fCircumRot = 0.0f;		// �~����̌���
	m_fRotMove = 0.0f;			// �����̈ړ���
	m_bRight = true;			// �E�����ǂ���

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CSilhouette::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//=========================
// �X�V����
//=========================
void CSilhouette::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	switch (m_state)
	{
	case STATE_APPEAR:		// �o�����

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount <= SILHOUETTE_APPEAR_COUNT)
		{ // �ړ��J�E���g����萔�ȏ�̏ꍇ

			// �~����̌��������Z����
			m_fCircumRot += m_fRotMove;
		}
		else
		{ // ��L�ȊO

			// ��~��Ԃɂ���
			m_state = STATE_STOP;
		}

		// �����̐��K��
		RotNormalize(&m_fCircumRot);

		if (m_bRight == true)
		{ // �E���̏ꍇ

			// �ʒu��ݒ肷��
			pos.x = SILHOUETTE_RIGHT_POS.x + sinf(m_fCircumRot)* SILHOUETTE_DISTANCE;
			pos.y = SILHOUETTE_RIGHT_POS.y + cosf(m_fCircumRot)* SILHOUETTE_DISTANCE;
		}
		else
		{ // �����̏ꍇ

			// �ʒu��ݒ肷��
			pos.x = SILHOUETTE_LEFT_POS.x + sinf(m_fCircumRot)* SILHOUETTE_DISTANCE;
			pos.y = SILHOUETTE_LEFT_POS.y + cosf(m_fCircumRot)* SILHOUETTE_DISTANCE;
		}

		if (m_nStateCount % SILHOUETTE_LOCUS_COUNT == 0)
		{ // ���Ԋu��

			// �c���̐���
			CLocus2D::Create(pos, GetSize(), 0.4f, 13, GetTexIdx());
		}

		break;

	case STATE_STOP:		// ��~���

		// ���ɏ�������

		break;
	}

	// ����ݒ肷��
	SetPos(pos);				// �ʒu
	SetLength();				// ����
	SetAngle();					// ����

	// ���_���̏�����
	SetVertex();
}

//=========================
// �`�揈��
//=========================
void CSilhouette::Draw(void)
{
	// �`�揈��
	CObject2D::Draw();
}

//=========================
// ���̐ݒ菈��
//=========================
void CSilhouette::SetData(const TYPE type, const bool bRight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �ʒu

	// �X�N���[���̐ݒ菈��
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// �����ݒ�
	SetSize(SILHOUETTE_SIZE);	// �T�C�Y�ݒ�

	// �S�Ă̒l������������
	m_state = STATE_APPEAR;		// ���
	m_type = type;				// ���
	m_nStateCount = 0;			// ��ԃJ�E���g
	m_bRight = bRight;			// �E�����ǂ���

	if (m_bRight == true)
	{ // �E���̏ꍇ

		// �~����̌�����ݒ肷��
		m_fCircumRot = D3DX_PI * 0.5f;

		// �ʒu��ݒ肷��
		pos.x = SILHOUETTE_RIGHT_POS.x + sinf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.y = SILHOUETTE_RIGHT_POS.y + cosf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.z = 0.0f;

		// �ʒu�ݒ�
		SetPos(pos);
	}
	else
	{ // �����̏ꍇ

		// �~����̌�����ݒ肷��
		m_fCircumRot = D3DX_PI * -0.5f;

		// �ʒu��ݒ肷��
		pos.x = SILHOUETTE_LEFT_POS.x + sinf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.y = SILHOUETTE_LEFT_POS.y + cosf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.z = 0.0f;

		// �ʒu�ݒ�
		SetPos(pos);
	}

	// �����̈ړ��ʂ�ݒ肷��
	m_fRotMove = D3DX_PI - m_fCircumRot;

	// �����̐��K��
	RotNormalize(&m_fRotMove);

	// �t���[�����̈ړ��ʂɂ���
	m_fRotMove /= SILHOUETTE_APPEAR_COUNT;

	SetLength();				// �����ݒ�
	SetAngle();					// �����ݒ�

	// ���_���̏�����
	SetVertex();

	// �e�N�X�`���̐ݒ菈��
	SetVtxTexture();

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::GetTexture()->Regist(m_apTextureFile[m_type]));
}

//=========================
// ��������
//=========================
CSilhouette* CSilhouette::Create(const TYPE type, const bool bRight)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSilhouette* pEffect = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEffect == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEffect = new CSilhouette;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEffect->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�V���G�b�g�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEffect->SetData(type, bRight);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pEffect;
}