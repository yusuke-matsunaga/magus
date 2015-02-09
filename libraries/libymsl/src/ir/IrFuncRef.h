#ifndef IRFUNCREF_H
#define IRFUNCREF_H

/// @file IrFuncRef.h
/// @brief IrFuncRef のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncRef IrFuncRef.h "IrFuncRef.h"
/// @brief 関数参照を表すクラス
//////////////////////////////////////////////////////////////////////
class IrFuncRef :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  IrFuncRef(const Function* func);

  /// @brief デストラクタ
  virtual
  ~IrFuncRef();


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

  /// @brief 関数本体を返す．
  ///
  /// kOpFuncRef のみ有効
  virtual
  const Function*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const Function* mFunction;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCREF_H
