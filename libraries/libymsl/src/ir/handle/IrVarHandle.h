#ifndef IRVARHANDLE_H
#define IRVARHANDLE_H

/// @file IrVarHandle.h
/// @brief IrVarHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVarHandle IrVarHandle.h "IrVarHandle.h"
/// @brief Var を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrVarHandle :
  public IrIndexHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] global グローバル変数の時 true とするフラグ
  /// @param[in] var_addr 変数のアドレス
  IrVarHandle(ShString name,
	      const Type* value_type,
	      bool global,
	      VsmValue* var_addr);

  /// @brief デストラクタ
  virtual
  ~IrVarHandle();


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

  /// @brief グローバル変数の時に true を返す．
  ///
  /// kVar のみ有効
  bool
  is_global() const;

  /// @brief 変数本体を返す．
  ///
  /// kVar かつ is_global() == true の時のみ有効
  virtual
  VsmValue*
  variable() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバルフラグ
  bool mGlobal;

  // 変数のアドレス
  VsmValue* mVarAddr;

};

END_NAMESPACE_YM_YMSL

#endif // IRVARHANDLE_H
