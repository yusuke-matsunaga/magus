#ifndef IRINDEXHANDLE_H
#define IRINDEXHANDLE_H

/// @file IrIndexHandle.h
/// @brief IrIndexHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrIndexHandle IrIndexHandle.h "IrIndexHandle.h"
/// @brief Function/Variable を保持する IrHandle に共通な基底クラス
//////////////////////////////////////////////////////////////////////
class IrIndexHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] type 型
  IrIndexHandle(ShString name,
		const Type* type);

  /// @brief デストラクタ
  virtual
  ~IrIndexHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 型を得る．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  const Type*
  value_type() const;

  /// @brief インデックスが設定されていたら true を返す．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  virtual
  bool
  has_index() const;

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
  // 下位1ビットは設定されたかを示すフラグ
  ymuint mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // IRINDEXHANDLE_H
