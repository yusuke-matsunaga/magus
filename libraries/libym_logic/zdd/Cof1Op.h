#ifndef COF1OP_H
#define COF1OP_H

/// @file Cof1Op.h
/// @brief Cof1Op のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddOp.h"
#include "ZddMgrImpl.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class Cof1Op Cof1Op.h "Cof1Op.h"
/// @brief cofactor1 演算を求めるクラス
//////////////////////////////////////////////////////////////////////
class Cof1Op :
  public ZddUnOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr ZddMgrImpl
  Cof1Op(ZddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~Cof1Op();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cofactor1 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] var 変数番号
  virtual
  ZddEdge
  apply(ZddEdge left,
	VarId var);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  ZddEdge
  cof1_step(ZddEdge f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のマネージャ
  ZddMgrImpl& mMgr;

  // 変数
  ZddVar* mVar;

  // 変数のレベル
  ymuint32 mLevel;

  // 一時的に結果を覚えておくハッシュ表
  hash_map<ZddEdge, ZddEdge> mCompTbl;

};

END_NAMESPACE_YM_ZDD

#endif // COF0OP_H
