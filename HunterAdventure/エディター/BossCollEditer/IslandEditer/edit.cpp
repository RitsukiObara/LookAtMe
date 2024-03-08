//===========================================
//
// �G�f�B�b�g�̃��C������[edit.cpp]
// Author ��������
//
//===========================================
//*******************************************
// �C���N���[�h�t�@�C��
//*******************************************
#include "main.h"
#include "manager.h"
#include "edit.h"
#include "renderer.h"
#include "useful.h"
#include "file.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "boss.h"
#include "boss_collision.h"
#include "game.h"

// �������O���
namespace
{
	
}

//==============================
// �R���X�g���N�^
//==============================
CEdit::CEdit() : CObject(CObject::TYPE_EDIT, DIM_3D, CObject::PRIORITY_UI)
{
	// �S�Ă̒l���N���A����
	m_pBoss = nullptr;
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apColl[nCnt] = nullptr;
	}
	m_nSetPartNumber = 0;
	m_nSetCollNumber = 0;
}

//==============================
// �f�X�g���N�^
//==============================
CEdit::~CEdit()
{

}

//==============================
//�u���b�N�̏���������
//==============================
HRESULT CEdit::Init(void)
{
	m_pBoss = CBoss::Create(NONE_D3DXVECTOR3, NONE_D3DXVECTOR3);

	// �l��Ԃ�
	return S_OK;
}

//========================================
//�u���b�N�̏I������
//========================================
void CEdit::Uninit(void)
{
	if (m_pBoss != nullptr)
	{ // ���f���� NULL ����Ȃ��ꍇ

		// �I������
		m_pBoss = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apColl[nCnt] != nullptr)
		{
			m_apColl[nCnt]->Uninit();
			m_apColl[nCnt] = nullptr;
		}
	}

	// �I������
	Release();

	// �G�f�B�b�g�̏�������
	CGame::DeleteEdit();
}

//========================================
//�u���b�N�̍X�V����
//========================================
void CEdit::Update(void)
{
	// Window�̃^�C�g��
	ImGui::Begin("Editer");

	// �ԍ��̐ݒ菈��
	SetIdx();

	// �ԍ��̕␳����
	CorrectIdx();

	// �ʒu�ݒ菈��
	Positioning();

	// ���a�ݒ菈��
	SetRadius();

	// ��_�󋵂̐ݒ菈��
	SetWeakness();

	// �����蔻��̒ǉ�����
	AddCollision();
	
	// �����蔻��̑����\������
	NumCollDisp();

	// �����蔻��̃}�g���b�N�X�|�����킹
	CollMatrix();

	if (ImGui::Button("Save") == true)
	{ // �����蔻��̒ǉ����������ꍇ

		CManager::Get()->GetFile()->Save(CFile::TYPE_BOSSCOLL);
	}

	//// �e�L�X�g�̕\��
	//ImGui::Text("Obara is Stupid");

	//// �`�F�b�N�{�b�N�X�̕\��
	//ImGui::Checkbox("Y/N", &bObaraNew);

	//// �e�L�X�g�̕\��
	//ImGui::Text("DonkuraiKusa?");

	//// INT�^�̃X���C�_�[
	//if (ImGui::SliderInt("Level", &nGlassLevel, 0, 6000) == true)
	//{ // ���������ꍇ

	//}

	//// float�^3�̃X���C�_�[
	//if (ImGui::SliderFloat3("position", (float*)&f, -4000.0f, 4000.0f))
	//{
	//	int n = 0;
	//}

	//// �{�^���̕\��
	//if (ImGui::Button("KusaHayashi") == true)
	//{ // �{�^�����������ꍇ

	//	nGlassLevel++;
	//}
	//// ��ɑ����e�L�X�g�𓯂��s�ɕ\������
	//ImGui::SameLine();

	//// �e�L�X�g��\������
	//ImGui::Text("KusaLevel = %d", nGlassLevel);

	// ImGui�̕\���̏I��
	ImGui::End();
}

