//============================================
//
// �J�ߌ��tUI����[praiseUI.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "praiseUI.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "texture.h"

#include "score.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define AMAZING_SIZE			(D3DXVECTOR3(90.0f,15.0f,0.0f))		// �A���C�W���O�̃T�C�Y
#define GREAT_SIZE				(D3DXVECTOR3(60.0f,15.0f,0.0f))		// �O���[�g�̃T�C�Y
#define EXCITING_SIZE			(D3DXVECTOR3(90.0f,15.0f,0.0f))		// �G�L�T�C�e�B���O�̃T�C�Y
#define POS_DEST				(D3DXVECTOR3(750.0f, 40.0f, 0.0f))	// �J�ߌ��t�̖ړI�̈ʒu
#define SCALING_MAGNI			(0.05f)								// �J�ߌ��t�̊g�嗦
#define MOVE_COUNT				(15)								// �ړ���Ԃ̃J�E���g��
#define APPEAR_CHANGE_COUNT		(40)								// �o����Ԃ���ς��J�E���g��
#define ROT_MOVE				(0.2f)								// ��]���x

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CPraiseUI::m_apTexturename[CPraiseUI::TYPE_MAX] =				// �e�N�X�`���̃p�X��
{
	"data/TEXTURE/Amazing.png",			// �A���C�W���O
	"data/TEXTURE/Great.png",			// �O���[�g
	"data/TEXTURE/Exciting.png",		// �G�L�T�C�e�B���O
};

//============================
// �R���X�g���N�^
//============================
CPraiseUI::CPraiseUI() : CObject2D(CObject2D::TYPE_PRAISEUI, PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̃T�C�Y
	m_scaling = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̊g�嗦
	m_state = STATE_APPEAR;							// ���
	m_nScore = 0;									// ���_
}

//============================
// �f�X�g���N�^
//============================
CPraiseUI::~CPraiseUI()
{

}

//============================
// ����������
//============================
HRESULT CPraiseUI::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_posDest = POS_DEST;							// �ړI�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̃T�C�Y
	m_scaling = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y�̊g�嗦
	m_state = STATE_APPEAR;							// ���
	m_nScore = 0;									// ���_

	// ������Ԃ�
	return S_OK;
}

//============================
// �I������
//============================
void CPraiseUI::Uninit(void)
{
	// �I��
	CObject2D::Uninit();
}

//============================
// �X�V����
//============================
void CPraiseUI::Update(void)
{
	switch (m_state)
	{
	case STATE_APPEAR:

		// �o������
		Appear();

		break;

	case STATE_MOVE:

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		// �ړ�����
		Move();

		// ��]����
		Cycle();

		// �k������
		ScaleDown();	

		if (m_nStateCount >= MOVE_COUNT)
		{ // ��ԃJ�E���g����萔�ȏ�ɂȂ����ꍇ

			if (CManager::GetMode() == CScene::MODE_GAME)
			{ // �Q�[�����[�h�̏ꍇ

				if (CGame::GetScore() != nullptr)
				{ // �X�R�A�̏�񂪂������ꍇ

					// �A�N�V�����X�R�A�����Z����
					CGame::GetScore()->AddScore(m_nScore);
				}
			}

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;
	}

	SetLength();							// �����ݒ�
	SetAngle();								// �����ݒ�

	// ���_���̏�����
	SetVertexRot();
}

//============================
// �`�揈��
//============================
void CPraiseUI::Draw(void)
{
	// �A���t�@�e�X�g��L���ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, ALPHA_TEST_NUMBER);	// �A���t�@�e�X�g�̎Q�ƒl�ݒ�

	// �`�揈��
	CObject2D::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);			// �A���t�@�e�X�g�̗L��/�����ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);		// �A���t�@�e�X�g�̐ݒ�
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0);					// �A���t�@�e�X�g�̎Q�ƒl�ݒ�
}

