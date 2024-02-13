//============================================
//
// �j�Ђ̃��C������[fraction.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "fraction.h"
#include "manager.h"
#include "renderer.h"
#include "useful.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define NUM_FRACTION			(15)		// �ʏ�̔j�Ђ̐�����
#define NUM_CLIFFCATCH			(40)		// �R�߂܂莞�̔j�Ђ̐�����
#define FRACTION_GRAVITY		(0.5f)		// �j�Ђ̏d��

// ��Q���̔j��
#define OBSTACLE_FRACTION_POS_SHIFT_XZ	(51)		// ��Q���̔j�Ђ̈ʒu�̂��炷��(XZ��)
#define OBSTACLE_FRACTION_POS_SHIFT_Y	(100)		// ��Q���̔j�Ђ̈ʒu�̂��炷��(Y��)
#define OBSTACLE_FRACTION_SCALE_RAND	(50)		// ��Q���̔j�Ђ̊g�嗦�̃����_����
#define OBSTACLE_FRACTION_SCALE_MIN		(0.5f)		// ��Q���̔j�Ђ̊g�嗦�̍Œ����
#define OBSTACLE_FRACTION_MOVE_RAND_XZ	(20)		// ��Q���̔j�Ђ̈ړ��ʂ̃����_���l(XZ��)
#define OBSTACLE_FRACTION_MOVE_RAND_Y	(5)			// ��Q���̔j�Ђ̈ړ��ʂ̃����_���l(Y��)
#define OBSTACLE_FRACTION_MOVE_MIN		(3)			// ��Q���̔j�Ђ̈ړ��ʂ̍Œ���l

// �ǃL�b�N�̔j��
#define WALLKICK_FRACTION_POS_SHIFT_XZ	(20)		// �ǃL�b�N�̔j�Ђ̈ʒu�̂��炷��(XZ��)
#define WALLKICK_FRACTION_POS_SHIFT_Y	(5)			// �ǃL�b�N�̔j�Ђ̈ʒu�̂��炷��(Y��)
#define WALLKICK_FRACTION_SCALE_RAND	(10)		// �ǃL�b�N�̔j�Ђ̊g�嗦�̃����_����
#define WALLKICK_FRACTION_SCALE_MIN		(0.2f)		// �ǃL�b�N�̔j�Ђ̊g�嗦�̍Œ����
#define WALLKICK_FRACTION_MOVE_RAND_XZ	(10)		// �ǃL�b�N�̔j�Ђ̈ړ��ʂ̃����_���l(XZ��)
#define WALLKICK_FRACTION_MOVE_RAND_Y	(10)		// �ǃL�b�N�̔j�Ђ̈ړ��ʂ̃����_���l(Y��)
#define WALLKICK_FRACTION_MOVE_MIN		(2)			// �ǃL�b�N�̔j�Ђ̈ړ��ʂ̍Œ���l

// �R�݂͂̔j��
#define CLIFFCATCH_FRACTION_SCALE_RAND		(30)		// �R�͂ݎ��̔j�Ђ̊g�嗦�̃����_����
#define CLIFFCATCH_FRACTION_SCALE_MIN		(0.4f)		// �R�͂ݎ��̔j�Ђ̊g�嗦�̍Œ����
#define CLIFFCATCH_FRACTION_MOVE_RAND_XZ	(15)		// �R�͂ݎ��̔j�Ђ̈ړ��ʂ̃����_���l(XZ��)
#define CLIFFCATCH_FRACTION_MOVE_RAND_Y		(5)			// �R�͂ݎ��̔j�Ђ̈ړ��ʂ̃����_���l(Y��)
#define CLIFFCATCH_FRACTION_MOVE_MIN		(3)			// �R�͂ݎ��̔j�Ђ̈ړ��ʂ̍Œ���l

//===========================================
// �R���X�g���N�^
//===========================================
CFraction::CFraction() : CModel(CObject::TYPE_FRACTION, CObject::PRIORITY_ENTITY)
{
	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
}

//===========================================
// �f�X�g���N�^
//===========================================
CFraction::~CFraction()
{

}

//===========================================
// ����������
//===========================================
HRESULT CFraction::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l���N���A����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F

	// ������Ԃ�
	return S_OK;
}

