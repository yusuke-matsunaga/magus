#ifndef W0OP_H
#define W0OP_H

/// @file W0Op.h
/// @brief W0Op のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class W0Op W0Op.h "W0Op.h"
/// @brief walsh の0次係数を求める演算を行うクラス
//////////////////////////////////////////////////////////////////////
class W0Op :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  W0Op(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~W0Op();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e 根の枝
  /// @param[in] nvar 変数の数
  /// @return e が表す BDD のWalsh の0次係数を返す．
  mpz_class
  apply(BddEdge e,
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
  count_sub1(BddEdge e);

  /// @brief apply() の下請け関数(ymint32 版)
  ymint32
  count_sub2(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

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

#endif // W0OP_H
