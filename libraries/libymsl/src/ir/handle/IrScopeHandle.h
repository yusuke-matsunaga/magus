#ifndef IRSCOPEHANDLE_H
#define IRSCOPEHANDLE_H

/// @file IrScopeHandle.h
/// @brief IrScopeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrScopeHandle IrScopeHandle.h "IrScopeHandle.h"
/// @brief Scope を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrScopeHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] scope スコープ
  IrScopeHandle(Scope* scope);

  /// @brief デストラクタ
  virtual
  ~IrScopeHandle();


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

  /// @brief 名前空間を返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  Scope*
  scope() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // スコープ
  Scope* mScope;

};

END_NAMESPACE_YM_YMSL

#endif // IRSCOPEHANDLE_H
