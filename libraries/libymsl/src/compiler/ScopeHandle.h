#ifndef SCOPEHANDLE_H
#define SCOPEHANDLE_H

/// @file ScopeHandle.h
/// @brief ScopeHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class ScopeHandle ScopeHandle.h "ScopeHandle.h"
/// @brief Scope を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class ScopeHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] scope スコープ
  ScopeHandle(Scope* scope);

  /// @brief デストラクタ
  virtual
  ~ScopeHandle();


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

#endif // SCOPEHANDLE_H
