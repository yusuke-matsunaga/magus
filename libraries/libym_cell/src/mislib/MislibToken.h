#ifndef MISLIBTOKEN_H
#define MISLIBTOKEN_H

/// @file MislibToken.h
/// @brief MislibToken のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @brief mislib 形式のトークン定義
//////////////////////////////////////////////////////////////////////
enum MislibToken {
  STR,
  NUM,
  LP,
  RP,
  SEMI,
  EQ,
  GATE,
  PIN,
  NOINV,
  INV,
  UNKNOWN,
  CONST0
  CONST1,
  PLUS,
  HAT,
  STAR,
  NOT,
  EOF,
  ERROR
};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBTOKEN_H
