#ifndef DIMACSSCANNER_H
#define DIMACSSCANNER_H

/// @file DimacsScanner.h
/// @brief DimacsScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/sat_nsdef.h"
#include "utils/Scanner.h"


BEGIN_NAMESPACE_YM_SAT

/// @brief トークンの型定義
enum tToken {
  kC,
  kP,
  kNUM,
  kZERO,
  kNL,
  kEOF,
  kERR
};

//////////////////////////////////////////////////////////////////////
/// @class DimacsScanner DimacsScanner.h "DimacsScanner.h"
/// @ingroup SatGroup
/// @brief DIMACS形式の CNFファイルを読み込む字句解析器
//////////////////////////////////////////////////////////////////////
class DimacsScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  /// @param[in] ido 入力データ
  DimacsScanner(IDO& ido);

  /// @brief デストラクタ
  ~DimacsScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンの読込み
  /// @param[out] loc 対応するファイル上の位置情報を格納する変数
  tToken
  read_token(FileRegion& loc);

  /// @brief 直前の read_token() が kNUM だった場合の値を返す．
  int
  cur_val();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  /// @return トークンを返す．
  tToken
  scan();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // read_token で読んだトークンが kNUM の場合の値
  int mCurVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前の read_token() が kNUM だった場合の値を返す．
inline
int
DimacsScanner::cur_val()
{
  return mCurVal;
}

END_NAMESPACE_YM_SAT

#endif // DIMACSSCANNER_H
