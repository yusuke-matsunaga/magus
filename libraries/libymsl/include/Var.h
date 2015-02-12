#ifndef VAR_H
#define VAR_H

/// @file Var.h
/// @brief Var のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class Var Var.h "Var.h"
/// @brief 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class Var
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] global グローバル変数の時 true にするフラグ
  Var(ShString name,
      const Type* value_type,
      bool global);

  /// @brief デストラクタ
  ~Var();


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

#endif // VAR_H
