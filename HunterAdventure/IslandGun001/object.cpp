//============================================
//
// �I�u�W�F�N�g�̃��C������[object.cpp]
// Author�F��������
//
//============================================
//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "object.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "pause.h"

//--------------------------------------------
// �ÓI�����o�ϐ��錾
//--------------------------------------------
CObject* CObject::m_apTop[DIM_MAX][PRIORITY_MAX] = {};	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObject* CObject::m_apCur[DIM_MAX][PRIORITY_MAX] = {};	// �Ō���I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nNumAll = 0;								// �I�u�W�F�N�g�̑���

//=========================================
// �I�[�o�[���[�h�R���X�g���N�^
//=========================================
CObject::CObject(TYPE type, DIM dim, PRIORITY priority)
{
	// �S�Ă̒l������������
	m_pPrev = nullptr;			// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	m_pNext = nullptr;			// ���̃I�u�W�F�N�g�ւ̃|�C���^
	m_type = type;				// ���
	m_priority = priority;		// �I�u�W�F�N�g�̗D�揇��
	m_dim = dim;				// ����
	m_bDeath = false;			// ���S�t���O

	if (m_apTop[dim][priority] == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �擪�̃|�C���^�Ɏ������g����
		m_apTop[dim][priority] = this;

		// �Ō���̃|�C���^�Ɏ������g����
		m_apCur[dim][priority] = this;

		// ��ނ�ݒ肷��
		m_type = type;

		// �I�u�W�F�N�g�̗D�揇�ʂ�ݒ肷��
		m_priority = priority;

		// �O�̃I�u�W�F�N�g�ւ̃|�C���^��ݒ肷��
		m_pPrev = nullptr;

		// ���̃I�u�W�F�N�g�ւ̃|�C���^��ݒ肷��
		m_pNext = nullptr;

		// �������J�E���g�A�b�v
		m_nNumAll++;
	}
	else
	{ // ��L�ȊO

		// ���[�J���|�C���^��錾
		CObject* pObject = m_apTop[dim][priority];			// ���̃I�u�W�F�N�g

		while (pObject->m_pNext != nullptr)
		{ // �I�u�W�F�N�g�����������

			// ���̃I�u�W�F�N�g��������
			pObject = pObject->m_pNext;
		}

		// ��r�I�u�W�F�N�g�̎��̃I�u�W�F�N�g�Ɏ������g��������
		pObject->m_pNext = this;

		// ���̃I�u�W�F�N�g�� NULL ������
		m_pNext = nullptr;

		// ���݂̔�r�I�u�W�F�N�g�������̑O�̃I�u�W�F�N�g�ɐݒ肷��
		m_pPrev = pObject;

		// ��ނ�ݒ肷��
		m_type = type;

		// �I�u�W�F�N�g�̗D�揇�ʂ�ݒ肷��
		m_priority = priority;

		// �������g���Ō���̃I�u�W�F�N�g�ɐݒ肷��
		m_apCur[dim][priority] = this;

		// �������J�E���g�A�b�v
		m_nNumAll++;
	}
}

//=========================================
// �f�X�g���N�^
//=========================================
CObject::~CObject()
{

}

#if 0

//===========================================
// �I�u�W�F�N�g�̏���������
//===========================================
HRESULT CObject::Init(void)
{

}

//===========================================
// �I�u�W�F�N�g�̏I������
//===========================================
void CObject::Uninit(void)
{

}

//===========================================
// �I�u�W�F�N�g�̍X�V����
//===========================================
void CObject::Update(void)
{

}

//===========================================
// �I�u�W�F�N�g�̕`�揈��
//===========================================
void CObject::Draw(void)
{

}

#endif

//===========================================
// �I�u�W�F�N�g�̎擾����
//===========================================
CObject* CObject::GetTop(DIM dim, PRIORITY priority)
{
	// �I�u�W�F�N�g�̏���n��
	return m_apTop[dim][priority];
}

