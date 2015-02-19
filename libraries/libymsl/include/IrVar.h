#ifndef IRVAR_H
#define IRVAR_H

/// @file IrVar.h
/// @brief IrVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVar IrVar.h "IrVar.h"
/// @brief 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class IrVar
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] global グローバル変数の時 true にするフラグ
  IrVar(ShString name,
	const Type* value_type,
	bool global);

  /// @brief デストラクタ
  ~IrVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数名を得る．
  ShString
  name() const;

  /// @brief 型を得る．
  const Type*
  value_type() const;

  /// @brief グローバル変数の時に true を返す．
  bool
  is_global() const;

  /// @brief インデックスを返す．
  ymuint
  index() const;

  /// @brief インデックスを設定する．
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

#endif // IRVAR_H
