#ifndef YM_YMSH_YMSHVAR_H
#define YM_YMSH_YMSHVAR_H

/// @file ym_ymsh/YmshVar.h
/// @brief YmshVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/ymsh_nsdef.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class YmshVar YmshVar.h "ym_ymsh/YmshVar.h"
/// @brief 変数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmshVar
{
public:

  /// @brief コンストラクタ
  /// @param[in] name
  YmshVar(const char* name);

  /// @brief デストラクタ
  ~YmshVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数名を返す．
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

  // 変数名
  StrBuff mName;

  // 値(の配列)
  vector<string> mValue;

};

END_NAMESPACE_YM_YMSH

#endif // YM_YMSH_YMSHVAR_H
