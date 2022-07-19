
/// @file SupportList.cc
/// @brief SupportList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "SupportList.h"


BEGIN_NAMESPACE_DG

BEGIN_NONAMESPACE

/// @brief マージを行う実際の関数
vector<SizeType>
merge(
  const vector<SizeType>& list1,
  const vector<SizeType>& list2
)
{
  vector<SizeType> ans_list;
  SizeType n1 = list1.size();
  SizeType n2 = list2.size();
  SizeType i1 = 0;
  SizeType i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    SizeType v1 = list1[i1];
    SizeType v2 = list2[i2];
    if ( v1 < v2 ) {
      ans_list.push_back(v1);
      ++ i1;
    }
    else if ( v1 == v2 ) {
      ans_list.push_back(v1);
      ++ i1;
      ++ i2;
    }
    else { // v1 > v2
      ans_list.push_back(v2);
      ++ i2;
    }
  }
  while ( i1 < n1 ) {
    SizeType v1 = list1[i1];
    ans_list.push_back(v1);
    ++ i1;
  }
  while ( i2 < n2 ) {
    SizeType v2 = list2[i2];
    ans_list.push_back(v2);
    ++ i2;
  }
  return ans_list;
}

/// @brief 残差を求める実際の関数
vector<SizeType>
diff(
  const vector<SizeType>& list1,
  const vector<SizeType>& list2
)
{
  vector<SizeType> ans_list;
  SizeType n1 = list1.size();
  SizeType n2 = list2.size();
  SizeType i1 = 0;
  SizeType i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    SizeType v1 = list1[i1];
    SizeType v2 = list2[i2];
    if ( v1 < v2 ) {
      ans_list.push_back(v1);
      ++ i1;
    }
    else if ( v1 == v2 ) {
      ++ i1;
      ++ i2;
    }
    else { // v1 > v2
      ++ i2;
    }
  }
  while ( i1 < n1 ) {
    SizeType v1 = list1[i1];
    ans_list.push_back(v1);
    ++ i1;
  }
  return ans_list;
}

/// @brief インターセクションを求める実際の関数
vector<SizeType>
intersection(
  const vector<SizeType>& list1,
  const vector<SizeType>& list2
)
{
  vector<SizeType> ans_list;
  SizeType n1 = list1.size();
  SizeType n2 = list2.size();
  SizeType i1 = 0;
  SizeType i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    SizeType v1 = list1[i1];
    SizeType v2 = list2[i2];
    if ( v1 < v2 ) {
      ++ i1;
    }
    else if ( v1 == v2 ) {
      ans_list.push_back(v1);
      ++ i1;
      ++ i2;
    }
    else { // v1 > v2
      ++ i2;
    }
  }
  return ans_list;
}

END_NONAMESPACE

// @brief マージする．
SupportList
SupportList::operator+(
  const SupportList& right
) const
{
  auto ans_list = merge(mBody, right.mBody);
  return SupportList{std::move(ans_list)};
}

// @brief 残差を求める．
SupportList
SupportList::operator-(
  const SupportList& right
) const
{
  auto ans_list = diff(mBody, right.mBody);
  return SupportList{std::move(ans_list)};
}

// @brief インターセクションを求める．
SupportList
SupportList::operator&(
  const SupportList& right
) const
{
  auto ans_list = intersection(mBody, right.mBody);
  return SupportList{std::move(ans_list)};
}

// @brief インターセクションが空でないか調べる．
bool
SupportList::operator&&(
  const SupportList& right ///< [in] オペランド
) const
{
  const auto& list1 = mBody;
  const auto& list2 = right.mBody;
  SizeType n1 = list1.size();
  SizeType n2 = list2.size();
  SizeType i1 = 0;
  SizeType i2 = 0;
  while ( i1 < n1 && i2 < n2 ) {
    SizeType v1 = list1[i1];
    SizeType v2 = list2[i2];
    if ( v1 < v2 ) {
      ++ i1;
    }
    else if ( v1 == v2 ) {
      return true;
    }
    else { // v1 > v2
      ++ i2;
    }
  }
  return false;
}

// @brief 自身にマージ結果を代入する．
SupportList&
SupportList::operator+=(
  const SupportList& right
)
{
  auto ans_list = merge(mBody, right.mBody);
  mBody.swap(ans_list);
  return *this;
}

// @brief 残差を自分自身に代入する．
SupportList&
SupportList::operator-=(
  const SupportList& right
)
{
  auto ans_list = diff(mBody, right.mBody);
  mBody.swap(ans_list);
  return *this;
}

// @brief インターセクションを自分自身に代入する．
SupportList&
SupportList::operator&=(
  const SupportList& right
)
{
  auto ans_list = intersection(mBody, right.mBody);
  mBody.swap(ans_list);
  return *this;
}

END_NAMESPACE_DG
