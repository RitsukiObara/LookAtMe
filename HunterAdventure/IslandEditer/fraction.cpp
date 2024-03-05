//===========================================
//
// �j�Ђ̃��C������[fraction.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "fraction.h"
#include "useful.h"

#include "objectElevation.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------
namespace
{
	const char* MODEL[CFraction::TYPE_MAX] =	// ���f��
	{
		"data\\MODEL\\Platform\\FracScrew.x",
		"data\\MODEL\\Platform\\FracGear.x",
		"data\\MODEL\\Platform\\FracRing.x",
		"data\\MODEL\\Platform\\FracWood.x",
		"data\\MODEL\\Platform\\FracRock.x",
	};
	const float GRAVITY = 0.4f;			// �d��
	const float BOUND_MAGNI = -0.7f;	// �o�E���h�̔{��
	const int MIN_MOVE_WIDTH = 3;		// XZ���̍ŏ����̈ړ���
	const int MIN_MOVE_HEIGHT = 4;		// Y���̍ŏ����̈ړ���
	const int RAND_ROT_MOVE = 30;		// �����̃����_���̈ړ���
	const int MIN_ROT_MOVE = 12;		// �����̍ŏ����̈ړ���
	const float SUB_ALPHA = 0.05f;		// �����x�̌��Z��
}

//==============================
// �R���X�g���N�^
//==============================
CFraction::CFraction() : CModel(CObject::TYPE_FRACTION, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_move = NONE_D3DXVECTOR3;			// �ړ���
	m_rotMove = NONE_D3DXVECTOR3;		// �����̈ړ���
	m_state = STATE_NONE;				// ���
	m_nLife = 0;						// ����
	m_fAlpha = 0.0f;					// �����x
}

//==============================
// �f�X�g���N�^
//==============================
CFraction::~CFraction()
{

}

//==============================
// �j�Ђ̏���������
//==============================
HRESULT CFraction::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �����������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �l��Ԃ�
	return S_OK;
}

//========================================
// �j�Ђ̏I������
//========================================
void CFraction::Uninit(void)
{
	// �I������
	CModel::Uninit();
}

//=====================================
// �j�Ђ̍X�V����
//=====================================
void CFraction::Update(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// �ʏ���

		// ���������Z����
		m_nLife--;

		if (m_nLife <= 0)
		{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

			// ��ԃJ�E���g��0�ɂ���
			m_nLife = 0;

			// ������Ԃɂ���
			m_state = STATE_DELETE;
		}

		break;

	case STATE_DELETE:		// �������

		// �����x�����Z����
		m_fAlpha -= SUB_ALPHA;

		if (m_fAlpha <= 0.0f)
		{ // �����x�� 0.0f�ȉ� �ɂȂ����ꍇ

			// �I������
			Uninit();

			// ���̐�̏������s��Ȃ�
			return;
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// �ړ�����
	Move();

	// �����̈ړ�����
	RotMove();

	// �N���n�ʂƂ̓����蔻�菈��
	Elevation();
}

//=====================================
// �j�Ђ̕`�揈��
//=====================================
void CFraction::Draw(void)
{
	switch (m_state)
	{
	case STATE_NONE:

		// �`�揈��
		CModel::Draw();

		break;

	case STATE_DELETE:

		// �`�揈��
		CModel::Draw(m_fAlpha);

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CFraction::SetData(const D3DXVECTOR3& pos, const TYPE type, const int nLife, const int nMoveWidth, const int nMoveHeight)
{
	// ���̐ݒ菈��
	SetPos(pos);					// �ʒu
	SetPosOld(pos);					// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);		// ����
	SetScale(NONE_SCALE);			// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL[type]));		// ���f������ݒ肷��

	// �ړ��ʂ̐ݒ菈��
	MoveSet(nMoveWidth, nMoveHeight);

	// ���̐ݒ菈��
	m_state = STATE_NONE;		// ���
	m_nLife = nLife;			// ����
	m_fAlpha = 1.0f;			// �����x
}

//=======================================
// ��������
//=======================================
CFraction* CFraction::Create(const D3DXVECTOR3& pos, const TYPE type, const int nLife, const int nMoveWidth, const int nMoveHeight)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CFraction* pFrac = nullptr;	// �C���X�^���X�𐶐�

	if (pFrac == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pFrac = new CFraction;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFrac != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pFrac->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pFrac->SetData(pos, type, nLife, nMoveWidth, nMoveHeight);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �j�Ђ̃|�C���^��Ԃ�
	return pFrac;
}

//=======================================
// �ړ��ʂ̐ݒ菈��
//=======================================
void CFraction::MoveSet(const int nMoveWidth, const int nMoveHeight)
{
	// ���[�J���ϐ��錾
	float m_fRot;			// ���ł�������

	// �����������_���ŏo��
	m_fRot = (float)((rand() % 629 - 314) * 0.01f);

	// �S�Ă̒l��ݒ肷��
	m_move.x = sinf(m_fRot) * (rand() % nMoveWidth + MIN_MOVE_WIDTH);
	m_move.y = (float)(rand() % nMoveHeight + MIN_MOVE_HEIGHT);
	m_move.z = cosf(m_fRot) * (rand() % nMoveWidth + MIN_MOVE_WIDTH);

	// �����̈ړ��ʂ�ݒ肷��
	m_rotMove.x = (rand() % RAND_ROT_MOVE - MIN_ROT_MOVE) * 0.01f;		// �����̈ړ���
	m_rotMove.y = (rand() % RAND_ROT_MOVE - MIN_ROT_MOVE) * 0.01f;		// �����̈ړ���
	m_rotMove.z = (rand() % RAND_ROT_MOVE - MIN_ROT_MOVE) * 0.01f;		// �����̈ړ���
}

//=======================================
// �ړ�����
//=======================================
void CFraction::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �d�͏���
	useful::Gravity(&m_move.y, pos, GRAVITY);

	// �ʒu���X�V����
	pos += m_move;

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �����̈ړ�����
//=======================================
void CFraction::RotMove(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �������X�V����
	rot += m_rotMove;

	// �����̐��K��
	useful::RotNormalize(&rot.x);
	useful::RotNormalize(&rot.y);
	useful::RotNormalize(&rot.z);

	// ������K�p����
	SetRot(rot);
}

//=======================================
// �N���n�ʂƂ̓����蔻�菈��
//=======================================
void CFraction::Elevation(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();				// �ʒu���擾����
	float fHeight = 0.0f;					// ����
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// �擪�̏���
	CElevation* pElevEnd = nullptr;		// �����̒l
	int nIdx = 0;

	// while������
	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		// �擪�̒l���擾����
		pElev = list.GetTop();

		// �����̒l���擾����
		pElevEnd = list.GetEnd();

		while (true)
		{ // �������[�v

			// �����蔻������
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // �����蔻��̈ʒu�����������ꍇ

				// ������ݒ肷��
				pos.y = fHeight;

				// �d�͂�ݒ肷��
				m_move.y *= BOUND_MAGNI;
			}

			if (pElev == pElevEnd)
			{ // �����ɒB�����ꍇ

				// while���𔲂��o��
				break;
			}

			// ���̃I�u�W�F�N�g��������
			pElev = list.GetData(nIdx + 1);

			// �C���f�b�N�X�����Z����
			nIdx++;
		}
	}

	// �ʒu���X�V����
	SetPos(pos);
}