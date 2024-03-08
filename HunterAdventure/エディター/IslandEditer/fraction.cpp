//===========================================
//
// 破片のメイン処理[fraction.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "fraction.h"
#include "useful.h"

#include "objectElevation.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
namespace
{
	const char* MODEL[CFraction::TYPE_MAX] =	// モデル
	{
		"data\\MODEL\\Platform\\FracScrew.x",
		"data\\MODEL\\Platform\\FracGear.x",
		"data\\MODEL\\Platform\\FracRing.x",
		"data\\MODEL\\Platform\\FracWood.x",
		"data\\MODEL\\Platform\\FracRock.x",
	};
	const float GRAVITY = 0.4f;			// 重力
	const float BOUND_MAGNI = -0.7f;	// バウンドの倍率
	const int MIN_MOVE_WIDTH = 3;		// XZ軸の最小限の移動量
	const int MIN_MOVE_HEIGHT = 4;		// Y軸の最小限の移動量
	const int RAND_ROT_MOVE = 30;		// 向きのランダムの移動量
	const int MIN_ROT_MOVE = 12;		// 向きの最小限の移動量
	const float SUB_ALPHA = 0.05f;		// 透明度の減算量
}

//==============================
// コンストラクタ
//==============================
CFraction::CFraction() : CModel(CObject::TYPE_FRACTION, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_move = NONE_D3DXVECTOR3;			// 移動量
	m_rotMove = NONE_D3DXVECTOR3;		// 向きの移動量
	m_state = STATE_NONE;				// 状態
	m_nLife = 0;						// 寿命
	m_fAlpha = 0.0f;					// 透明度
}

//==============================
// デストラクタ
//==============================
CFraction::~CFraction()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CFraction::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CFraction::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CFraction::Update(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		// 寿命を減算する
		m_nLife--;

		if (m_nLife <= 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを0にする
			m_nLife = 0;

			// 消去状態にする
			m_state = STATE_DELETE;
		}

		break;

	case STATE_DELETE:		// 消去状態

		// 透明度を減算する
		m_fAlpha -= SUB_ALPHA;

		if (m_fAlpha <= 0.0f)
		{ // 透明度が 0.0f以下 になった場合

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 移動処理
	Move();

	// 向きの移動処理
	RotMove();

	// 起伏地面との当たり判定処理
	Elevation();
}

//=====================================
// 破片の描画処理
//=====================================
void CFraction::Draw(void)
{
	switch (m_state)
	{
	case STATE_NONE:

		// 描画処理
		CModel::Draw();

		break;

	case STATE_DELETE:

		// 描画処理
		CModel::Draw(m_fAlpha);

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CFraction::SetData(const D3DXVECTOR3& pos, const TYPE type, const int nLife, const int nMoveWidth, const int nMoveHeight)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);		// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL[type]));		// モデル情報を設定する

	// 移動量の設定処理
	MoveSet(nMoveWidth, nMoveHeight);

	// 情報の設定処理
	m_state = STATE_NONE;		// 状態
	m_nLife = nLife;			// 寿命
	m_fAlpha = 1.0f;			// 透明度
}

//=======================================
// 生成処理
//=======================================
CFraction* CFraction::Create(const D3DXVECTOR3& pos, const TYPE type, const int nLife, const int nMoveWidth, const int nMoveHeight)
{
	// ローカルオブジェクトを生成
	CFraction* pFrac = nullptr;	// インスタンスを生成

	if (pFrac == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pFrac = new CFraction;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFrac != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFrac->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pFrac->SetData(pos, type, nLife, nMoveWidth, nMoveHeight);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 破片のポインタを返す
	return pFrac;
}

//=======================================
// 移動量の設定処理
//=======================================
void CFraction::MoveSet(const int nMoveWidth, const int nMoveHeight)
{
	// ローカル変数宣言
	float m_fRot;			// 飛んでいく向き

	// 向きをランダムで出す
	m_fRot = (float)((rand() % 629 - 314) * 0.01f);

	// 全ての値を設定する
	m_move.x = sinf(m_fRot) * (rand() % nMoveWidth + MIN_MOVE_WIDTH);
	m_move.y = (float)(rand() % nMoveHeight + MIN_MOVE_HEIGHT);
	m_move.z = cosf(m_fRot) * (rand() % nMoveWidth + MIN_MOVE_WIDTH);

	// 向きの移動量を設定する
	m_rotMove.x = (rand() % RAND_ROT_MOVE - MIN_ROT_MOVE) * 0.01f;		// 向きの移動量
	m_rotMove.y = (rand() % RAND_ROT_MOVE - MIN_ROT_MOVE) * 0.01f;		// 向きの移動量
	m_rotMove.z = (rand() % RAND_ROT_MOVE - MIN_ROT_MOVE) * 0.01f;		// 向きの移動量
}

//=======================================
// 移動処理
//=======================================
void CFraction::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 重力処理
	useful::Gravity(&m_move.y, pos, GRAVITY);

	// 位置を更新する
	pos += m_move;

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 向きの移動処理
//=======================================
void CFraction::RotMove(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 向きを更新する
	rot += m_rotMove;

	// 向きの正規化
	useful::RotNormalize(&rot.x);
	useful::RotNormalize(&rot.y);
	useful::RotNormalize(&rot.z);

	// 向きを適用する
	SetRot(rot);
}

//=======================================
// 起伏地面との当たり判定処理
//=======================================
void CFraction::Elevation(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();				// 位置を取得する
	float fHeight = 0.0f;					// 高さ
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// 先頭の小判
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
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 重力を設定する
				m_move.y *= BOUND_MAGNI;
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

	// 位置を更新する
	SetPos(pos);
}