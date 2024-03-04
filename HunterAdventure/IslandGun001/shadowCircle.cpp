//===========================================
//
// �ۉe�̃��C������[shadowCircle.cpp]
// Author ��������
//
//===========================================
#include "main.h"
#include "shadowCircle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"
#include "useful.h"

#include "objectElevation.h"
#include "rock.h"
#include "block.h"

//-------------------------------------------
// �������O���
//-------------------------------------------
namespace
{
	const D3DXCOLOR INIT_SHADOW_COL = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);	// �����̉e�̐F
	const char* SHADOW_TEXTURE = "data\\TEXTURE\\shadow.jpg";				// �e�̃e�N�X�`��
}

//====================
// �R���X�g���N�^
//====================
CShadowCircle::CShadowCircle() : CObject3D(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// �S�Ă̒l���N���A����
	m_col = INIT_SHADOW_COL;		// �F
	m_fRadiusInit = 0.0f;			// �����̔��a
	m_nAreaIdx = 0;					// �敪�̔ԍ�
}

//====================
// �f�X�g���N�^
//====================
CShadowCircle::~CShadowCircle()
{

}

//====================
// ����������
//====================
HRESULT CShadowCircle::Init(void)
{
	// ����������
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �S�Ă̒l������������
	m_col = INIT_SHADOW_COL;		// �F
	m_fRadiusInit = 0.0f;			// �����̔��a
	m_nAreaIdx = 0;					// �敪�̔ԍ�

	// ������Ԃ�
	return S_OK;
}

//====================
// �I������
//====================
void CShadowCircle::Uninit(void)
{
	// �I�u�W�F�N�g�̏I������
	CObject3D::Uninit();
}

//====================
// �X�V����
//====================
void CShadowCircle::Update(void)
{
	// �O��̈ʒu��ݒ肷��
	SetPosOld(GetPos());

	// �����ɂ��ݒ菈��
	Distance();

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);
}

//====================
// �`�揈��
//====================
void CShadowCircle::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	//���Z����
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`�揈��
	CObject3D::Draw();

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//====================
// ���̐ݒ菈��
//====================
void CShadowCircle::SetData(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx)
{
	// ���̏�����
	m_col = INIT_SHADOW_COL;		// �F
	m_fRadiusInit = fRadius;		// �����̔��a

	// ���̐ݒ�
	SetPos(pos);				// �ʒu
	SetPosOld(pos);				// �O��̈ʒu
	SetRot(NONE_D3DXVECTOR3);	// ����
	SetSize(D3DXVECTOR3(fRadius, 0.0f, fRadius));		// �T�C�Y

	// �敪�̔ԍ���ݒ肷��
	m_nAreaIdx = nAreaIdx;

	// �ݒ菈��
	Distance();

	// ���_���̐ݒ菈��
	SetVertex();

	// ���_�J���[�̐ݒ菈��
	SetVtxColor(m_col);

	// �e�N�X�`���̓ǂݍ��ݏ���
	BindTexture(CManager::Get()->GetTexture()->Regist(SHADOW_TEXTURE));
}

//====================
// ��������
//====================
CShadowCircle* CShadowCircle::Create(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx)
{
	// ���[�J���I�u�W�F�N�g�𐶐�
	CShadowCircle* pShadow = nullptr;	// �e�̃C���X�^���X�𐶐�

	if (pShadow == nullptr)
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// �I�u�W�F�N�g�𐶐�
		pShadow = new CShadowCircle;
	}
	else
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // �I�u�W�F�N�g�� NULL ����Ȃ��ꍇ

		// ����������
		if (FAILED(pShadow->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��~
			assert(false);

			// NULL ��Ԃ�
			return nullptr;
		}

		// ���̐ݒ菈��
		pShadow->SetData(pos, fRadius, nAreaIdx);
	}
	else
	{ // �I�u�W�F�N�g�� NULL �̏ꍇ

		// ��~
		assert(false);

		// NULL ��Ԃ�
		return nullptr;
	}

	// �e�̃|�C���^��Ԃ�
	return pShadow;
}

//====================
// �敪�̔ԍ��̐ݒ菈��
//====================
void CShadowCircle::SetAreaIdx(const int nIdx)
{
	// �敪�̔ԍ���ݒ肷��
	m_nAreaIdx = nIdx;
}

