#ifndef GDS_GDSSTRING_H
#define GDS_GDSSTRING_H

/// @file gds/GdsString.h
/// @brief GdsString のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsString GdsString.h "gds/GdsString.h"
/// @brief 文字列を表すクラス
//////////////////////////////////////////////////////////////////////
class GdsString
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  GdsString();

  /// @brief デストラクタ
  ~GdsString();


public:

  /// @brief 文字列を返す．
  const char*
  str() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列
  // 実際には必要な大きさの領域を確保する．
  char mStr[1];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
GdsString::GdsString()
{
}

// @brief デストラクタ
inline
GdsString::~GdsString()
{
}

// @brief 文字列を返す．
inline
const char*
GdsString::str() const
{
  return mStr;
}

END_NAMESPACE_YM_GDS

#endif // GDS_GDSSTRING_H
