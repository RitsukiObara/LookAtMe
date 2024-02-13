//============================================
//
// ���U���g�\���^�C�}�[����[resulttime.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "result_time.h"
#include "result_score.h"
#include "result_rank.h"
#include "file.h"
#include "manager.h"
#include "sound.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define RESULTTIME_WIDTH_SHIFT		(120.0f)								// ���U���g�^�C�}�[�̉��̂��炷��
#define RESULTTIME_SIZE				(D3DXVECTOR3(60.0f, 80.0f, 0.0f))		// ���U���g�^�C�}�[�̃T�C�Y
#define RESULTTIME_INIT_POS			(D3DXVECTOR3(-1000.0f, 200.0f, 0.0f))	// ���U���g�^�C�}�[�̈ʒu
#define RESULTTIME_DEST_POS			(D3DXVECTOR3(100.0f, 200.0f, 0.0f))		// ���U���g�^�C�}�[�̈ʒu
#define RESULTTIME_RAND				(10)									// ���U���g�^�C�}�[�̃����_���Ō��o���镝
#define RESULTTIME_MOVE				(70.0f)									// ���U���g�^�C�}�[�̈ړ���
#define RESULTTIME_RANK_POS			(D3DXVECTOR3(320.0f, 150.0f, 100.0f))	// ���U���g�^�C�}�[�̃����N���o���ʒu
#define RESULTTIME_DOT_SIZE			(D3DXVECTOR3(12.0f, 12.0f, 0.0f))		// ���U���g�^�C�}�[�̓_�̃T�C�Y

//========================
// �R���X�g���N�^
//========================
CResultTime::CResultTime() : CTime(CObject::TYPE_TIME, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
}

//========================
// �f�X�g���N�^
//========================
CResultTime::~CResultTime()
{

}

//========================
// ����������
//========================
HRESULT CResultTime::Init(void)
{
	if (FAILED(CTime::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	SetTotalTime(CManager::GetClearTime());			// �N���A�^�C����������
	m_pos = RESULTTIME_INIT_POS;					// �ʒu��ݒ肷��

	// �v�Z����
	Calculate();

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CResultTime::Uninit(void)
{
	// �I������
	CTime::Uninit();
}

//========================
// �X�V����
//========================
void CResultTime::Update(void)
{
	if (CResult::GetState() == CResult::STATE_TIMEDISP)
	{ // �^�C�}�[�\����Ԃ̏ꍇ

		// �ړ�����
		MoveProcess();
	}
	else
	{ // ��L�ȊO�̏ꍇ

		// �v�Z����
		Calculate();
	}
}

//========================
// �`�揈��
//========================
void CResultTime::Draw(void)
{
	// �`�揈��
	CTime::Draw();
}

//========================
// �v�Z����
//========================
void CResultTime::Calculate(void)
{
	// ���l�̐ݒ菈��
	SetNumber();

	// 1�����Ƃ̃e�N�X�`���̐ݒ菈��
	SetTexture();
}

//========================
// �ړ�����
//========================
void CResultTime::MoveProcess(void)
{
	// �ʒu���X�V����
	m_pos.x += RESULTTIME_MOVE;

	if (m_pos.x >= RESULTTIME_DEST_POS.x)
	{ // �ʒu����萔�ȏ�ɂȂ����ꍇ

		// �ʒu��␳����
		m_pos.x = RESULTTIME_DEST_POS.x;

		// ���U���g�̏�Ԃ�ݒ肷��
		CResult::SetState(CResult::STATE_SCOREDISP);

		// �����N��ݒ肷��
		CResultRank::Create(RESULTTIME_RANK_POS, DecisionRank());

		// �X�R�A�𐶐�����
		CResultScore::Create();

		// �����N�̏o������炷
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_RANKAPPEAR);
	}

	for (int nCnt = 0; nCnt < MAX_TIME_DIGIT; nCnt++)
	{
		if (GetNumber(nCnt) != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �ݒ菈��
			GetNumber(nCnt)->SetPos(D3DXVECTOR3(m_pos.x + (nCnt * RESULTTIME_WIDTH_SHIFT), m_pos.y, 0.0f));				// �ʒu�ݒ�

			// ���_���̐ݒ菈��
			GetNumber(nCnt)->SetVertex();
		}
	}

	// �_�̈ړ�����
	DotMove();
}

//========================
// �_�̈ړ�����
//========================
void CResultTime::DotMove(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = GetDot()->GetPos();		//�ʒu���擾����

	// �ʒu�𓮂���
	pos.x += RESULTTIME_MOVE;

	if (pos.x >= RESULTTIME_DEST_POS.x + (3 * RESULTTIME_WIDTH_SHIFT) - RESULTTIME_SIZE.x - (RESULTTIME_DOT_SIZE.x * 0.5f))
	{ // �ʒu�����ȏ�ɂȂ����ꍇ

		// �ʒu��␳����
		pos.x = RESULTTIME_DEST_POS.x + (3 * RESULTTIME_WIDTH_SHIFT) - RESULTTIME_SIZE.x - (RESULTTIME_DOT_SIZE.x * 0.5f);
	}

	// �ʒu���X�V����
	GetDot()->SetPos(pos);

	// ���_���W�̐ݒ菈��
	GetDot()->SetVertex();
}

//========================
// �����N�̌��菈��
//========================
CResult::RANK CResultTime::DecisionRank(void)
{
	// ���[�J���ϐ��錾
	CFile::SResultFile result = CManager::GetFile()->GetResultInfo();		// ���U���g�̏����擾����
	CResult::RANK rank = CResult::RANK_C;		// �����N

	if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_SS])
	{ // SS�����N�̊�������������ꍇ

		// SS�����N��ݒ肷��
		rank = CResult::RANK_SS;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_S])
	{ // S�����N�̊�������������ꍇ

		// S�����N��ݒ肷��
		rank = CResult::RANK_S;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_A])
	{ // A�����N�̊�������������ꍇ

		// A�����N��ݒ肷��
		rank = CResult::RANK_A;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_B])
	{ // B�����N�̊�������������ꍇ

		// B�����N��ݒ肷��
		rank = CResult::RANK_B;
	}
	else if (CManager::GetClearTime() <= (DWORD)result.aRankTime[CResult::RANK_C])
	{ // C�����N�̊�������������ꍇ

		// C�����N��ݒ肷��
		rank = CResult::RANK_C;
	}

	// �����N��Ԃ�
	return rank;
}

//========================
// ��������
//========================
CResultTime* CResultTime::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CResultTime* pTime = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pTime == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pTime = new CResultTime;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pTime != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pTime->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���Ԃ̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pTime->SetData(RESULTTIME_INIT_POS, D3DXVECTOR3(0.0f, 0.0f, 0.0f), RESULTTIME_SIZE, RESULTTIME_DOT_SIZE, RESULTTIME_WIDTH_SHIFT);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// ���Ԃ̃|�C���^��Ԃ�
	return pTime;
}