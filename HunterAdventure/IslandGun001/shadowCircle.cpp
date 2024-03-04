//===========================================
//
// 丸影のメイン処理[shadowCircle.cpp]
// Author 小原立暉
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
// 無名名前空間
//-------------------------------------------
namespace
{
	const D3DXCOLOR INIT_SHADOW_COL = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);	// 初期の影の色
	const char* SHADOW_TEXTURE = "data\\TEXTURE\\shadow.jpg";				// 影のテクスチャ
}

//====================
// コンストラクタ
//====================
CShadowCircle::CShadowCircle() : CObject3D(TYPE_SHADOW, PRIORITY_SHADOW)
{
	// 全ての値をクリアする
	m_col = INIT_SHADOW_COL;		// 色
	m_fRadiusInit = 0.0f;			// 初期の半径
	m_nAreaIdx = 0;					// 区分の番号
}

//====================
// デストラクタ
//====================
CShadowCircle::~CShadowCircle()
{

}

//====================
// 初期化処理
//====================
HRESULT CShadowCircle::Init(void)
{
	// 初期化処理
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_col = INIT_SHADOW_COL;		// 色
	m_fRadiusInit = 0.0f;			// 初期の半径
	m_nAreaIdx = 0;					// 区分の番号

	// 成功を返す
	return S_OK;
}

//====================
// 終了処理
//====================
void CShadowCircle::Uninit(void)
{
	// オブジェクトの終了処理
	CObject3D::Uninit();
}

//====================
// 更新処理
//====================
void CShadowCircle::Update(void)
{
	// 前回の位置を設定する
	SetPosOld(GetPos());

	// 距離による設定処理
	Distance();

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);
}

//====================
// 描画処理
//====================
void CShadowCircle::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファ更新の有効 / 無効の設定

	//減算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画処理
	CObject3D::Draw();

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);		// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);			// Zバッファ更新の有効 / 無効の設定
}

//====================
// 情報の設定処理
//====================
void CShadowCircle::SetData(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx)
{
	// 情報の初期化
	m_col = INIT_SHADOW_COL;		// 色
	m_fRadiusInit = fRadius;		// 初期の半径

	// 情報の設定
	SetPos(pos);				// 位置
	SetPosOld(pos);				// 前回の位置
	SetRot(NONE_D3DXVECTOR3);	// 向き
	SetSize(D3DXVECTOR3(fRadius, 0.0f, fRadius));		// サイズ

	// 区分の番号を設定する
	m_nAreaIdx = nAreaIdx;

	// 設定処理
	Distance();

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(SHADOW_TEXTURE));
}

//====================
// 生成処理
//====================
CShadowCircle* CShadowCircle::Create(const D3DXVECTOR3& pos, const float fRadius, const int nAreaIdx)
{
	// ローカルオブジェクトを生成
	CShadowCircle* pShadow = nullptr;	// 影のインスタンスを生成

	if (pShadow == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pShadow = new CShadowCircle;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pShadow != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pShadow->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pShadow->SetData(pos, fRadius, nAreaIdx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 影のポインタを返す
	return pShadow;
}

//====================
// 区分の番号の設定処理
//====================
void CShadowCircle::SetAreaIdx(const int nIdx)
{
	// 区分の番号を設定する
	m_nAreaIdx = nIdx;
}

//====================
// 当たり判定
//====================
void CShadowCircle::Collision(void)
{
	// 位置を取得
	D3DXVECTOR3 pos = GetPos();
	float fHeight = 0.0f;

	// 起伏地面との当たり判定
	ElevationCollision(pos, &fHeight);

	// 岩の当たり判定
	RockCollision(pos, &fHeight);

	// ブロックの当たり判定
	BlockCollision(pos, &fHeight);

	// 高さを設定する
	pos.y = fHeight;

	// 位置を適用
	SetPos(pos);
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
void CShadowCircle::ElevationCollision(const D3DXVECTOR3& pos, float* pHeight)
{
	// ローカル変数宣言
	float fComp = 0.0f;					// 比較用変数
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;		// 先頭の小判
	CElevation* pElevEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 当たり判定を取る
			fComp = pElev->ElevationCollision(pos);

			if (nIdx == 0)
			{ // 最初の場合

				// 高さを設定する
				*pHeight = fComp;
			}
			else
			{ // 上記以外

				if (pos.y - *pHeight >= pos.y - fComp &&
					pos.y >= fComp)
				{ // より現在に近い場合

					// 高さを設定する
					*pHeight = fComp;
				}
			}

			if (pElev == pElevEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pElev = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

//====================
// 岩の当たり判定
//====================
void CShadowCircle::RockCollision(const D3DXVECTOR3& pos, float* pHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posRock = NONE_D3DXVECTOR3;		// 岩の位置
	float fRadiusRock = 0.0f;					// 岩の半径
	float fTopRock = 0.0f;						// 岩の上の高さ

	CListManager<CRock*> list = CRock::GetList(m_nAreaIdx);
	int nNum = list.GetNumData();	// 情報の総数
	CRock* pRock = nullptr;			// 先頭の値

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		for (int nCnt = 0; nCnt < nNum; nCnt++)
		{
			// 情報を取得
			pRock = list.GetData(nCnt);

			// 岩の位置を取得する
			posRock = pRock->GetPos();

			// 岩の半径を取得する
			fRadiusRock = pRock->GetRadius();

			if (useful::CircleCollisionXZ(pos, posRock, 0.0f, fRadiusRock) == true)
			{ // 円周に入った場合

				// 岩の上の高さを取得する
				fTopRock = pRock->GetTopHeight();

				if (pos.y - *pHeight >= pos.y - (posRock.y + fTopRock) &&
					pos.y >= (posRock.y + fTopRock))
				{ // より現在に近い場合

					// 位置を設定する
					*pHeight = posRock.y + fTopRock;
				}
			}
		}
	}
}

//====================
// ブロックの当たり判定
//====================
void CShadowCircle::BlockCollision(const D3DXVECTOR3& pos, float* pHeight)
{
	// ローカル変数宣言
	D3DXVECTOR3 posBlock = NONE_D3DXVECTOR3;		// ブロックの位置
	D3DXVECTOR3 vtxMaxBlock = NONE_D3DXVECTOR3;		// 頂点の最大値
	D3DXVECTOR3 vtxMinBlock = NONE_D3DXVECTOR3;		// 頂点の最小値

	CListManager<CBlock*> list = CBlock::GetList(m_nAreaIdx);
	int nNum = list.GetNumData();		// 情報の総数
	CBlock* pBlock = nullptr;			// 先頭の値

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		for (int nCnt = 0; nCnt < nNum; nCnt++)
		{
			// 情報を取得
			pBlock = list.GetData(nCnt);

			// ブロックの位置を取得する
			posBlock = pBlock->GetPos();
			vtxMaxBlock = pBlock->GetVtxMax();
			vtxMinBlock = pBlock->GetVtxMin();

			if (useful::RectangleCollisionXZ(pos, posBlock, NONE_D3DXVECTOR3, vtxMaxBlock, NONE_D3DXVECTOR3, vtxMinBlock) == true &&
				pos.y - *pHeight >= pos.y - (posBlock.y + vtxMaxBlock.y) &&
				pos.y >= (posBlock.y + vtxMaxBlock.y))
			{ // 範囲内の場合

				// 位置を設定する
				*pHeight = posBlock.y + vtxMaxBlock.y;
			}
		}
	}
}

//====================
// 透明度の設定処理
//====================
void CShadowCircle::Distance(void)
{

}