#ifndef COF0OP_H
#define COF0OP_H

/// @file Cof0Op.h
/// @brief Cof0Op のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "UniVOp.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class Cof0Op Cof0Op.h "Cof0Op.h"
/// @brief cofactor0 演算を求めるクラス
//////////////////////////////////////////////////////////////////////
class Cof0Op :
  public UniVOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr CNFddMgrImpl
  Cof0Op(CNFddMgrImpl& mgr);

  /// @brief デストラクタ
  virtual
  ~Cof0Op();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cofactor0 演算を行う関数
  /// @param[in] left オペランド
  virtual
  CNFddEdge
  apply(CNFddEdge left);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 実際に演算を行う関数
  CNFddEdge
  apply_step(CNFddEdge f);

};

END_NAMESPACE_YM_CNFDD

#endif // COF0OP_H
