#ifndef BDDOP_H
#define BDDOP_H

/// @file BddOp.h
/// @brief BddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddOp BddOp.h "BddOp.h"
/// @brief BDD の演算を表すクラス
//////////////////////////////////////////////////////////////////////
class BddOp
{
public:

  /// @brief デストラクタ
  virtual
  ~BddOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の GC で回収されるノードに関連した情報を削除する．
  virtual
  void
  sweep() = 0;

};

END_NAMESPACE_YM_BDD

#endif // BDDOP_H