//=====================================
//�u���b�N�̕`�揈��
//=====================================
void CEdit::Draw(void)
{
	if (m_pBoss != nullptr)
	{ // ���f���̏�� NULL ����Ȃ��ꍇ

		// �`�揈��
		m_pBoss->Draw();
	}

	for (int nCnt = 0; nCnt < m_pBoss->GetNumModel(); nCnt++)
	{
		if (m_apColl[nCnt] != nullptr)
		{ // ���f���� NULL ����Ȃ��ꍇ

			// �`�揈��
			m_apColl[nCnt]->Draw(m_pBoss, nCnt);
		}
	}
}

//=====================================
// ���̐ݒ菈��
//=====================================
void CEdit::SetData(void)
{
	CManager::Get()->GetFile()->SetBossColl(&m_apColl[0]);
}

//=======================================
// ��������
//=======================================
CEdit* CEdit::Create(void)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CEdit* pEdit = nullptr;		// �G�f�B�b�g�𐶐�

	if (pEdit == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �C���X�^���X�𐶐�
		pEdit = new CEdit;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pEdit != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pEdit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pEdit->SetData();
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �G�f�B�b�g�̃|�C���^��Ԃ�
	return pEdit;
}

//===========================================
// �����蔻��̃}�g���b�N�X
//===========================================
void CEdit::CollMatrix(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;
	CBossCollision* coll = nullptr;

	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld = m_pBoss->GetMatrix();		// �}�g���b�N�X���擾����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_pBoss->GetScale().x, m_pBoss->GetScale().y, m_pBoss->GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_pBoss->GetRot().y + D3DX_PI, m_pBoss->GetRot().x, m_pBoss->GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pBoss->GetPos().x, m_pBoss->GetPos().y, m_pBoss->GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	for (int nCntPart = 0; nCntPart < m_pBoss->GetNumModel(); nCntPart++)
	{
		if (m_apColl[nCntPart] != nullptr)
		{
			// �}�g���b�N�X�̌v�Z����
			m_pBoss->GetHierarchy(nCntPart)->MatrixCalc(&mtx, mtxWorld);

			for (int nCntColl = 0; nCntColl < m_apColl[nCntPart]->GetNumColl(); nCntColl++)
			{
				coll = m_apColl[nCntPart];

				// �}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&mtxColl);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, coll->GetCollOffset(nCntColl).x, coll->GetCollOffset(nCntColl).y, coll->GetCollOffset(nCntColl).z);
				D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

				// �Z�o�����u�p�[�c�̃��[���h�}�g���b�N�X�v�Ɓu�e�̃}�g���b�N�X�v���|�����킹��
				D3DXMatrixMultiply
				(
					&mtxColl,
					&mtxColl,
					&mtx
				);

				// �ʒu��ݒ肷��
				posPart.x = mtxColl._41;
				posPart.y = mtxColl._42;
				posPart.z = mtxColl._43;
			}
		}
	}
}

//===========================================
// �{�X�̎擾����
//===========================================
CBoss* CEdit::GetBoss(void)
{
	// �{�X�̏���Ԃ�
	return m_pBoss;
}

//===========================================
// �����蔻��̏��̎擾����
//===========================================
CBossCollision* CEdit::GetColl(const int nIdx)
{
	// �����蔻��̏���Ԃ�
	return m_apColl[nIdx];
}

//===========================================
// �ʒu�ݒ菈��
//===========================================
void CEdit::Positioning(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // �����蔻�肪 NULL ����Ȃ��ꍇ

		D3DXVECTOR3 pos = m_apColl[m_nSetPartNumber]->GetCollOffset(m_nSetCollNumber);

		// �ʒu�̐ݒ�
		ImGui::DragFloat3("Position", (float*)&pos, 1.0f, -10000.0f, 10000.0f);

		// �ʒu��K�p
		m_apColl[m_nSetPartNumber]->SetCollOffset(pos, m_nSetCollNumber);
	}
}

//===========================================
// �ԍ��̐ݒ菈��
//===========================================
void CEdit::SetIdx(void)
{
	ImGui::DragInt("SetPartNumber", &m_nSetPartNumber, 1, 0, m_pBoss->GetNumModel() - 1);

	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // �����蔻�肪 NULL ����Ȃ��ꍇ

		ImGui::DragInt("SetCollNumber", &m_nSetCollNumber, 1, 0, m_apColl[m_nSetPartNumber]->GetNumColl() - 1);
	}
}

