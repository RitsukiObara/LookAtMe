//=======================================
//
// �G�t�F�N�g���f���̃��C������[effect_model.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "effect_model.h"
#include "useful.h"

//=======================================
// �������O���
//=======================================
namespace
{
	const char* MODEL = "data\\MODEL\\Platform\\FracScrew.x";		// ���f���̖��O
}

//=========================
// �R���X�g���N�^
//=========================
CEffectModel::CEffectModel() : CModel(CObject::TYPE_EFFECTMODEL, CObject::PRIORITY_EFFECT)
{
	// �S�Ă̒l���N���A����
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_rotMove = NONE_D3DXVECTOR3;	// �����̈ړ���
	m_reduce = NONE_D3DXVECTOR3;	// �g�嗦�̏k�ފԊu
	m_nLife = 0;					// ����
	m_fAlpha = 1.0f;				// �����x
	m_fSub = 0.0f;					// �����ɂȂ�Ԋu
}

//=========================
// �f�X�g���N�^
//=========================
CEffectModel::~CEffectModel()
{

}

//=========================
// ����������
//=========================
HRESULT CEffectModel::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_move = NONE_D3DXVECTOR3;		// �ړ���
	m_rotMove = NONE_D3DXVECTOR3;	// �����̈ړ���
	m_reduce = NONE_D3DXVECTOR3;	// �g�嗦�̏k�ފԊu
	m_nLife = 0;					// ����
	m_fAlpha = 1.0f;				// �����x
	m_fSub = 0.0f;					// �����ɂȂ�Ԋu

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CEffectModel::Uninit(void)
{
	// �I��
	CModel::Uninit();
}

//=========================
// �X�V����
//=========================
void CEffectModel::Update(void)
{
	// �ړ�����
	Move();

	// �����̈ړ�����
	RotMove();

	// �����x�̌��Z����
	SubAlpha();

	// �g�嗦�̌��Z����
	Reduce();

	// ���������炵�Ă���
	m_nLife--;

	if (m_nLife <= 0)
	{ // �G�t�F�N�g�̎������s������

		// �j������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}
}

//=========================
// �`�揈��
//=========================
void CEffectModel::Draw(void)
{
	// �`�揈��
	CModel::Draw(m_fAlpha);
}

//=========================
// ���̐ݒ菈��
//=========================
void CEffectModel::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove)
{
	// ���̐ݒ菈��
	SetPos(pos);							// �ʒu
	SetPosOld(pos);							// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);				// ����
	SetScale(scale);						// �g�嗦
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// ���f���̏��

	// �S�Ă̒l��ݒ肷��
	m_move = move;				// �ړ���
	m_rotMove = rotMove;		// �����̈ړ���
	m_nLife = nLife;			// ����
	m_fAlpha = 1.0f;			// �����x
	m_fSub = m_fAlpha * (1.0f / m_nLife);		// �����ɂȂ�Ԋu
	m_reduce.x = scale.x * (1.0f / m_nLife);	// �g�嗦�̏k�ފԊu(X��)
	m_reduce.y = scale.y * (1.0f / m_nLife);	// �g�嗦�̏k�ފԊu(Y��)
	m_reduce.z = scale.z * (1.0f / m_nLife);	// �g�嗦�̏k�ފԊu(Z��)
}

//=========================
// ��������
//=========================
CEffectModel* CEffectModel::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEffectModel* pEffect = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pEffect == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pEffect = new CEffectModel;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEffect->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEffect->SetData(pos, move, scale, nLife, rotMove);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pEffect;
}

//=========================
// �ړ�����
//=========================
void CEffectModel::Move(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu���擾����

	// �ړ�����
	pos += m_move;

	// �ʒu���X�V����
	SetPos(pos);
}

//=========================
// �����̈ړ�����
//=========================
void CEffectModel::RotMove(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot = GetRot();		// �������擾����

	// �������ړ�����
	rot += m_rotMove;

	// �����̐��K��
	useful::RotNormalize(&rot.x);
	useful::RotNormalize(&rot.y);
	useful::RotNormalize(&rot.z);

	// �ʒu���X�V����
	SetRot(rot);
}

//=========================
// �����x�̌��Z����
//=========================
void CEffectModel::SubAlpha(void)
{
	// �����x�����Z������
	m_fAlpha -= m_fSub;

	if (m_fAlpha <= 0.0f)
	{ // �����x��0.0f�ȉ��̏ꍇ

		// �����x��␳����
		m_fAlpha = 0.0f;
	}
}

//=========================
// �g�嗦�̌��Z����
//=========================
void CEffectModel::Reduce(void)
{
	// ���[�J���ϐ���錾
	D3DXVECTOR3 scale = GetScale();		// �g�嗦���擾����

	// �g�嗦���k�߂�
	scale -= m_reduce;

	if (scale.x <= 0.0f)
	{ // �g�嗦��0.0f�ȉ��̏ꍇ

		// �g�嗦��␳����
		scale.x = 0.0f;
	}

	if (scale.y <= 0.0f)
	{ // �g�嗦��0.0f�ȉ��̏ꍇ

		// �g�嗦��␳����
		scale.y = 0.0f;
	}

	if (scale.z <= 0.0f)
	{ // �g�嗦��0.0f�ȉ��̏ꍇ

		// �g�嗦��␳����
		scale.z = 0.0f;
	}

	// �g�嗦��K�p����
	SetScale(scale);
}