//===========================================
// �I������
//===========================================
void CFraction::Uninit(void)
{
	// �I��
	CModel::Uninit();
}

//===========================================
// �X�V����
//===========================================
void CFraction::Update(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetPos();		// �ʒu
	D3DXVECTOR3 rot = GetRot();		// ����

	// �O��̈ʒu�̐ݒ�
	SetPosOld(pos);

	// �d�͏���
	Gravity(&m_move.y, pos, FRACTION_GRAVITY);

	// �ʒu�ƌ��������Z����
	pos += m_move;

	// �ʒu�ƌ������X�V����
	SetPos(pos);
	SetRot(rot);

	if (pos.y <= 0.0f)
	{ // Y���W�� 0.0f �𒴂����ꍇ

		// �I������
		Uninit();
	}
}

//===========================================
// �`�揈��
//===========================================
void CFraction::Draw(void)
{
	// �`�揈��
	CModel::Draw(m_col);
}

//===========================================
// ���̐ݒ菈��
//===========================================
void CFraction::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3 scale, const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col)
{
	// �X�N���[���̐ݒ菈��
	SetPos(pos);							// �ʒu�̐ݒ�
	SetPosOld(pos);							// �O��̈ʒu�̐ݒ�
	SetRot(rot);							// �����̐ݒ�
	SetScale(scale);						// �g�嗦�̐ݒ�
	SetFileData(CXFile::TYPE_FRACTION);		// �t�@�C���̐ݒ菈��

	CXFile::SXFile file = GetFileData();

	// �ő�l�𔽉f����
	file.vtxMax.x *= scale.x;
	file.vtxMax.y *= scale.y;
	file.vtxMax.z *= scale.z;

	// �ŏ��l�𔽉f����
	file.vtxMin.x *= scale.x;
	file.vtxMin.y *= scale.y;
	file.vtxMin.z *= scale.z;

	// �e�N�X�`���̊��蓖�ď���
	file.m_nTexIdx[0] = nTexIdx;

	// �f�[�^�̐ݒ菈��
	SetFileData(file);

	// �S�Ă̒l������������
	m_move = move;			// �ړ���
	m_col = col;			// �F
}

