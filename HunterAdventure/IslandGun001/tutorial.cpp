//============================================
//
// �`���[�g���A����ʂ̃��C������[tutorial.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "fade.h"
#include "camera.h"
#include "renderer.h"

#include "objectElevation.h"
#include "Objectmesh.h"
#include "motion.h"
#include "file.h"
#include "skybox.h"

#include "player_tutorial.h"
#include "tutorial_map.h"
#include "door.h"
#include "tutorial_airplane.h"
#include "block.h"

//--------------------------------------------
// �萔��`
//--------------------------------------------
namespace
{
	const int TRANS_COUNT = 280;			// �J�ڂ܂ł̃J�E���g��
	const char* ELEVATION_TXT = "data\\TXT\\ElevationTutorial.txt";		// �N���n�ʂ̃e�L�X�g
}

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CTutorialPlayer* CTutorial::m_pPlayer = nullptr;		// �v���C���[�̏��
CSignboard* CTutorial::m_pLook = nullptr;				// ���݌��Ă�Ŕ�
CDoor* CTutorial::m_pDoor = nullptr;					// �h�A�̏��
CTutorialAirplane* CTutorial::m_pAirplane = nullptr;	// ��s�@�̏��
CTutorial::STATE CTutorial::m_state = STATE_NONE;		// ���

//=========================================
// �R���X�g���N�^
//=========================================
CTutorial::CTutorial()
{
	// �S�Ă̏����N���A����
	m_nTransCount = 0;			// �J�ڃJ�E���g
	m_pPlayer = nullptr;		// �v���C���[�̏��
	m_pLook = nullptr;			// ���݌��Ă�Ŕ�
	m_pDoor = nullptr;			// �h�A�̏��
	m_pAirplane = nullptr;		// ��s�@�̏��
	m_state = STATE_NONE;		// ������
}

//=========================================
// �f�X�g���N�^
//=========================================
CTutorial::~CTutorial()
{

}

//=========================================
//����������
//=========================================
HRESULT CTutorial::Init(void)
{
	// �V�[���̏�����
	CScene::Init();

	// �J���������Z�b�g����
	CManager::Get()->GetCamera()->Reset();

	// �e�L�X�g�ǂݍ��ݏ���
	CElevation::TxtSet(ELEVATION_TXT);

	// ���[�V�����̓ǂݍ��ݏ���
	CMotion::Load(CMotion::STYLE_PLAYER);		// �v���C���[
	CMotion::Load(CMotion::STYLE_TORDLE);		// �^�[�h��
	CMotion::Load(CMotion::STYLE_IWAKARI);		// �C���J��
	CMotion::Load(CMotion::STYLE_BOSS);			// �{�X

	// �}�b�v�̐���
	CManager::Get()->GetFile()->SetBalloon();		// ���D
	CManager::Get()->GetFile()->SetSignboard();		// �Ŕ�

	CBlock::Create(D3DXVECTOR3(-800.0f, 0.0f, -1300.0f), NONE_SCALE);
	CBlock::Create(D3DXVECTOR3(-1000.0f, 250.0f, -1600.0f), NONE_SCALE);

	// �e�L�X�g�ǂݍ��ݏ���
	CMesh::TxtSet();

	// �X�J�C�{�b�N�X�̐�������
	CSkyBox::Create();

	// �`���[�g���A���}�b�v�̐���
	CTutorialMap::Create();

	// �v���C���[�𐶐�
	m_pPlayer = CTutorialPlayer::Create(NONE_D3DXVECTOR3);

	// �h�A�𐶐�
	m_pDoor = CDoor::Create(D3DXVECTOR3(0.0f, 0.0f, 1500.0f));

	// ��s�@�𐶐�
	m_pAirplane = CTutorialAirplane::Create();

	// �S�Ă̒l���N���A����
	m_nTransCount = 0;			// �J�ڃJ�E���g

	// ������Ԃ�
	return S_OK;
}

//=============================================
//�I������
//=============================================
void CTutorial::Uninit(void)
{
	// �S�Ă̒l���N���A����
	m_pPlayer = nullptr;		// �v���C���[�̃|�C���^
	m_pLook = nullptr;			// ���݌��Ă�Ŕ�
	m_pDoor = nullptr;			// �h�A�̏��
	m_pAirplane = nullptr;		// ��s�@�̏��
	m_state = STATE_NONE;		// ���

	// �I������
	CScene::Uninit();
}

//======================================
//�X�V����
//======================================
void CTutorial::Update(void)
{
	switch (m_state)
	{
	case CTutorial::STATE_NONE:

		if (CManager::Get()->GetRenderer() != nullptr)
		{ // �����_���[�� NULL ����Ȃ��ꍇ

			// �X�V����
			CManager::Get()->GetRenderer()->Update();
		}

		break;

	case CTutorial::STATE_EXPL:

		// �Ŕ̐����̂ݍX�V
		CObject::AnyUpdate(CObject::TYPE_SIGNEXPLAIN);

		break;

	case CTutorial::STATE_TRANS:

		// �J�ڃJ�E���g�����Z����
		m_nTransCount++;

		if (CManager::Get()->GetRenderer() != nullptr)
		{ // �����_���[�� NULL ����Ȃ��ꍇ

			// �X�V����
			CManager::Get()->GetRenderer()->Update();
		}

		if (m_nTransCount >= TRANS_COUNT)
		{ // �J�ڃJ�E���g����萔�𒴂����ꍇ

			// �Q�[���ɑJ�ڂ���
			CManager::Get()->GetFade()->SetFade(CScene::MODE_GAME);
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//======================================
//�`�揈��
//======================================
void CTutorial::Draw(void)
{

}

//======================================
// �v���C���[�̎擾����
//======================================
CTutorialPlayer* CTutorial::GetPlayer(void)
{
	// �v���C���[�̏���Ԃ�
	return m_pPlayer;
}

//======================================
// �h�A�̎擾����
//======================================
CDoor* CTutorial::GetDoor(void)
{
	// �h�A�̏���Ԃ�
	return m_pDoor;
}

//======================================
// ��Ԃ̐ݒ菈��
//======================================
void CTutorial::SetState(const STATE state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;
}

//======================================
// ��Ԃ̎擾����
//======================================
CTutorial::STATE CTutorial::GetState(void)
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//======================================
// �Ŕ̐ݒ菈��
//======================================
void CTutorial::SetLookSign(CSignboard* pSign)
{
	// �Ŕ̏���ݒ肷��
	m_pLook = pSign;
}

//======================================
// �Ŕ̎擾����
//======================================
CSignboard* CTutorial::GetLookSign(void)
{
	// �Ŕ̏���Ԃ�
	return m_pLook;
}

//======================================
// ��s�@�̎擾����
//======================================
CTutorialAirplane* CTutorial::GetAirplane(void)
{
	// ��s�@�̏���Ԃ�
	return m_pAirplane;
}

//======================================
// �v���C���[�̃v���C���[��NULL������
//======================================
void CTutorial::DeletePlayer(void)
{
	// �v���C���[�� NULL �ɂ���
	m_pPlayer = nullptr;
}

//======================================
// �h�A��NULL������
//======================================
void CTutorial::DeleteDoor(void)
{
	// �h�A�� NULL �ɂ���
	m_pDoor = nullptr;
}

//======================================
// ��s�@��NULL������
//======================================
void CTutorial::DeleteAirplane(void)
{
	// ��s�@�� NULL �ɂ���
	m_pAirplane = nullptr;
}