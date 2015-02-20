#ifndef IRVARHANDLE_H
#define IRVARHANDLE_H

/// @file IrVarHandle.h
/// @brief IrVarHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"
#include "YmUtils/ShString.h"


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
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] global グローバル変数の時 true とするフラグ
  IrVarHandle(ShString name,
	      const Type* value_type,
	      bool global);

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

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief 型を得る．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  const Type*
  value_type() const;

  /// @brief グローバル変数の時に true を返す．
  ///
  /// kVar のみ有効
  bool
  is_global() const;

  /// @brief インデックスを返す．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  ymuint
  index() const;

  /// @brief インデックスを設定する．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  void
  set_index(ymuint index);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 型
  const Type* mValueType;

  // グローバルフラグ
  bool mGlobal;

  // インデックス
  ymuint mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // IRVARHANDLE_H
