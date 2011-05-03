#ifndef LIBYM_BLIF_BLIFSCANNER_H
#define LIBYM_BLIF_BLIFSCANNER_H

/// @file libym_blif/BlifScanner.h
/// @brief BlibScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifScanner.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_blif/blif_nsdef.h"
#include "ym_utils/StrBuff.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/FileScanner.h"
#include "BlifDic.h"


BEGIN_NAMESPACE_YM_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifScanner BlifScanner.h "BlifScanner.h"
/// @brief blif 用の字句解析器
//////////////////////////////////////////////////////////////////////
class BlifScanner :
  public FileScanner
{
public:

  /// @brief コンストラクタ
  BlifScanner();

  /// @brief デストラクタ
  ~BlifScanner();


public:

  /// @brief トークンを一つ読み出す．
  tToken
  get_token();

  /// @brief 最後の get_token() で読み出した字句の文字列を返す．
  const StrBuff&
  cur_string();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 予約語テーブル
  BlifDic mDic;

  // 文字列バッファ
  StrBuff mCurString;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 最後の get_token() で読み出した字句の文字列を返す．
inline
const StrBuff&
BlifScanner::cur_string()
{
  return mCurString;
}

END_NAMESPACE_YM_BLIF

#endif // LIBYM_BLIF_BLIFSCANNER_H
