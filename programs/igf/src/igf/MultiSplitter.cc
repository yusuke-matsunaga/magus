
/// @file MultiSplitter.cc
/// @brief MultiSplitter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "MultiSplitter.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス MultiSplitter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] splitter_list 元となる Splitter のリスト
MultiSplitter::MultiSplitter(const vector<Splitter*>& splitter_list) :
  mSplitterList(splitter_list)
{
}

// @brief デストラクタ
MultiSplitter::~MultiSplitter()
{
}

// @brief 分類数を返す．
ymuint
MultiSplitter::split_num() const
{
  ymuint ans = 1;
  for (vector<Splitter*>::const_iterator p = mSplitterList.begin();
       p != mSplitterList.end(); ++ p) {
    Splitter& splitter = **p;
    ans *= splitter.split_num();
  }
  return ans;
}

// @brief ベクタを分類する．
// @param[in] vect 対象のベクタ
ymuint
MultiSplitter::operator()(const RegVect* vect) const
{
  ymuint ans = 0;
  for (vector<Splitter*>::const_iterator p = mSplitterList.begin();
       p != mSplitterList.end(); ++ p) {
    Splitter& splitter = **p;
    ans = ans * splitter.split_num() + splitter(vect);
  }
  return ans;
}


END_NAMESPACE_YM_IGF
