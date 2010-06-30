#ifndef LIBYM_SEAL_SMATRIX_H
#define LIBYM_SEAL_SMATRIX_H

/// @file libym_seal/SMatrix.h
/// @brief SMatrix のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Matrix.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_seal/seal_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_SEAL


//////////////////////////////////////////////////////////////////////
/// @class SmCell SMatrix.h "SMatrix.h"
/// @brief SMatrix の要素を表すクラス
//////////////////////////////////////////////////////////////////////
class SmCell
{
  friend class SMatrix;
public:

  /// @brief 行番号を得る．
  ymuint
  row_pos() const;

  /// @brief 列番号を得る．
  ymuint
  col_pos() const;

  /// @brief 値を得る．
  double
  value() const;

  /// @brief 値を設定する．
  void
  set_value(double val);
  
  /// @brief 上に隣接したセルを得る．
  SmCell*
  up() const;

  /// @brief 下に隣接したセルを得る．
  SmCell*
  down() const;

  /// @brief 左に隣接したセルを得る．
  SmCell*
  left() const;

  /// @brief 右に隣接したセルを得る．
  SmCell*
  right() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  ymuint32 mRowPos;

  // 列番号
  ymuint32 mColPos;

  // 値
  double mVal;

  // 上方向のリンク
  SmCell* mUpLink;

  // 下方向のリンク
  SmCell* mDownLink;

  // 左方向のリンク
  SmCell* mLeftLink;

  // 右方向のリンク
  SmCell* mRightLink;
  
};


//////////////////////////////////////////////////////////////////////
/// @class SMatrix SMatrix.h "SMatrix.h"
/// @brief 行列を表すクラス
//////////////////////////////////////////////////////////////////////
class SMatrix
{
public:

  /// @brief コンストラクタ
  /// @param[in] size 変数の数
  /// @note 内容は空
  SMatrix(ymuint size);

#if 0
  /// @brief コピーコンストラクタ
  SMatrix(const SMatrix& src);

  /// @brief 代入演算子
  const SMatrix&
  operator=(const SMatrix& src);
#endif

  /// @brief デストラクタ
  ~SMatrix();

  
public:

  /// @brief 変数の数を返す．
  ymuint32
  size() const;

  /// @brief 行の先頭を得る．
  /// @param[in] pos 行番号 ( 0 <= pos < size() )
  SmCell*
  row_top(ymuint pos) const;

  /// @brief 行の末尾を得る．
  /// @param[in] pos 行番号 ( 0 <= pos < size() )
  /// @note このセルはダミー
  SmCell*
  row_end(ymuint pos) const;

  /// @brief 行の要素数を得る．
  /// @param[in] pos 行番号 ( 0 <= pos < size() )
  ymuint
  row_num(ymuint pos) const;

  /// @brief 列の先頭を得る．
  /// @param[in] pos 列番号 ( 0 <= pos < size() )
  SmCell*
  col_top(ymuint pos) const;

  /// @brief 列の末尾を得る．
  /// @param[in] pos 列番号 ( 0 <= pos < size() )
  /// @note このセルはダミー
  SmCell*
  col_end(ymuint pos) const;

  /// @brief 列の要素数を得る．
  ymuint
  col_num(ymuint pos) const;

  /// @brief 要素を取り出す．
  /// @param[in] row 行番号
  /// @param[in] col 列番号
  /// @note 要素がない場合には NULL を返す．
  SmCell*
  find_elem(ymuint row,
	    ymuint col) const;

  /// @brief 定数項を取り出す．
  /// @param[in] row 行番号
  double
  const_elem(ymuint row) const;


public:

  /// @brief 要素の値を設定する．
  /// @param[in] row 行番号
  /// @param[in] col 列番号
  /// @param[in] val 値
  void
  set_value(ymuint row,
	    ymuint col,
	    double val);

  /// @brief 定数項を設定する．
  /// @param[in] row 行番号
  /// @param[in] val 値
  void
  set_const(ymuint row,
	    double value);

  /// @brief ピボット演算を行う．
  void
  pivot(ymuint src_row,
	ymuint src_col,
	ymuint dst_row);


#ifdef SANITY_CHECK
  void
  sanity_check(const char* msg);
#endif


private:

