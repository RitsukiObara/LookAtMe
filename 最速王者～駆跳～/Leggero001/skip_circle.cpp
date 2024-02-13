//=======================================
//
// �X�L�b�v�~�̃��C������[skip_circle.cpp]
// Author ��������
//
//=======================================
#include "manager.h"
#include "scene.h"
#include "tutorial.h"
#include "skip_circle.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"
#include "input.h"

//=======================================
// �}�N����`
//=======================================
#define ADD_SIZE_DEST		(10.0f)							// �ڕW�̃T�C�Y�̒ǉ���
#define ADD_SIZE_CORRECT	(ADD_SIZE_DEST / 10)			// �T�C�Y�̕␳��

//=========================
// �R���X�g���N�^
//=========================
CSkipCircle::CSkipCircle() : CDonut2D(CObject::TYPE_SKIP, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_nVtx = 0;				// ���_��
	m_fSizeDest = 0.0f;		// �ړI�̃T�C�Y
	m_fSize = 0.0f;			// �T�C�Y(�ۑ��p)
	m_fDist = 0.0f;			// ����(�ۑ��p)
	m_fDistDest = 0.0f;		// �ړI�̋���
	m_bEnd = false;			// �I����
}

//=========================
// �f�X�g���N�^
//=========================
CSkipCircle::~CSkipCircle()
{

}

//=========================
// ����������
//=========================
HRESULT CSkipCircle::Init(void)
{
	if (FAILED(CDonut2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �F��ݒ肷��
	SetVtxColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// ������Ԃ�
	return S_OK;
}

//=========================
// �I������
//=========================
void CSkipCircle::Uninit(void)
{
	// �I��
	CDonut2D::Uninit();
}

//=========================
// �X�V����
//=========================
void CSkipCircle::Update(void)
{
	if (CManager::GetMode() == CScene::MODE_TUTORIAL)
	{ // �`���[�g���A�����[�h�̏ꍇ

		if ((CManager::GetInputKeyboard()->GetPress(DIK_RETURN) == true ||
			CManager::GetInputGamePad()->GetPress(CInputGamePad::JOYKEY_START,0) == true) &&
			m_nVtx < GetNumVtx())
		{ // ENTER�L�[���������ꍇ

			// �����Ă��鎞�̏���
			PushProcess();
		}
		else if (m_nVtx > 0)
		{ // �����Ă��Ȃ��ꍇ

			// �����Ă��Ȃ��Ƃ��̏���
			LeaveProcess();
		}

		// �I�����̏���
		EndProcess();
	}

	if (m_bEnd == true)
	{ // �I���󋵂� true �̏ꍇ

		// ���_����ݒ肷��
		m_nVtx = GetNumVtx();

		// �g�又��
		BigScale();
	}

	// �T�C�Y�␳����
	SizeCorrect();

	if (m_nVtx <= 0)
	{ // ���_���� 0 �ȉ��̏ꍇ

		// �I������
		Uninit();

		// ���̐�̏������s��Ȃ�
		return;
	}

	// ���_���W�̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��(�͈͐ݒ�)
	SetVtxColor(m_nVtx, D3DXCOLOR(0.3f, 0.3f, 1.0f, 1.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));

	// �e�N�X�`�����W�̐ݒ菈��
	SetVtxTexture();
}

//=========================
// �`�揈��
//=========================
void CSkipCircle::Draw(void)
{
	// �`�揈��
	CDonut2D::Draw();
}

//=========================
// ���̐ݒ菈��
//=========================
void CSkipCircle::SetData(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
{
	// �S�Ă̒l��ݒ肷��
	SetPos(pos);			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetSize(fSize);			// �T�C�Y
	SetDistance(fDist);		// ����
	SetDivi(nDivi);			// ������
	SetNumVtx();			// �����_��

	// �S�Ă̒l��ݒ肷��
	m_nVtx = 0;				// ���_��
	m_fSize = fSize;		// �T�C�Y(�ۑ��p)
	m_fSizeDest = fSize;	// �ړI�̃T�C�Y
	m_fDist = fDist;		// ����(�ۑ��p)
	m_fDistDest = fDist;	// �ړI�̋���
	m_bEnd = false;			// �I����
}

//=========================
// ��������
//=========================
CSkipCircle* CSkipCircle::Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CSkipCircle* pSkipCircle = nullptr;	// �v���C���[�̃C���X�^���X�𐶐�

	if (pSkipCircle == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pSkipCircle = new CSkipCircle;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pSkipCircle != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ���̐ݒ菈��
		pSkipCircle->SetData(pos, fSize, fDist, nDivi);

		// ����������
		if (FAILED(pSkipCircle->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�G�t�F�N�g2D�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �I�u�W�F�N�g2D�̃|�C���^��Ԃ�
	return pSkipCircle;
}

//=========================
// �����Ă��鎞�̏���
//=========================
void CSkipCircle::PushProcess(void)
{
	// ���_�������Z����
	m_nVtx++;

	// �g�又��
	BigScale();
}

//=========================
// �����Ă��Ȃ��Ƃ��̏���
//=========================
void CSkipCircle::LeaveProcess(void)
{
	// ���_�������Z����
	m_nVtx--;

	// �ڕW�̃T�C�Y��ݒ肷��
	m_fSizeDest = m_fSize;

	// �ڕW�̋�����ݒ肷��
	m_fDistDest = m_fDist;
}

//=========================
// �I�����̏���
//=========================
void CSkipCircle::EndProcess(void)
{
	if (m_nVtx >= GetNumVtx())
	{ // �����_���ȏ�ɂȂ����ꍇ

		// ���_����ݒ肷��
		m_nVtx = GetNumVtx();

		// ��Ԃ�ݒ肷��
		CTutorial::SetState(CTutorial::STATE_SKIP);

		// �I���󋵂� true �ɂ���
		m_bEnd = true;
	}
}

//=========================
// �g�又��
//=========================
void CSkipCircle::BigScale(void)
{
	// �ڕW�̃T�C�Y��ݒ肷��
	m_fSizeDest = m_fSize + ADD_SIZE_DEST;

	// �ڕW�̋�����ݒ肷��
	m_fDistDest = m_fDist + ADD_SIZE_DEST;
}

//=========================
// �T�C�Y�̕␳����
//=========================
void CSkipCircle::SizeCorrect(void)
{
	// ���[�J���ϐ��錾
	float fSize = GetSize();		// �T�C�Y
	float fDist = GetDistance();	// ����

	// ���̕␳����
	FrameCorrect(m_fSizeDest, &fSize, ADD_SIZE_CORRECT);		// �T�C�Y
	FrameCorrect(m_fDistDest, &fDist, ADD_SIZE_CORRECT);	// ����

	// ���̐ݒ菈��
	SetSize(fSize);			// �T�C�Y�ݒ菈��
	SetDistance(fDist);		// �����̐ݒ菈��
}