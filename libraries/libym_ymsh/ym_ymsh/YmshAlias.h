#ifndef YM_YMSH_YMSHALIAS_H
#define YM_YMSH_YMSHALIAS_H

/// @file ym_ymsh/YmshAlias.h
/// @brief YmshAlias のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/ymsh_nsdef.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class YmshAlias YmshAlias.h "ym_ymsh/YmshAlias.h"
/// @brief エイリアスを表すクラス
//////////////////////////////////////////////////////////////////////
class YmshAlias
{
public:

  /// @brief コンストラクタ
  /// @param[in] name
  YmshAlias(const char* name);

  /// @brief デストラクタ
  ~YmshAlias();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エイリアス名を返す．
  const char*
  name() const;

  /// @brief 値の配列を返す．
  const vector<string>&
  value() const;

  /// @brief 値の配列を返す．
  vector<string>&
  value();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // エイリアス名
  StrBuff mName;

  // 値(の配列)
  vector<string> mValue;

};

END_NAMESPACE_YM_YMSH

#endif // YM_YMSH_YMSHALIAS_H
