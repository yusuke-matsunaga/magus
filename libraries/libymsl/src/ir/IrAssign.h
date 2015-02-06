#ifndef IRASSIGN_H
#define IRASSIGN_H

/// @file IrAssign.h
/// @brief IrAssign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrAssign IrAssign.h "IrAssign.h"
/// @brief 代入文を表すクラス
//////////////////////////////////////////////////////////////////////
class IrAssign :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  IrAssign(IrNode* lhs,
	   IrNode* rhs);

  /// @brief デストラクタ
  virtual
  ~IrAssign();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 左辺式を返す．
  ///
  /// kOpAssign のみ有効
  virtual
  IrNode*
  lhs_expr() const;

  /// @brief 右辺式を返す．
  ///
  /// kOpAssign のみ有効
  virtual
  IrNode*
  rhs_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 左辺式
  IrNode* mLhsExpr;

  // 右辺式
  IrNode* mRhsExpr;

};

END_NAMESPACE_YM_YMSL

#endif // IRASSIGN_H
