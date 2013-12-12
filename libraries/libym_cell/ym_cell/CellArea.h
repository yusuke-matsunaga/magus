#ifndef YM_CELL_CELLAREA_H
#define YM_CELL_CELLAREA_H

/// @file ym_cell/CellArea.h
/// @brief CellArea のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/IDO.h"
#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellArea CellArea.h "ym_cell/CellArea.h"
/// @brief 面積を表すクラス
//////////////////////////////////////////////////////////////////////
class CellArea
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellArea();

  /// @brief double からの変換コンストラクタ
  explicit
  CellArea(double v);

  /// @brief デストラクタ
  ~CellArea();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellArea
  infty();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  const CellArea&
  operator=(const CellArea& src);

  /// @brief 加算付き代入演算子
  const CellArea&
  operator+=(const CellArea& src);

  /// @brief 減算付き代入演算子
  const CellArea&
  operator-=(const CellArea& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
/// @param[in] left, right オペランド
/// @relates CellArea
CellArea
operator+(const CellArea& left,
	  const CellArea& right);

/// @brief 減算
/// @param[in] left, right オペランド
/// @relates CellArea
CellArea
operator-(const CellArea& left,
	  const CellArea& right);

/// @brief 等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator==(const CellArea& left,
	   const CellArea& right);

/// @brief 非等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator!=(const CellArea& left,
	   const CellArea& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator<(const CellArea& left,
	  const CellArea& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator>(const CellArea& left,
	  const CellArea& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator<=(const CellArea& left,
	   const CellArea& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator>=(const CellArea& left,
	   const CellArea& right);

/// @brief ストリーム出力
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @relates CellArea
ostream&
operator<<(ostream& s,
	   const CellArea& val);

/// @brief バイナリダンプ
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @relates CellArea
ODO&
operator<<(ODO& s,
	   const CellArea& val);

/// @brief バイナリリストア
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @relates CellArea
IDO&
operator>>(IDO& s,
	   CellArea& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
CellArea::CellArea() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellArea::CellArea(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellArea::~CellArea()
{
}

// @brief 無限大の値を作る．
inline
CellArea
CellArea::infty()
{
  return CellArea(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellArea::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellArea&
CellArea::operator=(const CellArea& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellArea&
CellArea::operator+=(const CellArea& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellArea&
CellArea::operator-=(const CellArea& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellArea
operator+(const CellArea& left,
	  const CellArea& right)
{
  return CellArea(left).operator+=(right);
}

// @brief 減算
inline
CellArea
operator-(const CellArea& left,
	  const CellArea& right)
{
  return CellArea(left).operator-=(right);
}

// @brief 等価比較演算子
// @param[in] left, right オペランド
// @relates CellArea
inline
bool
operator==(const CellArea& left,
	   const CellArea& right)
{
  return left.value() == right.value();
}

// @brief 非等価比較演算子
// @param[in] left, right オペランド
// @relates CellArea
inline
bool
operator!=(const CellArea& left,
	   const CellArea& right)
{
  return !operator==(left, right);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellArea
inline
bool
operator<(const CellArea& left,
	  const CellArea& right)
{
  return left.value() < right.value();
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellArea
inline
bool
operator>(const CellArea& left,
	  const CellArea& right)
{
  return operator<(right, left);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellArea
inline
bool
operator<=(const CellArea& left,
	   const CellArea& right)
{
  return !operator<(right, left);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellArea
inline
bool
operator>=(const CellArea& left,
	   const CellArea& right)
{
  return !operator<(left, right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellArea& val)
{
  return s << val.value();
}

// @brief バイナリダンプ
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @relates CellArea
inline
ODO&
operator<<(ODO& s,
	   const CellArea& val)
{
  return s << val.value();
}

// @brief バイナリリストア
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @relates CellArea
inline
IDO&
operator>>(IDO& s,
	   CellArea& val)
{
  double tmp_val;
  s >> tmp_val;
  val = CellArea(tmp_val);
  return s;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELL_TYPE_H