//===========================================
// ���̃|�C���^�̎擾����
//===========================================
CObject* CObject::GetNext(void)
{
	// ���̃I�u�W�F�N�g��Ԃ�
	return m_pNext;
}

//===========================================
// ��ނ̐ݒ菈��
//===========================================
void CObject::SetType(const TYPE type)
{
	// ��ނ�ݒ肷��
	m_type = type;
}

//===========================================
// ��ނ̎擾����
//===========================================
CObject::TYPE CObject::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//===========================================
// �S�ẴI�u�W�F�N�g�̔j��
//===========================================
void CObject::ReleaseAll(void)
{
	// �I�u�W�F�N�g�̃|�C���^��錾
	CObject* pObj = nullptr;			// ���݂̃I�u�W�F�N�g
	CObject* pObjNext = nullptr;		// ���̃I�u�W�F�N�g

	for (int nCntDim = 0; nCntDim < DIM_MAX; nCntDim++)
	{
		for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
		{
			// ���݂̃I�u�W�F�N�g�ɐ擪�̃I�u�W�F�N�g��������
			pObj = m_apTop[nCntDim][nCntPri];

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				// ���̃I�u�W�F�N�g��������
				pObjNext = pObj->m_pNext;

				if (pObj->m_type != TYPE_NONE)
				{ // ��ޖ����ȊO�̏ꍇ

					// �I������
					pObj->Uninit();
				}

				// ���̃I�u�W�F�N�g��ݒ肷��
				pObj = pObjNext;
			}

			for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
			{
				// ���S���菈��
				DeathDecision(nCntDim, nCnt);
			}
		}
	}
}

//======================================
// �ėp�I�ȑS��������
//======================================
void CObject::AnyAllClear(const TYPE type)
{
	// �I�u�W�F�N�g�̃|�C���^��錾
	CObject* pObj = nullptr;			// ���݂̃I�u�W�F�N�g
	CObject* pObjNext = nullptr;		// ���̃I�u�W�F�N�g

	for (int nCntDim = 0; nCntDim < DIM_MAX; nCntDim++)
	{
		for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
		{
			// ���݂̃I�u�W�F�N�g�ɐ擪�̃I�u�W�F�N�g��������
			pObj = m_apTop[nCntDim][nCntPri];

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				// ���̃I�u�W�F�N�g��������
				pObjNext = pObj->m_pNext;

				if (pObj->GetType() == type)
				{ // �w�肳�ꂽ��ނ������ꍇ

					// �I������
					pObj->Uninit();
				}

				// ���̃I�u�W�F�N�g��ݒ肷��
				pObj = pObjNext;
			}

			for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
			{
				// ���S���菈��
				DeathDecision(nCntDim, nCnt);
			}
		}
	}
}

//===========================================
// �ėp�I�ȑS��������
//===========================================
void CObject::AnyAllClear(const DIM dim, const PRIORITY priority, const TYPE type)
{
	// �I�u�W�F�N�g�̃|�C���^��錾
	CObject* pObj = nullptr;			// ���݂̃I�u�W�F�N�g
	CObject* pObjNext = nullptr;		// ���̃I�u�W�F�N�g

	// ���݂̃I�u�W�F�N�g�ɐ擪�̃I�u�W�F�N�g��������
	pObj = m_apTop[dim][priority];

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		// ���̃I�u�W�F�N�g��������
		pObjNext = pObj->m_pNext;

		if (pObj->GetType() == type)
		{ // �w�肳�ꂽ��ނ������ꍇ

			// �I������
			pObj->Uninit();
		}

		// ���̃I�u�W�F�N�g��ݒ肷��
		pObj = pObjNext;
	}

	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		// ���S���菈��
		DeathDecision(dim, nCnt);
	}
}

