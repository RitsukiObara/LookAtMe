//===========================================
//
// ���V�̎��̃��C������[palm_fruit.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "palm_fruit.h"
#include "renderer.h"
#include "useful.h"

#include "objectElevation.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const char* FRUIT_MODEL = "data\\MODEL\\PalmFruit.x";		// ���V�̎��̃��f��
	const char* ITEM_MODEL = "data\\MODEL\\PalmItem.x";			// ���V�̎�(�A�C�e����)�̃��f��
	const float GRAVITY = 0.5f;			// �d��
	const float BOUND_GRAVITY = 10.0f;	// �o�E���h�����Ƃ���Y���̈ړ���
	const float BOUND_MOVE = 4.4f;		// �o�E���h�����Ƃ��̈ړ���
	const float BOUND_MOVE_ROT = 0.6f;	// �o�E���h�����Ƃ��̈ړ��ʂ̌���
}

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
CListManager<CPalmFruit*> CPalmFruit::m_list = {};		// ���X�g

//==============================
// �R���X�g���N�^
//==============================
CPalmFruit::CPalmFruit() : CModel(CObject::TYPE_PALMFRUIT, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_move = NONE_D3DXVECTOR3;	// �ړ���
	m_state = STATE_NONE;		// ���

	// ���X�g�ɒǉ�����
	m_list.Regist(this);
}

//==============================
// �f�X�g���N�^
//==============================
CPalmFruit::~CPalmFruit()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CPalmFruit::Init(void)
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
//�u���b�N�̏I������
//========================================
void CPalmFruit::Uninit(void)
{
	// �I������
	CModel::Uninit();

	// ������������
	m_list.Pull(this);
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CPalmFruit::Update(void)
{
	switch (m_state)
	{
	case CPalmFruit::STATE_NONE:

		// �N���n�ʂ̓����蔻��
		ElevationCollision();

		break;

	case CPalmFruit::STATE_FALL:

		// �d�͏���
		Gravity();

		if (ElevationCollision() == true)
		{ // �N���̒n�ʂɕt�����ꍇ

			// �o�E���h��Ԃɂ���
			m_state = STATE_BOUND;

			// �ړ��ʂ�ݒ肷��
			m_move.x = sinf(GetRot().y + BOUND_MOVE_ROT) * BOUND_MOVE;
			m_move.y = BOUND_GRAVITY;
			m_move.z = cosf(GetRot().y + BOUND_MOVE_ROT) * BOUND_MOVE;
		}

		break;

	case CPalmFruit::STATE_BOUND:

		// �ړ�����
		Move();

		// �d�͏���
		Gravity();

		if (ElevationCollision() == true)
		{ // �N���̒n�ʂɕt�����ꍇ

			// ��~��Ԃɂ���
			m_state = STATE_STOP;

			// �d�͂�0.0f�ɂ���
			m_move.y = 0.0f;
		}

		break;

	case CPalmFruit::STATE_STOP:



		break;

	case CPalmFruit::STATE_GET:

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CPalmFruit::Draw(void)
{
	// �`�揈��
	CModel::Draw();
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CPalmFruit::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���̐ݒ菈��
	SetPos(pos);							// �ʒu
	SetPosOld(pos);							// �O��̈ʒu
	SetRot(rot);							// ����
	SetScale(NONE_SCALE);					// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(FRUIT_MODEL));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_move = NONE_D3DXVECTOR3;	// �ړ���
	m_state = STATE_NONE;		// ���
}

//=======================================
// ��������
//=======================================
CPalmFruit* CPalmFruit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CPalmFruit* pFruit = nullptr;	// �C���X�^���X�𐶐�

	if (pFruit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pFruit = new CPalmFruit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFruit != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pFruit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pFruit->SetData(pos, rot);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���V�̎��̃|�C���^��Ԃ�
	return pFruit;
}

//=======================================
// ���X�g�̎擾����
//=======================================
CListManager<CPalmFruit*> CPalmFruit::GetList(void)
{
	// ���X�g�}�l�[�W���[��Ԃ�
	return m_list;
}

//=======================================
// �ړ��ʂ̐ݒ菈��
//=======================================
void CPalmFruit::SetMove(const D3DXVECTOR3& move)
{
	// �ړ��ʂ�ݒ肷��
	m_move = move;
}

//=======================================
// �ړ��ʂ̎擾����
//=======================================
D3DXVECTOR3 CPalmFruit::GetMove(void) const
{
	// �ړ��ʂ�Ԃ�
	return m_move;
}

//=======================================
// ��Ԃ̐ݒ菈��
//=======================================
void CPalmFruit::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//=======================================
// ��Ԃ̎擾����
//=======================================
CPalmFruit::STATE CPalmFruit::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//=======================================
// �q�b�g����
//=======================================
void CPalmFruit::Hit(void)
{
	// ���f�������V�̎�(�A�C�e���^)�ɂ���
	SetFileData(CManager::Get()->GetXFile()->Regist(ITEM_MODEL));
}

//=======================================
// �ړ�����
//=======================================
void CPalmFruit::Move(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �ړ�������
	pos.x += m_move.x;
	pos.z += m_move.z;

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �d�͏���
//=======================================
void CPalmFruit::Gravity(void)
{
	// �ʒu���擾����
	D3DXVECTOR3 pos = GetPos();

	// �d�͏���
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// �ʒu��K�p����
	SetPos(pos);
}

//=======================================
// �N���n�ʂ̓����蔻�菈��
//=======================================
bool CPalmFruit::ElevationCollision(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����
	float fHeight = 0.0f;			// ����
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
				m_move.y = 0.0f;

				// �ʒu���X�V����
				SetPos(pos);

				// true ��Ԃ�
				return true;
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

	// false ��Ԃ�
	return false;
}