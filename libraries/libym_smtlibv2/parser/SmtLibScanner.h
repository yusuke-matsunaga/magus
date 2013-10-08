#ifndef LIBYM_SMTLIBV2_PARSER_SMTLIBSCANNER_H
#define LIBYM_SMTLIBV2_PARSER_SMTLIBSCANNER_H

/// @file libym_smtlibv2/parser/SmtLibLex.h
/// @brief SmtLibLex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SmtLibLex.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_utils/Scanner.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/StrBuff.h"

#include "TokenType.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLibScaner SmtLibScanner.h "SmtLibScanner.h"
/// @brief SmtLib 用の字句解析器クラス
//////////////////////////////////////////////////////////////////////
class SmtLibScanner :
  public Scanner
{
public:

  /// @brief コンストラクタ
  SmtLibScanner();

  /// @brief デストラクタ
  ~SmtLibScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief トークンを一つとってくる．
  /// @param[out] loc トークンのファイル位置
  tTokenType
  read_token(FileRegion& loc);

  /// @brief 直前の read_token() に対応する文字列を返す．
  const char*
  cur_string() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read_token() の下請け関数
  tTokenType
  scan();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // read_token の結果の文字列を格納する
  StrBuff mCurString;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 直前の read_token() に対応する文字列を返す．
inline
const char*
SmtLibScanner::cur_string() const
{
  return mCurString.c_str();
}

END_NAMESPACE_YM_SMTLIBV2

#endif // LIBYM_SMTLIBV2_PARSER_SMTLIBSCANNER_H
