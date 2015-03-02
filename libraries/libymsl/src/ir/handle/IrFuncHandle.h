#ifndef IRFUNCHANDLE_H
#define IRFUNCHANDLE_H

/// @file IrFuncHandle.h
/// @brief IrFuncHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncHandle IrFuncHandle.h "IrFuncHandle.h"
/// @brief Function を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrFuncHandle :
  public IrIndexHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 関数の型
  /// @param[in] func 関数本体
  IrFuncHandle(ShString name,
	       const Type* type,
	       const VsmFunction* func);

  /// @brief デストラクタ
  virtual
  ~IrFuncHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief 関数本体を返す．
  ///
  /// kFunction, kMethodRef のみ有効
  virtual
  const VsmFunction*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数本体
  const VsmFunction* mFunction;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCHANDLE_H
