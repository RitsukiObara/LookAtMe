//=======================================
//
// エイムのメイン処理[aim.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "aim.h"
#include "texture.h"
#include "collision.h"
#include "debugproc.h"
#include "useful.h"

#include "enemy.h"
#include "balloon_spawner.h"
#include "balloon.h"

//---------------------------------------
// 無名名前空間
//---------------------------------------
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\Aim.png";		// テクスチャ
	const float RADIUS = 80.0f;		// 半径
	const D3DXCOLOR HIT_COL = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		// 敵に照準が合っている時の色
	const float NONE_LENGTH = 1500.0f;					// 何にも当たっていない長さ
	const float HIT_VECTOR_PERMISSION = 0.05f;			// エイムを合わせるときのベクトルの許容範囲
}

//=========================
// コンストラクタ
//=========================
CAim::CAim() : CBillboard(CObject::TYPE_NONE, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_posPlayer = NONE_D3DXVECTOR3;		// プレイヤーの位置
	m_fLength = NONE_LENGTH;			// 長さ
	m_bColl = false;					// 当たり判定状況
}

//=========================
// デストラクタ
//=========================
CAim::~CAim()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CAim::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CAim::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CAim::Update(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = CManager::Get()->GetCamera()->GetRot();

	// 目的の視点を設定する
	pos.x = m_posPlayer.x + sinf(rot.y) * sinf(rot.x) * m_fLength;
	pos.y = m_posPlayer.y + cosf(rot.x) * m_fLength;
	pos.z = m_posPlayer.z + cosf(rot.y) * sinf(rot.x) * m_fLength;

	// 位置を適用する
	SetPos(pos);

	// 当たり判定処理
	Collision();

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CAim::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff(false);
}

//=========================
// 情報の設定処理
//=========================
void CAim::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(0.0f);			// 向き
	SetSize(D3DXVECTOR3(RADIUS, RADIUS, 0.0f));	// サイズ
	SetAngle();				// 方向の設定処理
	SetLength();			// 長さの設定処理
	SetEnableLookY(true);	// Y軸状況

	// 全ての値を設定する
	m_posPlayer = NONE_D3DXVECTOR3;		// プレイヤーの位置
	m_fLength = NONE_LENGTH;			// 長さ
	m_bColl = false;					// 当たり判定状況

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//=========================
// 生成処理
//=========================
CAim* CAim::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CAim* pAim = nullptr;	// エイムのインスタンスを生成

	if (pAim == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pAim = new CAim;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pAim != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pAim->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pAim->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// エイムのポインタを返す
	return pAim;
}

//=========================
// プレイヤーの位置の設定処理
//=========================
void CAim::SetPosPlayer(const D3DXVECTOR3& pos)
{
	// プレイヤーの位置を設定する
	m_posPlayer = pos;
}

//=========================
// 当たり判定処理
//=========================
void CAim::Collision(void)
{
	if (EnemyCollision() == true ||
		BalloonCollision() == true)
	{ // 何かに当たった場合

		// 色を付ける
		SetVtxColor(HIT_COL);
	}
	else
	{ // 上記以外

		// 長さを初期化する
		m_fLength = NONE_LENGTH;

		// 色を通常に戻す
		SetVtxColor(NONE_D3DXCOLOR);
	}
}

