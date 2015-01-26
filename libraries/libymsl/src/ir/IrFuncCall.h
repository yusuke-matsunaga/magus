#ifndef YMSLFUNCCALL_H
#define YMSLFUNCCALL_H

/// @file IrFuncCall.h
/// @brief IrFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncCall IrFuncCall.h "IrFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class IrFuncCall :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  /// @param[in] arglist 引数リスト
  IrFuncCall(IrExpr* func,
	       const vector<IrExpr*>& arglist);

  /// @brief デストラクタ
  virtual
  ~IrFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 関数本体を返す．
  ///
  /// kFuncCall のみ有効
  virtual
  IrExpr*
  func_expr() const;

  /// @brief 関数の引数の数を得る．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 関数の引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  IrExpr*
  arglist_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  IrExpr* mFunc;

  // 引数の数
  ymuint mArgNum;

  // 引数のリスト
  IrExpr** mArgList;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFUNCCALL_H
