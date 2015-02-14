#ifndef IRFUNCHANDLE_H
#define IRFUNCHANDLE_H

/// @file IrFuncHandle.h
/// @brief IrFuncHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncHandle IrFuncHandle.h "IrFuncHandle.h"
/// @brief Function を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrFuncHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  IrFuncHandle(const Function* func);

  /// @brief デストラクタ
  virtual
  ~IrFuncHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

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

  /// @brief 関数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Function*
  function() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  const Function* mFunc;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCHANDLE_H
