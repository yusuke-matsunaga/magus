#ifndef IMPVAL_H
#define IMPVAL_H

/// @file ImpVal.h
/// @brief ImpVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpNode;

//////////////////////////////////////////////////////////////////////
/// @class ImpVal ImpVal.h "ImpVal.h"
/// @brief ノード番号と値の組を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpVal
{
public:

  /// @brief 空のコンストラクタ
  ImpVal();

  /// @brief コンストラクタ
  ImpVal(ymuint pval);

  /// @brief コンストラクタ
  /// @param[in] id ノード番号
  /// @param[in] val 値
  ImpVal(ymuint id,
	 ymuint val);

  /// @brief デストラクタ
  ~ImpVal();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を設定する．
  /// @param[in] id ノード番号
  /// @param[in] val 値
  void
  set(ymuint id,
      ymuint val);


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号を取り出す．
  ymuint
  id() const;

  /// @brief 値 ( 0 or 1 )
  ymuint
  val() const;

  /// @brief 圧縮した値を返す．
  ymuint
  packed_val() const;

  /// @brief 等価比較演算子
  bool
  operator==(const ImpVal& right) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const ImpVal& right) const;

  /// @brief 大小比較演算子
  bool
  operator<(const ImpVal& right) const;

  /// @brief 大小比較演算子
  bool
  operator>(const ImpVal& right) const;

  /// @brief 大小比較演算子
  bool
  operator<=(const ImpVal& right) const;

  /// @brief 大小比較演算子
  bool
  operator>=(const ImpVal& right) const;

  /// @brief 内容を出力する．
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号 + 値
  ymuint32 mVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
ImpVal::ImpVal()
{
  mVal = 0U;
}

// @brief コンストラクタ
inline
ImpVal::ImpVal(ymuint pval)
{
  mVal = pval;
}

// @brief コンストラクタ
// @param[in] id ノード番号
// @param[in] val 値
inline
ImpVal::ImpVal(ymuint id,
	       ymuint val)
{
  set(id, val);
}

// @brief デストラクタ
inline
ImpVal::~ImpVal()
{
}

// @brief 値を設定する．
// @param[in] id ノード番号
// @param[in] val 値
inline
void
ImpVal::set(ymuint id,
	    ymuint val)
{
  mVal = id * 2 + val;
}

// @brief ノード番号を取り出す．
inline
ymuint
ImpVal::id() const
{
  return mVal >> 1;
}

// @brief 値 ( 0 or 1 )
inline
ymuint
ImpVal::val() const
{
  return mVal & 1U;
}

// @brief 圧縮した値を返す．
inline
ymuint
ImpVal::packed_val() const
{
  return mVal;
}

// @brief 等価比較演算子
inline
bool
ImpVal::operator==(const ImpVal& right) const
{
  return mVal == right.mVal;
}

// @brief 非等価比較演算子
inline
bool
ImpVal::operator!=(const ImpVal& right) const
{
  return !operator==(right);
}

// @brief 大小比較演算子
inline
bool
ImpVal::operator<(const ImpVal& right) const
{
  return mVal < right.mVal;
}

// @brief 大小比較演算子
inline
bool
ImpVal::operator>(const ImpVal& right) const
{
  return right.operator<(*this);
}

// @brief 大小比較演算子
inline
bool
ImpVal::operator<=(const ImpVal& right) const
{
  return !right.operator<(*this);
}

// @brief 大小比較演算子
inline
bool
ImpVal::operator>=(const ImpVal& right) const
{
  return !operator<(right);
}

// @brief 内容を出力する．
inline
void
ImpVal::print(ostream& s) const
{
  if ( val() == 0 ) {
    s << "~";
  }
  s << id();
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPVAL_H