//===========================================
// ��ތ���̍X�V����
//===========================================
void CObject::AnyUpdate(const TYPE type)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// ���݂̃I�u�W�F�N�g�̃|�C���^
	CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g�̃|�C���^

	for (int nCntDim = 0; nCntDim < DIM_MAX; nCntDim++)
	{
		for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
		{
			// �I�u�W�F�N�g��������
			pObj = m_apTop[nCntDim][nCntPri];

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				// ���̃I�u�W�F�N�g��������
				pObjNext = pObj->m_pNext;

				if (pObj->GetType() == type &&
					pObj->m_bDeath == false)
				{ // �I�u�W�F�N�g�̎�ނ� NONE��PAUSE�ȊO���A���S�t���O��false�̏ꍇ

					// �I�u�W�F�N�g�̍X�V
					pObj->Update();

					if (pObj != nullptr &&
						pObj->m_pNext == nullptr)
					{ // ���̃I�u�W�F�N�g�� NULL �̏ꍇ

						// ���̃I�u�W�F�N�g�� NULL �ɂ���
						pObjNext = nullptr;
					}
				}

				// ���̃I�u�W�F�N�g��������
				pObj = pObjNext;
			}
		}

		for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
		{
			// ���S���菈��
			DeathDecision(nCntDim, nCnt);
		}
	}
}

//===========================================
// �S�Ă̍X�V����
//===========================================
void CObject::UpdateAll(void)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// ���݂̃I�u�W�F�N�g�̃|�C���^
	CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g�̃|�C���^

	for (int nCntDim = 0; nCntDim < DIM_MAX; nCntDim++)
	{
		for (int nCntPri = 0; nCntPri < PRIORITY_MAX; nCntPri++)
		{
			// �I�u�W�F�N�g��������
			pObj = m_apTop[nCntDim][nCntPri];

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				// ���̃I�u�W�F�N�g��������
				pObjNext = pObj->m_pNext;

				if (pObj->GetType() != TYPE_NONE &&
					pObj->m_bDeath == false)
				{ // �I�u�W�F�N�g�̎�ނ� NONE��PAUSE�ȊO���A���S�t���O��false�̏ꍇ

					// �I�u�W�F�N�g�̍X�V
					pObj->Update();

					if (pObj != nullptr &&
						pObj->m_pNext == nullptr)
					{ // ���̃I�u�W�F�N�g�� NULL �̏ꍇ

						// ���̃I�u�W�F�N�g�� NULL �ɂ���
						pObjNext = nullptr;
					}
				}

				// ���̃I�u�W�F�N�g��������
				pObj = pObjNext;
			}
		}

		for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
		{
			// ���S���菈��
			DeathDecision(nCntDim, nCnt);
		}
	}
}

//===========================================
// �S�Ă̕`�揈��
//===========================================
void CObject::DrawAll(void)
{
	// �J�����̐ݒ菈��
	CManager::Get()->GetCamera()->Set();

	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// ���݂̃I�u�W�F�N�g�̃|�C���^

	for (int nCntDim = 0; nCntDim < DIM_MAX; nCntDim++)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			// �擪�̃I�u�W�F�N�g��ݒ肷��
			pObj = m_apTop[nCntDim][nCntPriority];

			while (pObj != nullptr)
			{ // �I�u�W�F�N�g�� NULL ����Ȃ������

				if (pObj->GetType() != TYPE_NONE)
				{ // ��ނ� �����E�|�[�Y �ȊO�̏ꍇ

					// �I�u�W�F�N�g�̕`��
					pObj->Draw();
				}

				// ���̃I�u�W�F�N�g����
				pObj = pObj->m_pNext;
			}
		}
	}
}

//===========================================
// �I�u�W�F�N�g�̔j������
//===========================================
void CObject::Release(void)
{
	// ���S�t���O�𗧂Ă�
	m_bDeath = true;
}

