#ifndef MCCELL_H
#define MCCELL_H

/// @file McCell.h
/// @brief McCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmUtils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class McCell McCell.h "McCell.h"
/// @brief mincov 用の行列要素を表すクラス
//////////////////////////////////////////////////////////////////////
class McCell
{
  friend class McRowHead;
  friend class McColHead;
  friend class McMatrix;

private:

  /// @brief コンストラクタ
  McCell();

  /// @brief デストラクタ
  ~McCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号を返す．
  ymuint32
  row_pos() const;

  /// @brief 列番号を返す．
  ymuint32
  col_pos() const;

  /// @brief 同じ行の直前の要素を返す．
  const McCell*
  row_prev() const;

  /// @brief 同じ行の直後の要素を返す．
  const McCell*
  row_next() const;

  /// @brief 同じ列の直前の要素を返す．
  const McCell*
  col_prev() const;

  /// @brief 同じ列の直後の要素を返す．
  const McCell*
  col_next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  ymuint32 mRowPos;

  // 列番号
  ymuint32 mColPos;

  // 左隣の要素
  McCell* mLeftLink;

  // 右隣の要素
  McCell* mRightLink;

  // 上の要素
  McCell* mUpLink;

  // 下の要素
  McCell* mDownLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
McCell::McCell()
{
}

// @brief デストラクタ
inline
McCell::~McCell()
{
}

// @brief 行番号を返す．
inline
ymuint32
McCell::row_pos() const
{
  return mRowPos;
}

// @brief 列番号を返す．
inline
ymuint32
McCell::col_pos() const
{
  return mColPos;
}

// @brief 同じ行の直前の要素を返す．
inline
const McCell*
McCell::row_prev() const
{
  return mLeftLink;
}

// @brief 同じ行の直後の要素を返す．
inline
const McCell*
McCell::row_next() const
{
  return mRightLink;
}

// @brief 同じ列の直前の要素を返す．
inline
const McCell*
McCell::col_prev() const
{
  return mUpLink;
}

// @brief 同じ列の直後の要素を返す．
inline
const McCell*
McCell::col_next() const
{
  return mDownLink;
}

END_NAMESPACE_YM_SATPG

#endif // MCCELL_H
