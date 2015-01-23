#ifndef YMSLENUMCONST_H
#define YMSLENUMCONST_H

/// @file YmslEnumConst.h
/// @brief YmslEnumConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslEnumConst YmslEnumConst.h "YmslEnumConst.h"
/// @brief enum 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslEnumConst
{
  friend class YmslEnumType;

public:

  /// @brief コンストラクタ
  YmslEnumConst();

  /// @brief デストラクタ
  ~YmslEnumConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 親の enum 型を返す．
  YmslType*
  parent() const;

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
  YmslType* mParent;

  // 名前
  ShString mName;

  // 値
  int mVal;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLENUMCONST_H
