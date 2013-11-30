#ifndef AIGPAT_H
#define AIGPAT_H

/// @file AigPat.h
/// @brief AigPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/Aig.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class AigPat AigPat.h "AigPat.h"
/// @brief AIG とそれに付随する情報を表すクラス
//////////////////////////////////////////////////////////////////////
struct AigPat
{
  // 空のコンストラクタ
  AigPat() { }

  // コンストラクタ
  AigPat(Aig aig,
	 ymuint32 func,
	 ymuint level) :
    mAig(aig),
    mFunc(func),
    mLevel(level)
  {
  }

  // AIG
  Aig mAig;

  // 関数ベクタ
  ymuint32 mFunc;

  // レベル
  ymuint32 mLevel;

};

END_NAMESPACE_YM

#endif // AIGPAT_H
