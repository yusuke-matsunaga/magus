#ifndef IRINDEXHANDLE_H
#define IRINDEXHANDLE_H

/// @file IrIndexHandle.h
/// @brief IrIndexHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
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
  /// @param[in] module_index モジュールインデックス
  /// @param[in] local_index ローカルインデックス
  IrIndexHandle(ShString name,
		const Type* type,
		ymuint module_index,
		ymuint local_index);

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

  /// @brief モジュールインデックスを返す．
  ///
  /// kVar, kFunction のみ有効
  virtual
  ymuint
  module_index() const;

  /// @brief ローカルインデックスを返す．
  ///
  /// kVar, kFunction, kMemberRef, kMethodRef のみ有効
  virtual
  ymuint
  local_index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 型
  const Type* mType;

  // モジュールインデックス
  ymuint mModuleIndex;

  // ローカルインデックス
  ymuint mLocalIndex;

};

END_NAMESPACE_YM_YMSL

#endif // IRINDEXHANDLE_H
