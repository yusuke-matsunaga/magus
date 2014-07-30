
/// @file Literal.cc
/// @brief Literal の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Literal.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM

// 実はデフォルトコンストラクタが未定義リテラルを作っている．
const Literal kLiteralX;

// 場所がないのでここに書いておく．
const VarId kVarIdIllegal;

// ostream に対する書出し
ostream&
operator<<(ostream& s,
	   const Literal& lit)
{
  if ( lit == kLiteralX ) {
    s << "-X-";
  }
  else {
    s << "v_" << lit.varid();
    if ( lit.is_negative() ) {
      s << "'";
    }
  }
  return s;
}

END_NAMESPACE_YM
