#ifndef ENUMCONST_H
#define ENUMCONST_H

/// @file EnumConst.h
/// @brief EnumConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class EnumConst EnumConst.h "EnumConst.h"
/// @brief enum 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class EnumConst
{
  friend class EnumType;

public:

  /// @brief コンストラクタ
  EnumConst();

  /// @brief デストラクタ
  ~EnumConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の enum 型を返す．
  const Type*
  parent_type() const;

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 値を返す．
  int
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の enum 型
  const Type* mParentType;

  // 名前
  ShString mName;

  // 値
  int mVal;

};

END_NAMESPACE_YM_YMSL

#endif // ENUMCONST_H