//=========================
// 敵との当たり判定
//=========================
bool CAim::EnemyCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();					// 位置
	D3DXVECTOR3 posEnemy = NONE_D3DXVECTOR3;	// 敵の位置
	float fHeightEnemy = 0.0f;					// 敵の高さ
	D3DXVECTOR3 vecBullet = NONE_D3DXVECTOR3;	// 弾道のベクトル
	D3DXVECTOR3 vecEnemy = NONE_D3DXVECTOR3;	// プレイヤーから敵に向けてのベクトル
	float fLength = 0.0f;						// 長さ
	CListManager<CEnemy*> list = CEnemy::GetList();		// 敵のリスト
	CEnemy* pEnemy = nullptr;		// 先頭の敵
	CEnemy* pEnemyEnd = nullptr;	// 末尾の値
	int nIdx = 0;					// 敵の番号

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pEnemy = list.GetTop();

		// 末尾の値を取得する
		pEnemyEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵の位置を取得する
			posEnemy = pEnemy->GetPos();

			// 敵の高さを取得する
			fHeightEnemy = pEnemy->GetCollSize().y;

			// 位置を中心にする
			posEnemy.y += fHeightEnemy * 0.5f;

			// 弾道のベクトルを算出する
			vecBullet = pos - m_posPlayer;

			// 敵へのベクトルを算出する
			vecEnemy = posEnemy - m_posPlayer;

			// ベクトルを正規化する
			D3DXVec3Normalize(&vecBullet, &vecBullet);
			D3DXVec3Normalize(&vecEnemy, &vecEnemy);

			// 長さを設定する
			fLength = sqrtf((posEnemy.x - m_posPlayer.x) * (posEnemy.x - m_posPlayer.x) + (posEnemy.z - m_posPlayer.z) * (posEnemy.z - m_posPlayer.z));

			if (vecBullet.x + HIT_VECTOR_PERMISSION >= vecEnemy.x &&
				vecBullet.x - HIT_VECTOR_PERMISSION <= vecEnemy.x &&
				vecBullet.y + HIT_VECTOR_PERMISSION >= vecEnemy.y &&
				vecBullet.y - HIT_VECTOR_PERMISSION <= vecEnemy.y &&
				vecBullet.z + HIT_VECTOR_PERMISSION >= vecEnemy.z &&
				vecBullet.z - HIT_VECTOR_PERMISSION <= vecEnemy.z &&
				fLength <= NONE_LENGTH)
			{ // 敵が射線上にいる場合

				// 長さを算出する
				m_fLength = fLength;

				// true を返す
				return true;
			}

			if (pEnemy == pEnemyEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pEnemy = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}

//=========================
// 風船との当たり判定
//=========================
bool CAim::BalloonCollision(void)
{
	// ローカル変数宣言
	CBalloon* pBalloon = nullptr;				// 風船
	D3DXVECTOR3 pos = GetPos();					// 位置
	D3DXVECTOR3 posBalloon = NONE_D3DXVECTOR3;	// 敵の位置
	D3DXVECTOR3 vecBullet = NONE_D3DXVECTOR3;	// 弾道のベクトル
	D3DXVECTOR3 vecBalloon = NONE_D3DXVECTOR3;	// プレイヤーから敵に向けてのベクトル
	float fLength = 0.0f;						// 長さ

	CListManager<CBalloonSpawner*> list = CBalloonSpawner::GetList();		// 敵のリスト
	CBalloonSpawner* pSpawner = nullptr;		// 先頭の敵
	CBalloonSpawner* pSpawnerEnd = nullptr;		// 末尾の値
	int nIdx = 0;					// 敵の番号

	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pSpawner = list.GetTop();

		// 末尾の値を取得する
		pSpawnerEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 風船の情報を取得
			pBalloon = pSpawner->GetBalloon();

			if (pBalloon != nullptr)
			{ // 風船が NULL じゃない場合

				// 敵の位置を取得する
				posBalloon = pBalloon->GetPos();

				// 弾道のベクトルを算出する
				vecBullet = pos - m_posPlayer;

				// 敵へのベクトルを算出する
				vecBalloon = posBalloon - m_posPlayer;

				// ベクトルを正規化する
				D3DXVec3Normalize(&vecBullet, &vecBullet);
				D3DXVec3Normalize(&vecBalloon, &vecBalloon);

				// 長さを設定する
				fLength = sqrtf((posBalloon.x - m_posPlayer.x) * (posBalloon.x - m_posPlayer.x) + (posBalloon.z - m_posPlayer.z) * (posBalloon.z - m_posPlayer.z));

				if (vecBullet.x + HIT_VECTOR_PERMISSION >= vecBalloon.x &&
					vecBullet.x - HIT_VECTOR_PERMISSION <= vecBalloon.x &&
					vecBullet.y + HIT_VECTOR_PERMISSION >= vecBalloon.y &&
					vecBullet.y - HIT_VECTOR_PERMISSION <= vecBalloon.y &&
					vecBullet.z + HIT_VECTOR_PERMISSION >= vecBalloon.z &&
					vecBullet.z - HIT_VECTOR_PERMISSION <= vecBalloon.z &&
					fLength <= NONE_LENGTH)
				{ // 敵が射線上にいる場合

					// 長さを算出する
					m_fLength = fLength;

					// true を返す
					return true;
				}
			}

			if (pSpawner == pSpawnerEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pSpawner = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}