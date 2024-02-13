//===========================================
//
// �l�p�e�̃��C������[shadowCircle.cpp]
// Author ��������
//
//===========================================
#include "main.h"
#include "shadowSquare.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//-------------------------------------------
// �ÓI�����o�ϐ��錾
//-------------------------------------------
int CShadowSquare::m_nNumAll = 0;				// �e�̑���

//====================
// �R���X�g���N�^
//====================
CShadowSquare::CShadowSquare() : CObject3D(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// �S�Ă̒l���N���A����
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_nID = INIT_SHADOW;						// �e�̃C���f�b�N�X
}

//====================
// �f�X�g���N�^
//====================
CShadowSquare::~CShadowSquare()
{

}

//====================
// ����������
//====================
HRESULT CShadowSquare::Init(void)
{
	// ����������
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_nID = INIT_SHADOW;						// �e�̃C���f�b�N�X

	// ������Ԃ�
	return S_OK;
}

//====================
// �I������
//====================
void CShadowSquare::Uninit(void)
{
	// �I�u�W�F�N�g�̏I������
	CObject3D::Uninit();

	// �e�̑��������Z����
	m_nNumAll--;
}

//====================
// �X�V����
//====================
void CShadowSquare::Update(void)
{

}

//====================
// �`�揈��
//====================
void CShadowSquare::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// �`�揈��
	CObject3D::Draw();

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//====================
// ���̐ݒ菈��
//====================
void CShadowSquare::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// ���̏�����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_nID = m_nNumAll;								// �ԍ�

	// �e�̑��������Z����
	m_nNumAll++;

	// ���̐ݒ�
	SetPos(D3DXVECTOR3(pos.x, 0.0f, pos.z));			// �ʒu
	SetPosOld(pos);			// �O��̈ʒu
	SetRot(rot);			// ����
	SetSize(size);			// �T�C�Y

	// �ݒ菈��
	AlphaSet();

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);
}

//====================
// �����x�̐ݒ菈��
//====================
void CShadowSquare::AlphaSet(void)
{

}

//====================
// ID�̎擾����
//====================
int CShadowSquare::GetNumID(void) const
{
	// �e�̔ԍ���Ԃ�
	return m_nID;
}

//====================
// �l�p�e�̎擾����
//====================
CShadowSquare* CShadowSquare::GetShadow(int nIdx)
{
	// ���[�J���ϐ��錾
	CObject* pObj = nullptr;			// �I�u�W�F�N�g�̃|�C���^
	CShadowSquare* pShadow = nullptr;	// �e�̃|�C���^

	if (nIdx != INIT_SHADOW)
	{ // �e�̃C���f�b�N�X������ꍇ

		// �I�u�W�F�N�g�̏����擾����
		pObj = GetTop(PRIORITY_SHADOW);

		while (pObj != nullptr)
		{ // �I�u�W�F�N�g�̃|�C���^�� NULL ����Ȃ������

			if (pObj->GetType() == TYPE_SHADOW)
			{ // ��ނ��e�̏ꍇ

				// �e�̏����擾����
				pShadow = dynamic_cast<CShadowSquare*>(pObj);

				if (pShadow != nullptr)
				{ // �e�̃|�C���^�� NULL ����Ȃ��ꍇ

					if (pShadow->GetNumID() == nIdx)
					{ // �e�̔ԍ��������ƈ�v�����ꍇ

						// �e�̃|�C���^��Ԃ�
						return pShadow;
					}
					else
					{ // ��L�ȊO

						// ���̃I�u�W�F�N�g��ݒ肷��
						pObj = pObj->GetNext();
					}
				}
				else
				{ // �e�̃|�C���^�� NULL �̏ꍇ

					// ���̃I�u�W�F�N�g��ݒ肷��
					pObj = pObj->GetNext();
				}
			}
			else
			{ // ��L�ȊO

				// ���̃I�u�W�F�N�g��ݒ肷��
				pObj = pObj->GetNext();
			}
		}

		// NULL ��Ԃ�
		return nullptr;
	}
	else
	{ // ��L�ȊO

		// NULL ��Ԃ�
		return nullptr;
	}
}

//====================
// ��������
//====================
CShadowSquare* CShadowSquare::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& size)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CShadowSquare* pShadow = nullptr;	// �e�̃C���X�^���X�𐶐�

	if (pShadow == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pShadow = new CShadowSquare;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pShadow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �x����
			MessageBox(NULL, "�l�p�e�̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pShadow->SetData(pos, rot, size);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// NULL ��Ԃ�
		return nullptr;
	}

	// �e�̃|�C���^��Ԃ�
	return pShadow;
}