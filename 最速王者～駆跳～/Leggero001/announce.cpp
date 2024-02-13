//============================================
//
// �c�苗���̍��m����[announce.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "announce.h"
#include "object2D.h"
#include "animobject.h"
#include "manager.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "goal.h"
#include "player.h"

//--------------------------------------------
// �}�N����`
//--------------------------------------------
#define ROAD_SIZE			(D3DXVECTOR3(1000.0f, 10.0f, 0.0f))		// ���̂�̃T�C�Y
#define GOAL_SIZE			(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// �S�[���̃T�C�Y
#define PLAYER_SIZE			(D3DXVECTOR3(15.0f, 30.0f, 0.0f))		// �v���C���[�̃T�C�Y

#define ROAD_POS			(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (ROAD_SIZE.x * 0.5f), 700.0f, 0.0f))		// ���̂�̈ʒu
#define ROAD_TEX			(100.0f)								// ���̂�̃e�N�X�`�����W
#define PLAYER_ANIM_COUNT	(10)									// �v���C���[�̃A�j���[�V�����̃J�E���g
#define PLAYER_ANIM_PATTERN	(4)										// �v���C���[�̃A�j���[�V�����̃p�^�[����

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
const char* CAnnounce::c_apTextureName[TEX_MAX] =				// �e�N�X�`���̖��O
{
	"data/TEXTURE/Road.png",			// ���̂�
	"data/TEXTURE/GoalMark.png",		// �S�[���}�[�N
	"data/TEXTURE/PlayerMark.png",		// �v���C���[�}�[�N
};

//========================
// �R���X�g���N�^
//========================
CAnnounce::CAnnounce() : CObject(TYPE_ANNOUNCE, PRIORITY_UI)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		// �����̏��
		m_apObject2D[nCnt] = nullptr;
	}

	m_pPlayer = nullptr;	// �v���C���[
	m_fTotalDist = 0.0f;	// �����̋���
	m_nDist = 0;			// ����
}

//========================
// �f�X�g���N�^
//========================
CAnnounce::~CAnnounce()
{

}

//========================
// ����������
//========================
HRESULT CAnnounce::Init(void)
{
	// �S�Ă̒l������������
	m_fTotalDist = 0.0f;	// �����̋���
	m_nDist = 0;			// ����

	// ���������m�ۂ���
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] == nullptr)
		{ // �|�C���^�� NULL �̏ꍇ

			// �I�u�W�F�N�g�𐶐�����
			m_apObject2D[nCnt] = CObject2D::Create(CObject2D::TYPE_NONE, CObject::TYPE_NONE, PRIORITY_UI);

			if (m_apObject2D[nCnt] != nullptr)
			{ // �|���S���̃|�C���^�� NULL ����Ȃ��ꍇ

				// �e�N�X�`���̊��蓖�ď���
				m_apObject2D[nCnt]->BindTexture(CManager::GetTexture()->Regist(c_apTextureName[nCnt]));
			}
			else
			{ // ��L�ȊO

				// �x����
				MessageBox(NULL, "���m�̃|�C���^��NULL�ł��I", "�x���I", MB_ICONWARNING);

				// ���s��Ԃ�
				return E_FAIL;
			}
		}
		else
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �x����
			MessageBox(NULL, "���m�̃����������Ɏg���Ă���I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	if (m_pPlayer == nullptr)
	{ // �|�C���^�� NULL �̏ꍇ

		// �v���C���[�𐶐�����
		m_pPlayer = CAnim::Create(CObject::TYPE_NONE, PRIORITY_UI);

		if (m_pPlayer != nullptr)
		{ // �|�C���^�� NULL ����Ȃ��ꍇ

			// �e�N�X�`���̊��蓖�ď���
			m_pPlayer->BindTexture(CManager::GetTexture()->Regist(c_apTextureName[TEX_PLAYER]));
		}
		else
		{ // ��L�ȊO

			// �x����
			MessageBox(NULL, "���m�̃|�C���^��NULL�ł��I", "�x���I", MB_ICONWARNING);

			// ���s��Ԃ�
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//========================
// �I������
//========================
void CAnnounce::Uninit(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �|���S���̃|�C���^�� NULL ����Ȃ��ꍇ

			// �I������
			m_apObject2D[nCnt]->Uninit();
			m_apObject2D[nCnt] = nullptr;
		}
	}

	if (m_pPlayer != nullptr)
	{ // �v���C���[���g�p����Ă���ꍇ

		// �I������
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// �����̏I������
	Release();
}

//========================
// �X�V����
//========================
void CAnnounce::Update(void)
{
	// �������菈��
	DistanceMeasure();

	// �v���C���[�̈ړ�����
	PlayerMove();

	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (nCnt == POLY_ROAD)
		{ // ���̂�̏ꍇ

			// ���_���W�̐ݒ菈��
			m_apObject2D[nCnt]->SetVtxWidthGage();
		}
		else
		{ // ��L�ȊO

			// ���_���W�̐ݒ菈��
			m_apObject2D[nCnt]->SetVertex();
		}
	}
}

//========================
// �`�揈��
//========================
void CAnnounce::Draw(void)
{
	// �S�Ă̒l������������
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		// �`�揈��
		m_apObject2D[nCnt]->Draw();
	}

	// �`�揈��
	m_pPlayer->Draw();
}

