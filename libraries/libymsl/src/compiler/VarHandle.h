#ifndef VARHANDLE_H
#define VARHANDLE_H

/// @file VarHandle.h
/// @brief VarHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class VarHandle VarHandle.h "VarHandle.h"
/// @brief Var を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class VarHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  VarHandle(const Var* var);

  /// @brief デストラクタ
  virtual
  ~VarHandle();


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

#endif // VARHANDLE_H
