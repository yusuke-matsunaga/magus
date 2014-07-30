#ifndef MISLIBSCANNER_H
#define MISLIBSCANNER_H

/// @file MislexScanner.h
/// @brief MislexScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"

#include "YmUtils/StrBuff.h"
#include "YmUtils/Scanner.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibScanner MislibScanner.h "MislibScanner.h"
/// @brief Mislib 用の LEX クラス
//////////////////////////////////////////////////////////////////////
class MislibScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] ido 入力データ
  MislibScanner(IDO& ido);

  /// @brief デストラクタ
  ~MislibScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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
MislibScanner::cur_string() const
{
  return mCurString.c_str();
}

// @brief cur_string() を double に変換したものを返す．
inline
double
MislibScanner::cur_num() const
{
  return strtod(cur_string(), static_cast<char**>(NULL));
}

END_NAMESPACE_YM_MISLIB

#endif // MISLIBSCANNER_H
