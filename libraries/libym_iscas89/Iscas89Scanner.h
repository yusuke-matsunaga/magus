#ifndef LIBYM_ISCAS89_ISCAS89SCANNER_H
#define LIBYM_ISCAS89_ISCAS89SCANNER_H

/// @file libym_iscas89/Iscas89Scanner.h
/// @brief Iscas89Scanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Iscas89Scanner.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_iscas89/iscas89_nsdef.h"
#include "ym_utils/StrBuff.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileScanner.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class Iscas89Scanner Iscas89Scanner.h "Iscas89Scanner.h"
/// @brief iscas89 用の字句解析器
//////////////////////////////////////////////////////////////////////
class Iscas89Scanner :
  public FileScanner
{
public:

  /// @brief コンストラクタ
  Iscas89Scanner();

  /// @brief デストラクタ
  ~Iscas89Scanner();


public:

  /// @brief トークンを一つ読み出す．
  int
  get_token();

  /// @brief 最後の get_token() で読み出した字句の文字列を返す．
  const StrBuff&
  cur_string();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列バッファ
  StrBuff mCurString;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 最後の get_token() で読み出した字句の文字列を返す．
inline
const StrBuff&
Iscas89Scanner::cur_string()
{
  return mCurString;
}

END_NAMESPACE_YM_ISCAS89

#endif // LIBYM_ISCAS89_ISCAS89SCANNER_H
