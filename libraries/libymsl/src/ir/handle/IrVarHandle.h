#ifndef IRVARHANDLE_H
#define IRVARHANDLE_H

/// @file IrVarHandle.h
/// @brief IrVarHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVarHandle IrVarHandle.h "IrVarHandle.h"
/// @brief Var を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrVarHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  IrVarHandle(const Var* var);

  /// @brief デストラクタ
  virtual
  ~IrVarHandle();


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

  /// @brief 変数を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  const Var*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  const Var* mVar;

};

END_NAMESPACE_YM_YMSL

#endif // IRVARHANDLE_H
