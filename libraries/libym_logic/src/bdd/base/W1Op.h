#ifndef W1OP_H
#define W1OP_H

/// @file W1Op.h
/// @brief W1Op のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class W1Op W1Op.h "W1Op.h"
/// @brief walsh の1次係数を求める演算を行うクラス
//////////////////////////////////////////////////////////////////////
class W1Op :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  W1Op(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~W1Op();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e 根の枝
  /// @param[in] level 対象の変数のレベル
  /// @param[in] nvar 変数の数
  /// @return e が表す BDD の varid に関する Walsh の1次係数を返す．
  mpz_class
  apply(BddEdge e,
	ymuint level,
	ymuint nvar);

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep();


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief apply() の下請け関数(mpz_class 版)
  mpz_class
  w1_sub1(BddEdge e);

  /// @brief apply() の下請け関数(ymint32 版)
  ymint32
  w1_sub2(BddEdge e);

  /// @brief w1_sub() の下請け関数(mpz_class 版)
  mpz_class
  w0_sub1(BddEdge e);

  /// @brief w1_sub() の下請け関数(ymint32 版)
  ymint32
  w0_sub2(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象の変数のレベル
  ymuint32 mLevel;

  // 全最小項数(mpz_class版)
  mpz_class mAllCount1;

  // 演算結果を覚えておくハッシュ表(mpz_class版)
  BddEdgeMpzMap mCompTbl1;

  // 全最小項数(ymint32版)
  ymint32 mAllCount2;

  // 演算結果を覚えておくハッシュ表(ymint32版)
  BddEdgeIntMap mCompTbl2;

};

END_NAMESPACE_YM_BDD

#endif // W1OP_H
