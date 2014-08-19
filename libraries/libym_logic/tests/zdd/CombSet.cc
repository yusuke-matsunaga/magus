
/// @file CombSet.cc
/// @brief CombSet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CombSet.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス CombSet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num 要素数
// @note 空の集合となる．
CombSet::CombSet(ymuint num) :
  mNum(num)
{
}

// @brief デストラクタ
CombSet::~CombSet()
{
}

// @brief 要素数を返す．
ymuint
CombSet::num() const
{
  return mNum;
}

// @brief 空集合 {} にする．
const CombSet&
CombSet::make_empty()
{
  mBody.clear();
  return *this;
}

// @brief base set {{}} にする．
const CombSet&
CombSet::make_base()
{
  mBody.clear();
  mBody.push_back(0UL);
  return *this;
}

// @brief var の出現を反転させる．
// @param[in] var 変数番号
const CombSet&
CombSet::swap(VarId var)
{
  ymuint32 mask = 1UL << var.val();
  for (vector<ymuint32>::iterator p = mBody.begin();
       p != mBody.end(); ++ p) {
    *p ^= mask;
  }
  return *this;
}

// @brief intersection を計算する．
CombSet
operator&(const CombSet& left,
	  const CombSet& right)
{
  assert_cond( left.num() == right.num(), __FILE__, __LINE__);
  vector<ymuint32>::const_iterator p1 = left.mBody.begin();
  vector<ymuint32>::const_iterator e1 = left.mBody.end();
  vector<ymuint32>::const_iterator p2 = right.mBody.begin();
  vector<ymuint32>::const_iterator e2 = right.mBody.end();
  CombSet ans(left.num());
  while ( p1 != e1 && p2 != e2 ) {
    ymuint32 a = *p1;
    ymuint32 b = *p2;
    if ( a < b ) {
      ++ p1;
    }
    else if ( a > b ) {
      ++ p2;
    }
    else {
      ans.mBody.push_back(a);
      ++ p1;
      ++ p2;
    }
  }
  for ( ; p1 != e1; ++ p1) {
    ans.mBody.push_back(*p1);
  }
  for ( ; p2 != e2; ++ p2) {
    ans.mBody.push_back(*p2);
  }
  return ans;
}

// @brief union を計算する．
CombSet
operator|(const CombSet& left,
	  const CombSet& right)
{
  assert_cond( left.num() == right.num(), __FILE__, __LINE__);
  vector<ymuint32>::const_iterator p1 = left.mBody.begin();
  vector<ymuint32>::const_iterator e1 = left.mBody.end();
  vector<ymuint32>::const_iterator p2 = right.mBody.begin();
  vector<ymuint32>::const_iterator e2 = right.mBody.end();
  CombSet ans(left.num());
  while ( p1 != e1 && p2 != e2 ) {
    ymuint32 a = *p1;
    ymuint32 b = *p2;
    if ( a < b ) {
      ans.mBody.push_back(a);
      ++ p1;
    }
    else if ( a > b ) {
      ans.mBody.push_back(b);
      ++ p2;
    }
    else {
      ans.mBody.push_back(a);
      ++ p1;
      ++ p2;
    }
  }
  for ( ; p1 != e1; ++ p1) {
    ans.mBody.push_back(*p1);
  }
  for ( ; p2 != e2; ++ p2) {
    ans.mBody.push_back(*p2);
  }
  return ans;
}

// @brief 集合差を計算する．
CombSet
operator-(const CombSet& left,
	  const CombSet& right)
{
  assert_cond( left.num() == right.num(), __FILE__, __LINE__);
  vector<ymuint32>::const_iterator p1 = left.mBody.begin();
  vector<ymuint32>::const_iterator e1 = left.mBody.end();
  vector<ymuint32>::const_iterator p2 = right.mBody.begin();
  vector<ymuint32>::const_iterator e2 = right.mBody.end();
  CombSet ans(left.num());
  while ( p1 != e1 && p2 != e2 ) {
    ymuint32 a = *p1;
    ymuint32 b = *p2;
    if ( a < b ) {
      ans.mBody.push_back(a);
      ++ p1;
    }
    else if ( a > b ) {
      ++ p2;
    }
    else {
      ++ p1;
      ++ p2;
    }
  }
  for ( ; p1 != e1; ++ p1) {
    ans.mBody.push_back(*p1);
  }
  return ans;
}

// @brief cofactor0 を計算する．
CombSet
CombSet::cofactor0(VarId var) const
{
  ymuint32 mask = 1UL << var.val();
  CombSet ans(num());
  for (vector<ymuint32>::const_iterator p1 = mBody.begin();
       p1 != mBody.end(); ++ p1) {
    ymuint32 a = *p1;
    if ( (a & mask) == 0UL ) {
      ans.mBody.push_back(a);
    }
  }
  return ans;
}

// @brief cofactor1 を計算する．
CombSet
CombSet::cofactor1(VarId var) const
{
  ymuint32 mask = 1UL << var.val();
  CombSet ans(num());
  for (vector<ymuint32>::const_iterator p1 = mBody.begin();
       p1 != mBody.end(); ++ p1) {
    ymuint32 a = *p1;
    if ( (a & mask) == mask ) {
      ans.mBody.push_back(a);
    }
  }
  return ans;
}

// @brief 内容を出力する．
void
CombSet::print(ostream& s) const
{
  s << "{";
  const char* comma = "";
  for (vector<ymuint32>::const_iterator p1 = mBody.begin();
       p1 != mBody.end(); ++ p1) {
    ymuint32 a = *p1;
    s << comma << a;
    comma = ", ";
  }
  s << "}" << endl;
}

END_NAMESPACE_YM_ZDD
