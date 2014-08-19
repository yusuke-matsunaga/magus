#ifndef MCROWHEAD_H
#define MCROWHEAD_H

/// @file McRowHead.h
/// @brief McRowHead のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"
#include "McCell.h"
#include "YmUtils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McRowHead McRowHead.h "McRowHead.h"
/// @brief Mincov 用の行の先頭要素を表すクラス
//////////////////////////////////////////////////////////////////////
class McRowHead
{
  friend class McMatrix;

private:

  /// @brief コンストラクタ
  /// @param[in] pos 行番号
  McRowHead(ymuint pos);

  /// @brief デストラクタ
  ~McRowHead();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 行番号を返す．
  ymuint32
  pos() const;

  /// @brief 要素数を返す．
  ymuint32
  num() const;

  /// @brief 先頭の要素を返す．
  const McCell*
  front() const;

  /// @brief 末尾の要素を返す．
  const McCell*
  back() const;

  /// @brief cell が終端かどうか調べる．
  /// @param[in] cell 対象の要素
  /// @return cell が終端の時 true を返す．
  bool
  is_end(const McCell* cell) const;

  /// @brief 直前の行を返す．
  const McRowHead*
  prev() const;

  /// @brief 直後の行を返す．
  const McRowHead*
  next() const;


public:

  mutable
  ymuint32 mWork;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 接続している要素をクリアする．
  void
  clear();

  /// @brief 追加する要素の両脇を求める．
  /// @param[in] cell 対象の要素
  /// @retval true 正しく処理された．
  /// @retval false 同一の要素が存在した．
  /// @note cell の mRightLink, mLeftLink に結果がセットされる．
  bool
  search_insert_pos(McCell* cell);

  /// @brief 要素を追加する．
  /// @param[in] cell 追加する要素
  /// @note cell の mRightLink, mLeftLink に適切な値がセットされているものとする．
  void
  insert_elem(McCell* cell);

  /// @brief 要素を削除する．
  /// @param[in] cell 削除する要素
  void
  delete_elem(McCell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 行番号
  ymuint32 mPos;

  // 要素数
  ymuint32 mNum;

  // 二重連結リストのダミーヘッダ
  McCell mDummy;

  // 直前の行を指すリンク
  McRowHead* mPrev;

  // 直後の列を指すリンク
  McRowHead* mNext;

  // 削除フラグ
  bool mDeleted;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pos 行番号
inline
McRowHead::McRowHead(ymuint pos) :
  mPos(pos),
  mNum(0),
  mPrev(NULL),
  mNext(NULL),
  mDeleted(false)
{
  mDummy.mLeftLink = &mDummy;
  mDummy.mRightLink = &mDummy;
}

/// @brief デストラクタ
inline
McRowHead::~McRowHead()
{
}

// @brief 行番号を返す．
inline
ymuint32
McRowHead::pos() const
{
  return mPos;
}

// @brief 要素数を返す．
inline
ymuint32
McRowHead::num() const
{
  return mNum;
}

// @brief 先頭の要素を返す．
inline
const McCell*
McRowHead::front() const
{
  return mDummy.mRightLink;
}

// @brief 末尾の要素を返す．
inline
const McCell*
McRowHead::back() const
{
  return mDummy.mLeftLink;
}

// @brief cell が終端かどうか調べる．
// @param[in] cell 対象の要素
// @return cell が終端の時 true を返す．
inline
bool
McRowHead::is_end(const McCell* cell) const
{
  return cell == &mDummy;
}

// @brief 直前の行を返す．
inline
const McRowHead*
McRowHead::prev() const
{
  return mPrev;
}

// @brief 直後の行を返す．
inline
const McRowHead*
McRowHead::next() const
{
  return mNext;
}

// @brief 接続している要素をクリアする．
inline
void
McRowHead::clear()
{
  mDummy.mLeftLink = &mDummy;
  mDummy.mRightLink = &mDummy;
  mNum = 0;
}

// @brief 要素を追加する．
// @param[in] cell 追加する要素
// @note cell の mRightLink, mLeftLink に適切な値がセットされているものとする．
inline
void
McRowHead::insert_elem(McCell* cell)
{
  McCell* prev = cell->mLeftLink;
  McCell* next = cell->mRightLink;

  if ( prev->mRightLink != cell ) {
    ++ mNum;
  }

  prev->mRightLink = cell;
  next->mLeftLink = cell;
}

// @brief 要素を削除する．
// @param[in] cell 削除する要素
inline
void
McRowHead::delete_elem(McCell* cell)
{
  McCell* prev = cell->mLeftLink;
  McCell* next = cell->mRightLink;

  prev->mRightLink = next;
  next->mLeftLink = prev;
  -- mNum;
}

END_NAMESPACE_YM_MINCOV

#endif // MCROWHEAD_H