//===========================================
// �ԍ��̕␳����
//===========================================
void CEdit::CorrectIdx(void)
{
	// �p�[�c�ԍ��̕␳
	if (m_nSetPartNumber >= m_pBoss->GetNumModel())
	{
		m_nSetPartNumber = m_pBoss->GetNumModel() - 1;
	}
	if (m_nSetPartNumber < 0)
	{
		m_nSetPartNumber = 0;
	}

	// �����蔻��ԍ��̕␳
	if (m_apColl[m_nSetPartNumber] != nullptr &&
		m_nSetCollNumber >= m_apColl[m_nSetPartNumber]->GetNumColl())
	{
		m_nSetCollNumber = m_apColl[m_nSetPartNumber]->GetNumColl() - 1;
	}
	if (m_nSetCollNumber < 0)
	{
		m_nSetCollNumber = 0;
	}
}

//===========================================
// �����蔻��̒ǉ�����
//===========================================
void CEdit::AddCollision(void)
{
	if (ImGui::Button("Add Collision") == true)
	{ // �����蔻��̒ǉ����������ꍇ

		if (m_apColl[m_nSetPartNumber] == nullptr)
		{ // �����蔻��̋��� NULL �̏ꍇ

			// �����蔻��𐶐�
			m_apColl[m_nSetPartNumber] = CBossCollision::Create(0);
		}

		if (m_apColl[m_nSetPartNumber]->GetNumColl() < CBossCollision::MAX_COLLISION)
		{ // �ő�l����Ȃ��ꍇ

			int nNumColl = m_apColl[m_nSetPartNumber]->GetNumColl();

			// ���f���𐶐�
			m_apColl[m_nSetPartNumber]->SetModel(nNumColl);

			nNumColl++;

			// ����ݒ肷��
			m_apColl[m_nSetPartNumber]->SetNumColl(nNumColl);
		}
	}
}

//===========================================
// �����蔻��̑����\������
//===========================================
void CEdit::NumCollDisp(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // �����蔻�肪 NULL ����Ȃ��ꍇ

		if (m_apColl[m_nSetPartNumber]->GetNumColl() >= CBossCollision::MAX_COLLISION)
		{ // �����蔻�肪�ő�̏ꍇ

			// �e�L�X�g��\������
			ImGui::Text("NumColl = MAX");
		}
		else
		{ // ��L�ȊO

			// �e�L�X�g��\������
			ImGui::Text("NumColl = %d", m_apColl[m_nSetPartNumber]->GetNumColl());
		}
	}
	else
	{ // ��L�ȊO

		// �e�L�X�g��\������
		ImGui::Text("404Collision");
	}
}

//===========================================
// �����蔻��̔��a�̐ݒ菈��
//===========================================
void CEdit::SetRadius(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // �����蔻�肪 NULL ����Ȃ��ꍇ

		float fRadius = m_apColl[m_nSetPartNumber]->GetRadius(m_nSetCollNumber);

		// �ʒu�̐ݒ�
		ImGui::DragFloat("Radius", &fRadius, 1.0f, 1.0f, 10000.0f);

		// �ʒu��K�p
		m_apColl[m_nSetPartNumber]->SetRadius(fRadius, m_nSetCollNumber);
	}
}

//===========================================
// �����蔻��̎�_�󋵂̐ݒ菈��
//===========================================
void CEdit::SetWeakness(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // �����蔻�肪 NULL ����Ȃ��ꍇ

		if (ImGui::Button("Weakness") == true)
		{ // ��_�󋵂̃{�^�����������ꍇ

			// ��_�󋵂��擾
			bool bWeak = m_apColl[m_nSetPartNumber]->GetWeakness(m_nSetCollNumber);

			// ��_�󋵂�؂�ւ���
			bWeak = !bWeak;

			// ��_�󋵂�K�p
			m_apColl[m_nSetPartNumber]->SetWeakness(bWeak, m_nSetCollNumber);
		}

		// ��ɑ����e�L�X�g�𓯂��s�ɕ\������
		ImGui::SameLine();

		if (m_apColl[m_nSetPartNumber]->GetWeakness(m_nSetCollNumber) == true)
		{ // ��_�̏ꍇ

			ImGui::Text("TRUE");
		}
		else
		{ // ��L�ȊO

			ImGui::Text("FALSE");
		}
	}
}