//====================
// �����蔻��
//====================
void CShadowCircle::Collision(void)
{
	// �ʒu���擾
	D3DXVECTOR3 pos = GetPos();
	float fHeight = 0.0f;

	// �N���n�ʂƂ̓����蔻��
	ElevationCollision(pos, &fHeight);

	// ��̓����蔻��
	RockCollision(pos, &fHeight);

	// �u���b�N�̓����蔻��
	BlockCollision(pos, &fHeight);

	// ������ݒ肷��
	pos.y = fHeight;

	// �ʒu��K�p
	SetPos(pos);
}

//=======================================
// �N���n�ʂ̓����蔻�菈��
//=======================================
void CShadowCircle::ElevationCollision(const D3DXVECTOR3& pos, float* pHeight)
{
	// ���[�J���ϐ��錾
	float fComp = 0.0f;					// ��r�p�ϐ�
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;		// �擪�̏���
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
			fComp = pElev->ElevationCollision(pos);

			if (nIdx == 0)
			{ // �ŏ��̏ꍇ

				// ������ݒ肷��
				*pHeight = fComp;
			}
			else
			{ // ��L�ȊO

				if (pos.y - *pHeight >= pos.y - fComp &&
					pos.y >= fComp)
				{ // ��茻�݂ɋ߂��ꍇ

					// ������ݒ肷��
					*pHeight = fComp;
				}
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
}

//====================
// ��̓����蔻��
//====================
void CShadowCircle::RockCollision(const D3DXVECTOR3& pos, float* pHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// ��̈ʒu
	float fRadiusRock = 0.0f;					// ��̔��a
	float fTopRock = 0.0f;						// ��̏�̍���

	CListManager<CRock*> list = CRock::GetList(m_nAreaIdx);
	int nNum = list.GetNumData();	// ���̑���
	CRock* pRock = nullptr;			// �擪�̒l

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		for (int nCnt = 0; nCnt < nNum; nCnt++)
		{
			// �����擾
			pRock = list.GetData(nCnt);

			// ��̈ʒu���擾����
			posRock = pRock->GetPos();

			// ��̔��a���擾����
			fRadiusRock = pRock->GetRadius();

			if (useful::CircleCollisionXZ(pos, posRock, 0.0f, fRadiusRock) == true)
			{ // �~���ɓ������ꍇ

				// ��̏�̍������擾����
				fTopRock = pRock->GetTopHeight();

				if (pos.y - *pHeight >= pos.y - (posRock.y + fTopRock) &&
					pos.y >= (posRock.y + fTopRock))
				{ // ��茻�݂ɋ߂��ꍇ

					// �ʒu��ݒ肷��
					*pHeight = posRock.y + fTopRock;
				}
			}
		}
	}
}

//====================
// �u���b�N�̓����蔻��
//====================
void CShadowCircle::BlockCollision(const D3DXVECTOR3& pos, float* pHeight)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posBlock = NONE_D3DXVECTOR3;		// �u���b�N�̈ʒu
	D3DXVECTOR3 vtxMaxBlock = NONE_D3DXVECTOR3;		// ���_�̍ő�l
	D3DXVECTOR3 vtxMinBlock = NONE_D3DXVECTOR3;		// ���_�̍ŏ��l

	CListManager<CBlock*> list = CBlock::GetList(m_nAreaIdx);
	int nNum = list.GetNumData();		// ���̑���
	CBlock* pBlock = nullptr;			// �擪�̒l

	if (list.IsEmpty() == false)
	{ // �󔒂���Ȃ��ꍇ

		for (int nCnt = 0; nCnt < nNum; nCnt++)
		{
			// �����擾
			pBlock = list.GetData(nCnt);

			// �u���b�N�̈ʒu���擾����
			posBlock = pBlock->GetPos();
			vtxMaxBlock = pBlock->GetVtxMax();
			vtxMinBlock = pBlock->GetVtxMin();

			if (useful::RectangleCollisionXZ(pos, posBlock, NONE_D3DXVECTOR3, vtxMaxBlock, NONE_D3DXVECTOR3, vtxMinBlock) == true &&
				pos.y - *pHeight >= pos.y - (posBlock.y + vtxMaxBlock.y) &&
				pos.y >= (posBlock.y + vtxMaxBlock.y))
			{ // �͈͓��̏ꍇ

				// �ʒu��ݒ肷��
				*pHeight = posBlock.y + vtxMaxBlock.y;
			}
		}
	}
}

//====================
// �����x�̐ݒ菈��
//====================
void CShadowCircle::Distance(void)
{

}