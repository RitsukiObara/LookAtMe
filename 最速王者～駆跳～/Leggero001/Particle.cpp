//=======================================
//
//パーティクルのメイン処理[Particle.cpp]
//Author 小原立暉
//
//=======================================
#include "Particle.h"

//---------------------------------------
// コンストラクタ
//---------------------------------------
CParticle::CParticle()
{

}

//---------------------------------------
// デストラクタ
//---------------------------------------
CParticle::~CParticle()
{

}

//===============================
//パーティクルの初期化処理
//===============================
HRESULT CParticle::Init(void)
{
	// 成功を返す
	return S_OK;
}

//===============================
//パーティクルの終了処理
//===============================
void CParticle::Uninit(void)
{

}

//===============================
//パーティクルの更新処理
//===============================
void CParticle::Update(void)
{
	
}

//===============================
//パーティクルの描画処理
//===============================
void CParticle::Draw(void)
{
	
}

//===============================
// 生成処理
//===============================
void CParticle::Create(const D3DXVECTOR3& pos, const CEffect::TYPE type)
{
	// ローカルポインタ宣言
	CParticle* pParticle = nullptr;		// パーティクル2Dのポインタ

	if (pParticle == nullptr)
	{ // パーティクルが NULL の場合

		// メモリを確保する
		pParticle = new CParticle;
	}

	switch (type)
	{
	case CEffect::TYPE_NONE:		// 無し

		break;

	case CEffect::TYPE_FIRE:		// 炎系

		// 爆発の初期化処理
		pParticle->InitFire(pos);

		break;

	case CEffect::TYPE_ITEMGET:		// アイテム獲得

		// アイテム獲得の初期化処理
		pParticle->InitItemGet(pos);

		break;

	case CEffect::TYPE_CATCH:		// 掴み時

		// 掴み時の埃の初期化処理
		pParticle->InitCatch(pos);

		break;

	case CEffect::TYPE_BIGJUMPCHARGE:		// 超ジャンプのチャージ

		// 超ジャンプのチャージの初期化処理
		pParticle->InitBigJumpCharge(pos);

		break;

	case CEffect::TYPE_LANDDUST:

		// 着地埃の初期化処理
		pParticle->InitLandDust(pos);

		break;

	default:			// 上記以外

		// 警告文
		MessageBox(NULL, "パーティクルの種類で予定外の数値が検知された！", "警告！", MB_ICONWARNING);

		break;
	}

	if (pParticle != nullptr)
	{ // パーティクルが NULL じゃない場合

		// メモリを開放する
		delete pParticle;
		pParticle = nullptr;
	}
}

//===============================
// 爆発パーティクル処理
//===============================
void CParticle::InitFire(const D3DXVECTOR3& pos)
{
	//[引数用変数]
	D3DXVECTOR3 ShiftPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	int nLife = 0;											// 体力
	float fAngle = 0.0f;									// 方向
	float fSpeed = 0.0f;									// 速度
	float fRadius = 0.0f;									// 半径

	for (int nCntPar = 0; nCntPar < 20; nCntPar++)
	{
		// 位置の設定
		ShiftPos.x = pos.x + (float)((rand() % 101 - 50) / 10.0f);
		ShiftPos.y = pos.y + (float)((rand() % 101 - 50) / 10.0f);
		ShiftPos.z = 0.0f;

		//角度を設定する
		fAngle = (float)(rand() % 629 - 314) / 100.0f;

		//移動量を設定する
		fSpeed = (float)(rand() % 300) / 100.0f + 4.0f;

		//移動量を設定する
		move.x = sinf(fAngle) * fSpeed;
		move.y = cosf(fAngle) * fSpeed;

		//半径を設定する
		fRadius = (float)(rand() % 1000) / 100.0f + 30.0f;

		// 寿命の設定
		nLife = rand() % 30 + 7;

		// エフェクトの設定処理
		CEffect::Create
		(
			ShiftPos,			// 位置
			move,				// 移動量
			nLife,				// 寿命
			fRadius,			// 半径
			CEffect::TYPE_FIRE	// 種類
		);
	}
}