//========================
// ���̐ݒ菈��
//========================
void CAnnounce::SetData(void)
{
	for (int nCnt = 0; nCnt < POLY_MAX; nCnt++)
	{
		if (m_apObject2D[nCnt] != nullptr)
		{ // �I�u�W�F�N�g�̏�� NULL �̏ꍇ

			// ���̐ݒ菈��
			switch (nCnt)
			{
			case POLY_ROAD:		// ���̂�

				m_apObject2D[nCnt]->SetPos(ROAD_POS);		// �ʒu�ݒ�
				m_apObject2D[nCnt]->SetSize(ROAD_SIZE);		// �T�C�Y�ݒ�

				// ���_���̐ݒ菈��
				m_apObject2D[nCnt]->SetVtxWidthGage();

				// �����̃e�N�X�`�����W��ݒ肷��
				m_apObject2D[nCnt]->SetVtxTextureWidth(ROAD_TEX);

				break;

			case POLY_GOAL:		// �S�[��

				m_apObject2D[nCnt]->SetPos(D3DXVECTOR3(m_apObject2D[POLY_ROAD]->GetPos().x + m_apObject2D[POLY_ROAD]->GetSize().x, m_apObject2D[POLY_ROAD]->GetPos().y - m_apObject2D[POLY_ROAD]->GetSize().y - GOAL_SIZE.y, 0.0f));		// �ʒu�ݒ�
				m_apObject2D[nCnt]->SetSize(GOAL_SIZE);		// �T�C�Y�ݒ�

				// ���_���̐ݒ菈��
				m_apObject2D[nCnt]->SetVertex();

				break;
			}

			// �ݒ菈��
			m_apObject2D[nCnt]->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// �����ݒ�
			m_apObject2D[nCnt]->SetLength();			// �����ݒ�
			m_apObject2D[nCnt]->SetAngle();				// �����ݒ�
		}
	}

	// ���̐ݒ菈��
	m_pPlayer->SetPos(D3DXVECTOR3(m_apObject2D[POLY_ROAD]->GetPos().x, m_apObject2D[POLY_ROAD]->GetPos().y - m_apObject2D[POLY_ROAD]->GetSize().y - PLAYER_SIZE.y, 0.0f));		// �ʒu�ݒ�
	m_pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// �����ݒ�
	m_pPlayer->SetSize(PLAYER_SIZE);		// �T�C�Y�ݒ�
	m_pPlayer->SetLength();					// �����ݒ�
	m_pPlayer->SetAngle();					// �����ݒ�

	// �A�j���[�V�����̐ݒ菈��
	m_pPlayer->SetAnim(PLAYER_ANIM_COUNT, PLAYER_ANIM_PATTERN);

	// ���_���̐ݒ菈��
	m_pPlayer->SetVertex();

	// �e�N�X�`�����W�̐ݒ菈��
	m_pPlayer->SetVtxTextureAnim(m_pPlayer->GetTexPattern(), m_pPlayer->GetPattern());

	// �������菈��
	DistanceMeasure();

	if (CManager::GetFile() != nullptr)
	{ // �t�@�C���� NULL ����Ȃ��ꍇ

		// �����̋��������߂�
		m_fTotalDist = CManager::GetFile()->GetGameInfo().GoalPos.x - CManager::GetFile()->GetGameInfo().StartPos.x;
	}
}

//========================
// �������菈��
//========================
void CAnnounce::DistanceMeasure(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// �I�u�W�F�N�g�̃|�C���^
	CGoal* pGoal = nullptr;			// �S�[���̃|�C���^

	// �擪�̃|�C���^���擾����
	pObj = CObject::GetTop(PRIORITY_PLAYER);

	while (pObj != nullptr)
	{ // �|�C���^�� NULL ����Ȃ������

		if (pObj->GetType() == TYPE_GOAL)
		{ // ��ނ��S�[���̏ꍇ

			// �S�[���̏����擾����
			pGoal = dynamic_cast<CGoal*>(pObj);

			if (pGoal != nullptr)
			{ // �S�[���� NULL �̏ꍇ

				// �����o��
				break;
			}
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObj->GetNext();
	}

	if (pGoal != nullptr &&
		CScene::GetPlayer() != nullptr)
	{ // �S�[���̃|�C���^�� NULL ����Ȃ��ꍇ

		// ������ݒ肷��
		m_nDist = (int)((pGoal->GetPos().x - pGoal->GetSize().x) - CScene::GetPlayer()->GetPos().x);
	}
	else
	{ // ��L�ȊO

		// ������ݒ肷��
		m_nDist = 0;
	}
}

//========================
// �v���C���[�̈ړ�����
//========================
void CAnnounce::PlayerMove(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 pos = m_pPlayer->GetPos();		// �ʒu
	float fSize;								// �����̋����̊��������

	// �����̋����̊����𑪂�
	fSize = ROAD_SIZE.x / m_fTotalDist;

	// �ʒu�����݂̈ʒu�ɐݒ肷��
	pos.x = (m_apObject2D[POLY_ROAD]->GetPos().x + m_apObject2D[POLY_ROAD]->GetSize().x) - fSize * m_nDist;

	// �v���C���[�̈ʒu���X�V����
	m_pPlayer->SetPos(pos);

	// ���_�����X�V����
	m_pPlayer->SetVertex();

	// �A�j���[�V�����̍X�V����
	m_pPlayer->Update();
}

//========================
// ��������
//========================
CAnnounce* CAnnounce::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CAnnounce* pAnnou = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pAnnou == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pAnnou = new CAnnounce;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pAnnou != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pAnnou->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "���m�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pAnnou->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pAnnou;
}