#ifndef STRINGCONST_H
#define STRINGCONST_H

/// @file StringConst.h
/// @brief StringConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ConstVal.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class StringConst StringConst.h "StringConst.h"
/// @brief 文字列定数を表すノード
//////////////////////////////////////////////////////////////////////
class StringConst :
  public ConstVal
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] val 値
  StringConst(const Type* type,
	      const char* val);

  /// @brief デストラクタ
  virtual
  ~StringConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を返す．
  virtual
  const char*
  string_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const char* mVal;

};

END_NAMESPACE_YM_YMSL

#endif // STRINGCONST_H