//===============================
// アイテム獲得処理
//===============================
void CParticle::InitItemGet(const D3DXVECTOR3& pos)
{
	//[引数用変数]
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	int nLife = 0;											// 体力
	float fAngle = 0.0f;									// 方向
	float fSpeed = 0.0f;									// 速度
	float fRadius = 0.0f;									// 半径

	for (int nCntPar = 0; nCntPar < 20; nCntPar++)
	{
		//角度を設定する
		fAngle = (float)(rand() % 629 - 314) / 100.0f;

		//移動量を設定する
		fSpeed = (float)(rand() % 500) / 100.0f + 8.0f;

		//移動量を設定する
		move.x = sinf(fAngle) * fSpeed;
		move.y = cosf(fAngle) * fSpeed;

		//半径を設定する
		fRadius = (float)(rand() % 800) / 100.0f + 20.0f;

		// 寿命の設定
		nLife = rand() % 25 + 8;

		// エフェクトの設定処理
		CEffect::Create
		(
			pos,					// 位置
			move,					// 移動量
			nLife,					// 寿命
			fRadius,				// 半径
			CEffect::TYPE_ITEMGET	// 種類
		);
	}
}

//===============================
// 掴み時(埃舞い)の生成
//===============================
void CParticle::InitCatch(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 EffectPos, move;
	int nLife;

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		// エフェクトの位置を設定する
		EffectPos.x = pos.x;
		EffectPos.y = pos.y + rand() % 15 + 7;
		EffectPos.z = pos.z + rand() % 41 - 20;

		// 移動量を設定する
		move.x = (float)(rand() % 7 - 3);
		move.y = (float)(rand() % 7 + 1);
		move.z = (float)(rand() % 7 - 3);

		// 寿命を設定する
		nLife = rand() % 40;

		// エフェクトの設定処理
		CEffect::Create(EffectPos, move, nLife, 70.0f, CEffect::TYPE_CATCH);
	}
}

//===============================
// 超ジャンプのチャージの生成
//===============================
void CParticle::InitBigJumpCharge(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 EffectPos, move;		// 位置、移動量
	int nLife;							// 寿命
	float fRadius;						// 半径

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		// 寿命を設定する
		nLife = rand() % 40;

		// 移動量を設定する
		move.x = (float)(rand() % 11 - 5);
		move.y = (float)(rand() % 11 - 5);
		move.z = (float)(rand() % 11 - 5);

		// エフェクトの位置を設定する
		EffectPos.x = -move.x * nLife;
		EffectPos.y = -move.y * nLife;
		EffectPos.z = -move.z * nLife;

		// 半径を設定する
		fRadius = (float)(rand() % 200 * 0.1f) + 10.0f;

		// エフェクトの設定処理
		CEffect::Create(pos + EffectPos, move, nLife, fRadius, CEffect::TYPE_BIGJUMPCHARGE);
	}
}

//===============================
// 着地煙の生成
//===============================
void CParticle::InitLandDust(const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 EffectPos;		// エフェクトの位置
	D3DXVECTOR3 EffectMove;		// エフェクトの移動量
	int nLife;					// 寿命
	D3DXVECTOR3 randRot;		// ランダムの向き

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		// 向きを設定する
		randRot.x = (float)((rand() % 629 - 314) * 0.01f);
		EffectPos.x = pos.x + sinf(randRot.x) * 20.0f;

		EffectPos.y = pos.y;

		// 向きを設定する
		randRot.z = (float)((rand() % 629 - 314) * 0.01f);
		EffectPos.z = pos.z + cosf(randRot.z) * 20.0f;

		for (int nCntDust = 0; nCntDust < 4; nCntDust++)
		{
			EffectMove.x = sinf(randRot.x) * (rand() % 3 + 10);
			EffectMove.y = (float)(rand() % 3 + 1);
			EffectMove.z = cosf(randRot.z) * (rand() % 3 + 10);

			// 寿命を設定する
			nLife = rand() % 20 + 10;

			// エフェクトの生成処理
			CEffect::Create(EffectPos, EffectMove, nLife, 50.0f, CEffect::TYPE_LANDDUST);
		}
	}
}