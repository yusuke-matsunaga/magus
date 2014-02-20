
/// @file CombiGen.cc
/// @brief CombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/CombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CombiGenIterator
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
CombiGen::CombiGen(ymuint n,
		   ymuint k) :
  GenBase(n, k)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
CombiGen::CombiGen(const CombiGen& src) :
  GenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const CombiGen&
CombiGen::operator=(const CombiGen& src)
{
  copy(src);
  return *this;
}

// デストラクタ
CombiGen::~CombiGen()
{
}

// 次の要素を求める．
void
CombiGen::operator++()
{
  for (ymuint pos = k(); pos -- > 0; ) {
    if ( elem(pos) < n() - k() + pos ) {
      ++ elem(pos);
      for (ymuint pos1 = pos + 1; pos1 < k(); ++ pos1) {
	elem(pos1) = elem(pos1 - 1) + 1;
      }
      break;
    }
    else if ( pos == 0 ) {
      elem(0) = n();
    }
  }
}

END_NAMESPACE_YM
