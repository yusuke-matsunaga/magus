
/// @file Variable.cc
/// @brief Variable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Variable.h"
#include "RegVect.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス Variable
//////////////////////////////////////////////////////////////////////

// @brief 通常の変数用のコンストラクタ
// @param[in] vid 変数番号
Variable::Variable(ymuint vid) :
  mVidList(1, vid)
{
}

// @brief 合成変数用のコンストラクタ
// @param[in] vid_list 変数番号のリスト
Variable::Variable(const vector<ymuint>& vid_list) :
  mVidList(vid_list)
{
  sort(mVidList.begin(), mVidList.end());
}

// @brief デストラクタ
Variable::~Variable()
{
}

// @brief 変数番号のリストを得る．
const vector<ymuint>&
Variable::vid_list() const
{
  return mVidList;
}

// @brief ベクタを分類する．
// @param[in] vect 対象のベクタ
ymuint
Variable::classify(const RegVect* vect) const
{
  ymuint n = mVidList.size();
  ymuint ans = vect->val(mVidList[0]);
  for (ymuint i = 1; i < n; ++ i) {
    ymuint varid = mVidList[i];
    ans ^= vect->val(varid);
  }
  return ans;
}

// @brief 内容を出力する．
void
Variable::dump(ostream& s) const
{
  if ( mVidList.size() == 1 ) {
    // 単一の変数の時
    s << mVidList[0];
  }
  else {
    // 合成変数の時
    s << "(";
    for (vector<ymuint>::const_iterator q = mVidList.begin();
	 q != mVidList.end(); ++ q) {
      s << " " << *q;
    }
    s << ")";
  }
}

END_NAMESPACE_YM_IGF