  /// @brief セルを列に挿入する．
  void
  insert_col(SmCell* cell);

  /// @brief セルを確保する．
  SmCell*
  new_cell(ymuint row,
	   ymuint col,
	   double val);

  /// @brief セルを削除する．
  void
  delete_cell(SmCell* cell);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルの確保用アロケータ
  UnitAlloc mAlloc;
  
  // サイズ
  ymuint32 mSize;

  // 行の先頭の配列
  SmCell* mRowArray;

  // 行の要素数の配列
  ymuint32* mRowNumArray;

  // 列の先頭の配列
  SmCell* mColArray;

  // 列の要素数の配列
  ymuint32* mColNumArray;

  // 定数項の配列
  double* mConstArray;

#ifdef SANITY_CHECK
  double* mShadowArray;
#endif
  
};


/// @brief 行列の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] m 対象の行列
void
display(ostream& s,
	const SMatrix& m);

/// @brief ガウスの消去法を行って連立方程式の解を得る．
/// @param[in] src_matrix 対象の行列
/// @param[in] solution 解を格納するベクタ
/// @return 解が得られたら true を返す．
bool
gaussian_elimination(const SMatrix& src_matrix,
		     vector<double>& solution);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 行番号を得る．
inline
ymuint
SmCell::row_pos() const
{
  return mRowPos;
}

// @brief 列番号を得る．
inline
ymuint
SmCell::col_pos() const
{
  return mColPos;
}

// @brief 値を得る．
inline
double
SmCell::value() const
{
  return mVal;
}

// @brief 値を設定する．
inline
void
SmCell::set_value(double val)
{
  mVal = val;
}

// @brief 上に隣接したセルを得る．
inline
SmCell*
SmCell::up() const
{
  return mUpLink;
}

// @brief 下に隣接したセルを得る．
inline
SmCell*
SmCell::down() const
{
  return mDownLink;
}

// @brief 左に隣接したセルを得る．
inline
SmCell*
SmCell::left() const
{
  return mLeftLink;
}

// @brief 右に隣接したセルを得る．
inline
SmCell*
SmCell::right() const
{
  return mRightLink;
}

// @brief 行数を返す．
inline
ymuint32
SMatrix::size() const
{
  return mSize;
}

// @brief 行の先頭を得る．
// @param[in] pos 行番号 ( 0 <= pos < size() )
inline
SmCell*
SMatrix::row_top(ymuint pos) const
{
  return mRowArray[pos].right();
}

// @brief 行の末尾を得る．
// @param[in] pos 行番号 ( 0 <= pos < size() )
// @note このセルはダミー
inline
SmCell*
SMatrix::row_end(ymuint pos) const
{
  return &mRowArray[pos];
}

// @brief 行の要素数を得る．
// @param[in] pos 行番号 ( 0 <= pos < size() )
inline
ymuint
SMatrix::row_num(ymuint pos) const
{
  return mRowNumArray[pos];
}

// @brief 列の先頭を得る．
// @param[in] pos 列番号 ( 0 <= pos < size() )
inline
SmCell*
SMatrix::col_top(ymuint pos) const
{
  return mColArray[pos].down();
}

// @brief 列の末尾を得る．
// @param[in] pos 列番号 ( 0 <= pos < size() )
// @note このセルはダミー
inline
SmCell*
SMatrix::col_end(ymuint pos) const
{
  return &mColArray[pos];
}

// @brief 列の要素数を得る．
inline
ymuint
SMatrix::col_num(ymuint pos) const
{
  return mColNumArray[pos];
}

// @brief 定数項を取り出す．
// @param[in] row 行番号
inline
double
SMatrix::const_elem(ymuint row) const
{
  return mConstArray[row];
}

// @brief 定数項を設定する．
// @param[in] row 行番号
// @param[in] val 値
inline
void
SMatrix::set_const(ymuint row,
		   double value)
{
  mConstArray[row] = value;
}

END_NAMESPACE_YM_SEAL

#endif // LIBYM_SEAL_SMATRIX_H
