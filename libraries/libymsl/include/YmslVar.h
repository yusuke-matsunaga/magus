#ifndef YMSLVAR_H
#define YMSLVAR_H

/// @file YmslVar.h
/// @brief YmslVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslVar YmslVar.h "YmslVar.h"
/// @brief YMSL の変数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslVar
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] index インデックス
  YmslVar(ShString name,
	  const YmslType* value_type,
	  ymuint index);

  /// @brief デストラクタ
  ~YmslVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数名を得る．
  ShString
  name() const;

  /// @brief 型を得る．
  const YmslType*
  value_type() const;

  /// @brief 変数インデックスを得る．
  ymuint
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 型
  const YmslType* mValueType;

  // インデックス
  ymuint mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLVAR_H
