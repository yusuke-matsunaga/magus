
/// @file XorSplitter.cc
/// @brief XorSplitter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "XorSplitter.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス XorSplitter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] varid_list 変数番号のリスト
XorSplitter::XorSplitter(const vector<ymuint32>& varid_list) :
  mVarIdList(varid_list)
{
}

// @brief デストラクタ
XorSplitter::~XorSplitter()
{
}

// @brief 分類数を返す．
ymuint
XorSplitter::split_num() const
{
  return 2;
}

// @brief 変数番号のリストを返す．
const vector<ymuint32>&
XorSplitter::varid_list() const
{
  return mVarIdList;
}

// @brief ベクタを分類する．
// @param[in] vect 対象のベクタ
ymuint
XorSplitter::operator()(const RegVect* vect) const
{
  ymuint ans = 0;
  for (vector<ymuint32>::const_iterator p = mVarIdList.begin();
       p != mVarIdList.end(); ++ p) {
    ymuint32 varid = *p;
    ans ^= vect->val(varid);
  }
  return ans;
}

END_NAMESPACE_YM_IGF