//============================
// ���̐ݒ菈��
//============================
void CPraiseUI::SetData(const int nScore)
{
	// ���[�J���ϐ��錾
	TYPE type;				// ���

	// ��ނ������_���Ō��߂�
	type = (TYPE)(rand() % TYPE_MAX);

	// �X�N���[���̐ݒ菈��
	SetPos(D3DXVECTOR3(1080.0f, 150.0f, 0.0f));		// �ʒu�ݒ�
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// �����ݒ�
	SetSize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// �T�C�Y�ݒ�

	switch (type)
	{
	case TYPE_AMAZING:			// �A���C�W���O

		m_sizeDest = AMAZING_SIZE;		// �ړI�̃T�C�Y

		break;

	case TYPE_GREAT:			// �O���[�g

		m_sizeDest = GREAT_SIZE;		// �ړI�̃T�C�Y

		break;

	case TYPE_EXCITING:			// �G�L�T�C�e�B���O

		m_sizeDest = EXCITING_SIZE;		// �ړI�̃T�C�Y

		break;

	default:

		// �x����
		MessageBox(NULL, "�J�߉��oUI�ŗ\�����Ȃ���ނ����m�I", "�x���I", MB_ICONWARNING);

		break;
	}

	// ���_���̏�����
	SetVertexRot();

	// �S�Ă̒l��ݒ肷��
	m_posDest = POS_DEST;						// �ړI�̈ʒu
	m_move = (m_posDest - GetPos()) / (float)MOVE_COUNT;		// �ړ���
	m_state = STATE_APPEAR;						// ���
	m_nScore = nScore;							// ���_
	m_scaling = m_sizeDest * SCALING_MAGNI;		// �T�C�Y�̊g�嗦

	// �e�N�X�`���̊��蓖�ď���
	BindTexture(CManager::GetTexture()->Regist(m_apTexturename[type]));
}

//============================
// �o������
//============================
void CPraiseUI::Appear(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();		// �T�C�Y

	if (size.x >= m_sizeDest.x ||
		size.y >= m_sizeDest.y)
	{ // ���݂̃T�C�Y���ړI�̃T�C�Y�ȏゾ�����ꍇ

		// �ړI�̃T�C�Y�ɕ␳����
		size = m_sizeDest;

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;
	}
	else
	{ // ��L�ȊO

		// �T�C�Y���g�傷��
		size += m_scaling;
	}

	if (m_nStateCount >= APPEAR_CHANGE_COUNT)
	{ // ��ԃJ�E���g�����ȏ�ɂȂ����ꍇ

		// �ړ���Ԃɂ���
		m_state = STATE_MOVE;

		// ��ԃJ�E���g�� 0 �ɂ���
		m_nStateCount = 0;

		// �ڕW�̃T�C�Y��ݒ肷��
		m_sizeDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �T�C�Y�̊g�嗦��ݒ肷��
		m_scaling = (m_sizeDest - size) / (float)MOVE_COUNT;
	}

	// �T�C�Y�̐ݒ菈��
	SetSize(size);
}

//============================
// �ړ�����
//============================
void CPraiseUI::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu

	// �ړ��ʂ��ʒu�ɉ��Z����
	pos += m_move;

	// �����X�V����
	SetPos(pos);		// �ʒu
}

//============================
// ��]����
//============================
void CPraiseUI::Cycle(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// ����

	// ���������Z����
	rot.z += ROT_MOVE;

	// �������X�V����
	SetRot(rot);		
}

//============================
// �k������
//============================
void CPraiseUI::ScaleDown(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 size = GetSize();		// �T�C�Y

	if (size.x <= m_sizeDest.x ||
		size.y <= m_sizeDest.y)
	{ // ���݂̃T�C�Y���ړI�̃T�C�Y�ȏゾ�����ꍇ

		// �ړI�̃T�C�Y�ɕ␳����
		size = m_sizeDest;
	}
	else
	{ // ��L�ȊO

		// �T�C�Y���g�傷��
		size += m_scaling;
	}

	// �T�C�Y�̐ݒ菈��
	SetSize(size);
}

//============================
//��������
//============================
CPraiseUI* CPraiseUI::Create(const int nScore)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPraiseUI* pLandUI = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pLandUI == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pLandUI = new CPraiseUI;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pLandUI != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pLandUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�J�ߌ��tUI�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pLandUI->SetData(nScore);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pLandUI;
}