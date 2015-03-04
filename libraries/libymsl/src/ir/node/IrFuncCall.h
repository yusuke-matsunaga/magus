#ifndef IRFUNCCALL_H
#define IRFUNCCALL_H

/// @file IrFuncCall.h
/// @brief IrFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncCall IrFuncCall.h "IrFuncCall.h"
/// @brief 関数呼び出しを表すクラス
//////////////////////////////////////////////////////////////////////
class IrFuncCall :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] arglist 引数リスト
  IrFuncCall(const vector<IrNode*>& arglist);

  /// @brief デストラクタ
  virtual
  ~IrFuncCall();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief 関数のアドレスを設定する．
  /// @param[in] func_handle 関数を表すハンドル
  ///
  /// kFuncCall のみ有効
  virtual
  void
  set_function_address(IrHandle* func_handle);

  /// @brief 関数のアドレスを返す．
  ///
  /// kFuncCall のみ有効
  virtual
  IrHandle*
  function_address() const;

  /// @brief 関数の引数の数を得る．
  ///
  /// kOpFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 関数の引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kOpFuncCall のみ有効
  virtual
  IrNode*
  arglist_elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数のハンドル
  IrHandle* mFuncHandle;

  // 引数の数
  ymuint mArgNum;

  // 引数のリスト
  IrNode** mArgList;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCCALL_H
