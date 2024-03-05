//===================================
//
// エディットヘッダー[edit.h]
// Author 小原立暉
//
//===================================
#ifndef _EDIT_H_
#define _EDIT_H_

//***********************************
// インクルードファイル
//***********************************
#include "object.h"
#include "enemy.h"
#include "tree.h"
#include "rock.h"
#include "wall.h"

//-----------------------------------
// 前方宣言
//-----------------------------------
class CModel;			// モデル

// クラス定義(処理)
class CProcess
{
public:

	CProcess();
	virtual ~CProcess();

	virtual void Process(CModel* m_pModel) = 0;		// 処理
	virtual void Delete(CModel* m_pModel) = 0;		// 消去
};

//-----------------------------------
// クラス定義(エディット)
//-----------------------------------
class CEdit : public CObject
{
public:			// 誰でもアクセスできる

	// 列挙型定義(スタイル)
	enum STYLE
	{
		STYLE_ENEMY = 0,		// 敵
		STYLE_COIN,				// コイン
		STYLE_GOLDBONE,			// 金の骨
		STYLE_TREE,				// 木
		STYLE_ROCK,				// 岩
		STYLE_BLOCK,			// ブロック
		STYLE_BANGFLOWER,		// 爆弾花
		STYLE_WALL,				// 壁
		STYLE_GRASS,			// 草
		STYLE_LAKE,				// 湖
		STYLE_MAX				// この列挙型の総数
	};

	CEdit();					// コンストラクタ
	~CEdit();					// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void SetData(void);				// 情報の設定処理

	// 静的メンバ関数
	static CEdit* Create(void);		// 生成処理

private:		// 自分だけアクセスできる
	
	// メンバ変数
	CModel* m_pModel;		// モデルの情報
	CProcess* m_pProcess;	// 処理の情報
	STYLE m_style;			// スタイル
};

// クラス定義(敵の処理)
class CEnemyProcess : public CProcess
{
public:
	CEnemyProcess();
	~CEnemyProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	CEnemy::TYPE m_type;		// 敵の種類
};

// クラス定義(コインの処理)
class CCoinProcess : public CProcess
{
public:
	CCoinProcess();
	~CCoinProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// クラス定義(金の骨の処理)
class CGoldBoneProcess : public CProcess
{
public:
	CGoldBoneProcess();
	~CGoldBoneProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// クラス定義(木の処理)
class CTreeProcess : public CProcess
{
public:
	CTreeProcess();
	~CTreeProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	CTree::TYPE m_type;		// 木の種類
};

// クラス定義(岩の処理)
class CRockProcess : public CProcess
{
public:
	CRockProcess();
	~CRockProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	CRock::TYPE m_type;		// 木の種類
};

// クラス定義(ブロックの処理)
class CBlockProcess : public CProcess
{
public:
	CBlockProcess();
	~CBlockProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// クラス定義(爆弾花の処理)
class CBangFlowerProcess : public CProcess
{
public:
	CBangFlowerProcess();
	~CBangFlowerProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

};

// クラス定義(壁の処理)
class CWallProcess : public CProcess
{
public:
	CWallProcess();
	~CWallProcess();

	void Process(CModel* m_pModel) override;
	void Delete(CModel* m_pModel) override;

private:

	// メンバ変数
	CWall::TYPE m_type;			// 種類
	CWall::ROTTYPE m_rottype;	// 向きの種類
};

#endif