//===========================================
// �ݒ菈��
//===========================================
void CFraction::SetObstacle(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scale;		// �g�嗦
	D3DXVECTOR3 move;		// �ړ���

	for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
	{
		// �ʒu��ݒ肷��
		pos.x = center.x + rand() % OBSTACLE_FRACTION_POS_SHIFT_XZ - (int)(OBSTACLE_FRACTION_POS_SHIFT_XZ * 0.5f);
		pos.y = center.y + rand() % OBSTACLE_FRACTION_POS_SHIFT_Y;
		pos.z = center.z + rand() % OBSTACLE_FRACTION_POS_SHIFT_XZ - (int)(OBSTACLE_FRACTION_POS_SHIFT_XZ * 0.5f);

		// ������ݒ肷��
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// �g�嗦��ݒ肷��
		scale.x = (float)((rand() % OBSTACLE_FRACTION_SCALE_RAND) * 0.01f) + OBSTACLE_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % OBSTACLE_FRACTION_SCALE_RAND) * 0.01f) + OBSTACLE_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % OBSTACLE_FRACTION_SCALE_RAND) * 0.01f) + OBSTACLE_FRACTION_SCALE_MIN;

		// �ړ��ʂ�ݒ肷��
		move.x = (float)(rand() % OBSTACLE_FRACTION_MOVE_RAND_XZ + OBSTACLE_FRACTION_MOVE_MIN) - (int)(OBSTACLE_FRACTION_MOVE_RAND_XZ * 0.5f);
		move.y = (float)(rand() % OBSTACLE_FRACTION_MOVE_RAND_Y + OBSTACLE_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % OBSTACLE_FRACTION_MOVE_RAND_XZ + OBSTACLE_FRACTION_MOVE_MIN) - (int)(OBSTACLE_FRACTION_MOVE_RAND_XZ * 0.5f);

		// �j�Ђ̐�������
		CFraction::Create(pos, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// �ǃL�b�N���̔j�Ђ̐ݒ菈��
//===========================================
void CFraction::SetWallKick(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scale;		// �g�嗦
	D3DXVECTOR3 move;		// �ړ���

	for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
	{
		// �ʒu��ݒ肷��
		pos.x = center.x + rand() % WALLKICK_FRACTION_POS_SHIFT_XZ - (int)(WALLKICK_FRACTION_POS_SHIFT_XZ * 0.5f);
		pos.y = center.y + rand() % WALLKICK_FRACTION_POS_SHIFT_Y;
		pos.z = center.z + rand() % WALLKICK_FRACTION_POS_SHIFT_XZ - (int)(WALLKICK_FRACTION_POS_SHIFT_XZ * 0.5f);

		// ������ݒ肷��
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// �g�嗦��ݒ肷��
		scale.x = (float)((rand() % WALLKICK_FRACTION_SCALE_RAND) * 0.01f) + WALLKICK_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % WALLKICK_FRACTION_SCALE_RAND) * 0.01f) + WALLKICK_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % WALLKICK_FRACTION_SCALE_RAND) * 0.01f) + WALLKICK_FRACTION_SCALE_MIN;

		// �ړ��ʂ�ݒ肷��
		move.x = (float)(rand() % WALLKICK_FRACTION_MOVE_RAND_XZ + WALLKICK_FRACTION_MOVE_MIN) - (int)(WALLKICK_FRACTION_MOVE_RAND_XZ * 0.5f);
		move.y = (float)(rand() % WALLKICK_FRACTION_MOVE_RAND_Y + WALLKICK_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % WALLKICK_FRACTION_MOVE_RAND_XZ + WALLKICK_FRACTION_MOVE_MIN) - (int)(WALLKICK_FRACTION_MOVE_RAND_XZ * 0.5f);

		// �j�Ђ̐�������
		CFraction::Create(pos, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// ���R�͂ݎ��̔j�Ђ̐ݒ菈��
//===========================================
void CFraction::SetLCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scale;		// �g�嗦
	D3DXVECTOR3 move;		// �j�Ђ̈ړ���

	for (int nCnt = 0; nCnt < NUM_CLIFFCATCH; nCnt++)
	{
		// ������ݒ肷��
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// �g�嗦��ݒ肷��
		scale.x = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;

		// �ړ��ʂ�ݒ肷��
		move.x = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.y = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_Y + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);

		// �j�Ђ̐�������
		CFraction::Create(center, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// �E�R�͂ݎ��̔j�Ђ̐ݒ菈��
//===========================================
void CFraction::SetRCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scale;		// �g�嗦
	D3DXVECTOR3 move;		// �j�Ђ̈ړ���

	for (int nCnt = 0; nCnt < NUM_CLIFFCATCH; nCnt++)
	{
		// ������ݒ肷��
		rot.x = (float)((rand() % 629 - 314) * 0.01f);
		rot.y = (float)((rand() % 629 - 314) * 0.01f);
		rot.z = (float)((rand() % 629 - 314) * 0.01f);

		// �g�嗦��ݒ肷��
		scale.x = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.y = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;
		scale.z = (float)((rand() % CLIFFCATCH_FRACTION_SCALE_RAND) * 0.01f) + CLIFFCATCH_FRACTION_SCALE_MIN;

		// �ړ��ʂ�ݒ肷��
		move.x = (float)-(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.y = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_Y + CLIFFCATCH_FRACTION_MOVE_MIN);
		move.z = (float)(rand() % CLIFFCATCH_FRACTION_MOVE_RAND_XZ + CLIFFCATCH_FRACTION_MOVE_MIN);

		// �j�Ђ̐�������
		CFraction::Create(center, rot, scale, move, nTexIdx, col);
	}
}

//===========================================
// ��������
//===========================================
CFraction* CFraction::Create(const D3DXVECTOR3& pos,const D3DXVECTOR3& rot,const D3DXVECTOR3 scale,const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CFraction* pFraction = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pFraction == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pFraction = new CFraction;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pFraction != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pFraction->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�j�Ђ̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pFraction->SetData(pos, rot, scale, move, nTexIdx, col);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pFraction;
}