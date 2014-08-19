#ifndef IMPVALS_H
#define IMPVALS_H

/// @file ImpVals.h
/// @brief ImpVals のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ImpVals ImpVals.h "ImpVals.h"
/// @brief ImpVal の集合を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpVals
{
public:

  /// @brief 空のコンストラクタ
  ImpVals();

  /// @brief 一つの要素を指定したコンストラクタ
  ImpVals(ImpVal val);


public:

  /// @brief 要素数を得る．
  ymuint
  num() const;

  /// @brief 要素を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < num() )
  const ImpVal&
  val(ymuint pos) const;

  /// @brief 要素を追加する．
  void
  add(const ImpVal& val);

  /// @brief 等価比較演算子
  bool
  operator==(const ImpVals& right) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const ImpVals& right) const;

  /// @brief 大小比較演算子
  bool
  operator<(const ImpVals& right) const;

  /// @brief 大小比較演算子
  bool
  operator>(const ImpVals& right) const;

  /// @brief 大小比較演算子
  bool
  operator<=(const ImpVals& right) const;

  /// @brief 大小比較演算子
  bool
  operator>=(const ImpVals& right) const;

  /// @brief 内容を出力する
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mNum;

  // 値の配列
  ImpVal mArray[5];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
ImpVals::ImpVals() :
  mNum(0)
{
}

// @brief 一つの要素を指定したコンストラクタ
inline
ImpVals::ImpVals(ImpVal val) :
  mNum(1)
{
  mArray[0] = val;
}

// @brief 要素数を得る．
inline
ymuint
ImpVals::num() const
{
  return mNum;
}

// @brief 要素を得る．
// @param[in] pos 位置番号 ( 0 <= pos < num() )
inline
const ImpVal&
ImpVals::val(ymuint pos) const
{
  assert_cond( pos < mNum, __FILE__, __LINE__);
  return mArray[pos];
}

// @brief 要素を追加する．
inline
void
ImpVals::add(const ImpVal& val)
{
  ymuint i;
  for (i = 0; i < mNum; ++ i) {
    if ( mArray[i] == val ) {
      return;
    }
    if ( mArray[i] > val ) {
      break;
    }
  }
  assert_cond( mNum < 5, __FILE__, __LINE__);
  if ( i < mNum ) {
    for (ymuint j = mNum - 1; j > i; --j) {
      mArray[j] = mArray[j - 1];
    }
  }
  mArray[i] = val;
  ++ mNum;
}

// @brief 等価比較演算子
inline
bool
ImpVals::operator==(const ImpVals& right) const
{
  if ( mNum != right.mNum ) {
    return false;
  }
  for (ymuint i = 0; i < mNum; ++ i) {
    if ( mArray[i] != right.mArray[i] ) {
      return false;
    }
  }
  return true;
}

// @brief 非等価比較演算子
inline
bool
ImpVals::operator!=(const ImpVals& right) const
{
  return !operator==(right);
}

// @brief 大小比較演算子
inline
bool
ImpVals::operator<(const ImpVals& right) const
{
  ymuint min_num = mNum;
  if ( min_num > right.mNum ) {
    min_num = right.mNum;
  }
  for (ymuint i = 0; i < min_num; ++ i) {
    if ( mArray[i] < right.mArray[i] ) {
      return true;
    }
    if ( mArray[i] > right.mArray[i] ) {
      return false;
    }
  }
  if ( mNum < right.mNum ) {
    return true;
  }
  return false;
}

// @brief 大小比較演算子
inline
bool
ImpVals::operator>(const ImpVals& right) const
{
  return right.operator<(*this);
}

// @brief 大小比較演算子
inline
bool
ImpVals::operator<=(const ImpVals& right) const
{
  return !right.operator<(*this);
}

// @brief 大小比較演算子
inline
bool
ImpVals::operator>=(const ImpVals& right) const
{
  return !operator<(right);
}

// @brief 内容を出力する
inline
void
ImpVals::print(ostream& s) const
{
  s << "{";
  for (ymuint i = 0; i < mNum; ++ i) {
    s << " ";
    mArray[i].print(s);
  }
  s << "}";
}


END_NAMESPACE_YM_NETWORKS

#endif // IMPVALS_H
