#ifndef IRFUNCHANDLE_H
#define IRFUNCHANDLE_H

/// @file IrFuncHandle.h
/// @brief IrFuncHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"
#include "YmUtils/ShString.h"


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
  /// @param[in] name 関数名
  /// @param[in] type 関数の型
  IrFuncHandle(ShString name,
	       const Type* type);

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

  /// @brief 型を得る．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  const Type*
  value_type() const;

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
  const Type* mType;

  // インデックス
  ymuint mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCHANDLE_H
