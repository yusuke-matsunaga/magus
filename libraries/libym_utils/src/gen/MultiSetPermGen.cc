
/// @file MultiSetPermGen.cc
/// @brief MultiSetPermGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MultiSetPermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiSetPermGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
MultiSetPermGen::MultiSetPermGen(const vector<ymuint>& num_array,
				 ymuint k) :
  MultiSetGenBase(num_array, k)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MultiSetPermGen::MultiSetPermGen(const MultiSetPermGen& src) :
  MultiSetGenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const MultiSetPermGen&
MultiSetPermGen::operator=(const MultiSetPermGen& src)
{
  copy(src);
  return *this;
}

// @brief デストラクタ
MultiSetPermGen::~MultiSetPermGen()
{
}

// @brief 次の要素を求める．
void
MultiSetPermGen::operator++()
{
  ymuint ng = group_num();
  vector<ymuint> count(ng, 0);
  for (ymuint i = 0; i < k(); ++ i) {
    ymuint v = elem(i);
    ++ count[v];
  }

  // 唯一，後置型の演算子の意味のある使い方
  // 符号なし数なので pos >= 0 は常に成り立ってしまう．
  // あえて pos <= k という使い方もできないことはないが，，，，
  for (ymuint pos = k(); pos -- > 0; ) {
    bool found = false;
    for (ymuint v = elem(pos); ++ v < ng; ++ v) {
      if ( count[v] < n(v) ) {
	-- count[elem(pos)];
	elem(pos) = v;
	++ count[v];
	found = true;
	break;
      }
    }
    if ( found ) {
      ymuint g = 0;
      for (ymuint pos1 = pos + 1; pos1 < k(); ++ pos1) {
	for ( ; count[g] == n(g); ++ g) ;
	++ count[g];
	elem(pos1) = g;
      }
      break;
    }
    if ( pos > 0 ) {
      -- count[elem(pos)];
    }
    else {
      elem(0) = ng;
    }
  }
}

END_NAMESPACE_YM
