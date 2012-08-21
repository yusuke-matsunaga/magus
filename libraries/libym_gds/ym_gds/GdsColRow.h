#ifndef YM_GDS_GDSCOLROW_H
#define YM_GDS_GDSCOLROW_H

/// @file ym_gds/GdsColRow.h
/// @brief GdsColRow のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsColRow GdsColRow.h "ym_gds/GdsColRow.h"
/// @brief COLROW のデータを表すクラス
//////////////////////////////////////////////////////////////////////
class GdsColRow
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] col 列番号
  /// @param[in] row 行番号
  GdsColRow(ymint16 col,
	    ymint16 row);

  /// @brief デストラクタ
  ~GdsColRow();


public:

  /// @brief 列番号を返す．
  ymint16
  col() const;

  /// @brief 行番号を返す．
  ymint16
  row() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 列番号
  ymint16 mCol;

  // 行番号
  ymint16 mRow;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] col 列番号
// @param[in] row 行番号
inline
GdsColRow::GdsColRow(ymint16 col,
		     ymint16 row) :
  mCol(col),
  mRow(row)
{
}

// @brief デストラクタ
inline
GdsColRow::~GdsColRow()
{
}

// @brief 列番号を返す．
inline
ymint16
GdsColRow::col() const
{
  return mCol;
}

// @brief 行番号を返す．
inline
ymint16
GdsColRow::row() const
{
  return mRow;
}

END_NAMESPACE_YM_GDS

#endif // YM_GDS_GDSCOLROW_H
