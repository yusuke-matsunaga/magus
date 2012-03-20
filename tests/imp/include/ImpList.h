#ifndef IMPLIST_H
#define IMPLIST_H

/// @file ImpList.h
/// @brief ImpList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ImpCell.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpListIterator;

//////////////////////////////////////////////////////////////////////
/// @class ImpList ImpList.h "ImpList.h"
/// @brief 含意情報のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class ImpList
{
  friend class ImpInfo;
public:

  typedef ImpListIterator iterator;

public:

  /// @brief コンストラクタ
  ImpList();

  /// @brief デストラクタ
  ~ImpList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 空の時 true を返す．
  bool
  empty() const;

  /// @brief 要素数を得る．
  ymuint
  size() const;

  /// @brief 先頭要素を表す反復子を返す．
  ImpListIterator
  begin() const;

  /// @brief 末尾要素を表す反復子を返す．
  ImpListIterator
  end() const;

  /// @brief 要素を末尾に追加する．
  void
  push_back(ImpCell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mSize;

  // 先頭の要素
  ImpCell* mTop;

  // 末尾の要素
  ImpCell* mLast;

};


//////////////////////////////////////////////////////////////////////
/// @class ImpListIterator ImpInfo.h "ImpInfo.h"
/// @brief ImpList の反復子を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpListIterator
{
  friend class ImpList;
public:

  /// @brief 空のコンストラクタ
  ImpListIterator();

  /// @brief デストラクタ
  ~ImpListIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を取り出す関数
  const ImpCell&
  operator*() const;

  /// @breif 内容を取り出す関数
  const ImpCell*
  operator->() const;

  /// @brief 次の要素に進める．
  /// @return 自身を返す．
  const ImpListIterator&
  operator++();

#if 0
  /// @brief 前の要素に戻る．
  /// @return 自身を返す．
  const ImpListIterator&
  operator--();
#endif

  /// @brief 等価比較関数
  bool
  operator==(const ImpListIterator& right) const;

  /// @brief 非等価比較関数
  bool
  operator!=(const ImpListIterator& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // ImpList から用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を指定したコンストラクタ
  ImpListIterator(ImpCell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素を指すポインタ
  ImpCell* mCur;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
ImpList::ImpList()
{
  mSize = 0;
  mTop = NULL;
  mLast = NULL;
}

// @brief デストラクタ
inline
ImpList::~ImpList()
{
}

// @brief 空の時 true を返す．
inline
bool
ImpList::empty() const
{
  return mSize == 0;
}

// @brief 要素数を得る．
inline
ymuint
ImpList::size() const
{
  return mSize;
}

// @brief 先頭要素を表す反復子を返す．
inline
ImpListIterator
ImpList::begin() const
{
  return ImpListIterator(mTop);
}

// @brief 末尾要素を表す反復子を返す．
inline
ImpListIterator
ImpList::end() const
{
  return ImpListIterator(NULL);
}

// @brief 要素を末尾に追加する．
inline
void
ImpList::push_back(ImpCell* cell)
{
  if ( mTop ) {
    mLast->mNext = cell;
    mLast = cell;
  }
  else {
    mTop = mLast = cell;
  }
  ++ mSize;
}

// @brief 空のコンストラクタ
inline
ImpListIterator::ImpListIterator()
{
  mCur = NULL;
}

// @brief 値を指定したコンストラクタ
inline
ImpListIterator::ImpListIterator(ImpCell* cell)
{
  mCur = cell;
}

// @brief デストラクタ
inline
ImpListIterator::~ImpListIterator()
{
}

// @brief 内容を取り出す関数
inline
const ImpCell&
ImpListIterator::operator*() const
{
  return *mCur;
}

// @breif 内容を取り出す関数
inline
const ImpCell*
ImpListIterator::operator->() const
{
  return mCur;
}

// @brief 次の要素に進める．
// @return 自身を返す．
inline
const ImpListIterator&
ImpListIterator::operator++()
{
  if ( mCur ) {
    mCur = mCur->mNext;
  }
  return *this;
}

#if 0
// @brief 前の要素に戻る．
// @return 自身を返す．
inline
const ImpListIterator&
ImpListIterator::operator--()
{
  if ( mCur ) {
    mCur = mCur->mPrev;
  }
  return *this;
}
#endif

// @brief 等価比較関数
inline
bool
ImpListIterator::operator==(const ImpListIterator& right) const
{
  return mCur == right.mCur;
}

// @brief 非等価比較関数
inline
bool
ImpListIterator::operator!=(const ImpListIterator& right) const
{
  return !operator==(right);
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPLIST_H
