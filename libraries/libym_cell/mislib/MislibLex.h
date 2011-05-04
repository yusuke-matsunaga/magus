#ifndef LIBYM_CELL_MISLIB_MISLIBLEX_H
#define LIBYM_CELL_MISLIB_MISLIBLEX_H

/// @file libym_cell/mislib/MislibLex.h
/// @brief MislibLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibLex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"

#include "ym_utils/StrBuff.h"
#include "ym_utils/FileScanner.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_CELL_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibLex MislibLex.h "MislibLex.h"
/// @brief Mislib 用の LEX クラス
//////////////////////////////////////////////////////////////////////
class MislibLex :
  public FileScanner
{
public:

  /// @brief コンストラクタ
  MislibLex();

  /// @brief デストラクタ
  ~MislibLex();


public:

  /// @brief トークンを一つとってくる．
  /// @param[out] loc 対応するファイル上の位置を格納する変数
  int
  read_token(FileRegion& loc);

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;

  /// @brief cur_string() を double に変換したものを返す．
  double
  cur_num() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token の下請け関数
  /// @return トークンを返す．
  int
  scan();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在読込中の文字列を貯めておくバッファ
  StrBuff mCurString;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
MislibLex::cur_string() const
{
  return mCurString.c_str();
}

// @brief cur_string() を double に変換したものを返す．
inline
double
MislibLex::cur_num() const
{
  return strtod(cur_string(), static_cast<char**>(NULL));
}

END_NAMESPACE_YM_CELL_MISLIB

#endif // LIBYM_CELL_MISLIB_MISLIBLEX_H
