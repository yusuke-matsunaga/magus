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
  NONINV,
  INV,
  UNKNOWN,
  CONST0,
  CONST1,
  PLUS,
  HAT,
  STAR,
  NOT,
  END,
  ERROR
};

/// @brief MislibToken のストリーム出力
inline
ostream&
operator<<(ostream& s,
	   MislibToken tok)
{
  switch ( tok ) {
  case STR:     s << "STR"; break;
  case NUM:     s << "NUM"; break;
  case LP:      s << "LP"; break;
  case RP:      s << "RP"; break;
  case SEMI:    s << "SEMI"; break;
  case EQ:      s << "EQ"; break;
  case GATE:    s << "GATE"; break;
  case PIN:     s << "PIN"; break;
  case NONINV:  s << "NONINV"; break;
  case INV:     s << "INV"; break;
  case UNKNOWN: s << "UNKNOWN"; break;
  case CONST0:  s << "CONST0"; break;
  case CONST1:  s << "CONST1"; break;
  case PLUS:    s << "PLUS"; break;
  case HAT:     s << "HAT"; break;
  case STAR:    s << "STAR"; break;
  case NOT:     s << "NOT"; break;
  case END:     s << "END"; break;
  case ERROR:   s << "ERROR"; break;
  default: ASSERT_NOT_REACHED;
  }
  return s;
}

END_NAMESPACE_YM_MISLIB

#endif // MISLIBTOKEN_H
