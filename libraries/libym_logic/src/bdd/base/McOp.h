#ifndef MCOP_H
#define MCOP_H

/// @file McOp.h
/// @brief McOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class McOp McOp.h "McOp.h"
/// @brief 節点数を数える演算を行うクラス
//////////////////////////////////////////////////////////////////////
class McOp :
  public BddOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr マネージャ
  McOp(BddMgrImpl* mgr);

  /// @brief デストラクタ
  virtual
  ~McOp();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] e 根の枝
  /// @param[in] nvar 変数の数
  /// @return e が表す BDD の最小項の数を返す．
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

  /// @brief apply() の下請け関数(ymuint32 版)
  ymuint32
  count_sub2(BddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全最小項数(mpz_class版)
  mpz_class mAllCount1;

  // 演算結果を覚えておくハッシュ表(mpz_class版)
  BddEdgeMpzMap mCompTbl1;

  // 全最小項数(ymuint32版)
  ymuint32 mAllCount2;

  // 演算結果を覚えておくハッシュ表(ymuint32版)
  BddEdgeUint32Map mCompTbl2;

};

END_NAMESPACE_YM_BDD

#endif // MCOP_H