//===========================================
// ���S����
//===========================================
void CObject::Death(void)
{
	// ���[�J���ϐ���錾
	int nPriority = m_priority;		// �D�揇�ʂ̕ۑ��ϐ�
	int nDim = m_dim;				// �����̕ۑ��ϐ�
	CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g
	CObject* pObjPrev = nullptr;	// �O�̃I�u�W�F�N�g

	if (m_apTop[nDim][nPriority] == this)
	{ // �擪�̃I�u�W�F�N�g�������ꍇ

		if (m_apTop[nDim][nPriority]->m_pNext != nullptr)
		{ // ���̃I�u�W�F�N�g�����݂����ꍇ

			// ���̃I�u�W�F�N�g��ݒ肷��
			pObjNext = m_apTop[nDim][nPriority]->m_pNext;
		}

		// �O�̃I�u�W�F�N�g�͑��݂��Ȃ��̂� NULL �ɂ���
		pObjPrev = nullptr;

		// �I�u�W�F�N�g�ɐݒ肷��
		m_apTop[nDim][nPriority] = pObjNext;

		if (m_apCur[nDim][nPriority] == this)
		{ // �Ō�����������g�������ꍇ

			// �Ō���� NULL �ɂ���
			m_apCur[nDim][nPriority] = nullptr;
		}

		// ���������J������
		delete this;

		if (pObjNext != nullptr)
		{ // ���̃I�u�W�F�N�g�����݂����ꍇ

			// ���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g(�������g)�� NULL �ɂ���
			pObjNext->m_pPrev = nullptr;
		}

		// �������J�E���g�_�E������
		m_nNumAll--;
	}
	else if (m_apCur[nDim][nPriority] == this)
	{ // �Ō���̃I�u�W�F�N�g�������ꍇ

		// ���̃I�u�W�F�N�g�͑��݂��Ȃ��̂� NULL �ɂ���
		pObjNext = nullptr;

		// �O�̃I�u�W�F�N�g��ݒ肷��
		pObjPrev = m_apCur[nDim][nPriority]->m_pPrev;

		// �I�u�W�F�N�g�ɐݒ肷��
		m_apCur[nDim][nPriority] = pObjPrev;

		// ���������J������
		delete this;

		// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g(�������g)�� NULL �ɂ���
		pObjPrev->m_pNext = nullptr;

		// �������J�E���g�_�E������
		m_nNumAll--;
	}
	else
	{ // ��L�ȊO(���Ԃ̃I�u�W�F�N�g)�̏ꍇ

		// ���̃I�u�W�F�N�g�͑��݂��Ȃ��̂� NULL �ɂ���
		pObjNext = m_pNext;

		// �O�̃I�u�W�F�N�g��ݒ肷��
		pObjPrev = m_pPrev;

		// �O�̃I�u�W�F�N�g�̎��̃I�u�W�F�N�g��u��������
		pObjPrev->m_pNext = pObjNext;

		// ���̃I�u�W�F�N�g�̑O�̃I�u�W�F�N�g��u��������
		pObjNext->m_pPrev = pObjPrev;

		// ���������J������
		delete this;

		// �������J�E���g�_�E������
		m_nNumAll--;
	}
}

//===========================================
// ���S���ʏ���
//===========================================
void CObject::DeathDecision(const int dim, const int nCnt)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;		// ���݂̃I�u�W�F�N�g�̃|�C���^
	CObject* pObjNext = nullptr;	// ���̃I�u�W�F�N�g�̃|�C���^

	// �I�u�W�F�N�g��������
	pObj = m_apTop[dim][nCnt];

	while (pObj != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ������

		// ���̃I�u�W�F�N�g��������
		pObjNext = pObj->m_pNext;

		if (pObj->m_bDeath == true)
		{ // �I�u�W�F�N�g�Ɏ��S�t���O�������Ă����ꍇ

			if (pObj->m_pPrev != nullptr)
			{ // �O�̃I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

				// ���X�g�\�����玩���𔲂��o��
				pObj->m_pPrev->m_pNext = pObj->m_pNext;
			}

			if (pObj->m_pNext != nullptr)
			{ // ���̃I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

				// ���X�g�\�����玩���𔲂��o��
				pObj->m_pNext->m_pPrev = pObj->m_pPrev;
			}

			// ���S(�j��)����
			pObj->Death();
			pObj = nullptr;
		}

		// ���̃I�u�W�F�N�g��������
		pObj = pObjNext;
	}
}