#ifndef ASTLIST_H
#define ASTLIST_H

/// @file AstList.h
/// @brief AstList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

template <typename T>
struct AstListCell
{
  T* mCar;
  AstListCell* mCdr;
};

template <typename T>
class AstListIterator;

//////////////////////////////////////////////////////////////////////
/// @class AstList AstList.h "AstList.h"
/// @brief Ast のリストを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstList
{
  friend class AstListIterator<T>;

public:

  typedef AstListCell<T> Cell;
  typedef AstListIterator<T> Iterator;

public:

  /// @brief コンストラクタ
  AstList();

  /// @brief デストラクタ
  ~AstList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を追加する．
  /// @param[in] item 追加する要素．
  void
  add(T* item);

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief 先頭の反復子を取り出す．
  Iterator
  begin() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mSize;

  // 先頭の要素
  Cell* mTop;

  // 末尾の要素
  Cell* mTail;

};


//////////////////////////////////////////////////////////////////////
/// @class AstListIterator AstList.h "AstList.h"
/// @brief AstList の反復子
//////////////////////////////////////////////////////////////////////
template <typename T>
class AstListIterator
{
  typedef AstListCell<T> Cell;

public:

  /// @brief コンストラクタ
  AstListIterator(AstListCell<T>* cell = NULL);

  /// @brief デストラクタ
  ~AstListIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を得る．
  T*
  operator*() const;

  /// @brief 末尾の時に true を返す．
  bool
  is_end() const;

  /// @brief 次の要素を指すように進める．
  void
  next();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在のセル
  Cell* mCell;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス AstList<T>
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template <typename T>
inline
AstList<T>::AstList()
{
  mSize = 0;
  mTop = NULL;
  mTail = NULL;
}

// @brief デストラクタ
template <typename T>
inline
AstList<T>::~AstList()
{
  for (Cell* cell = mTop; cell; ) {
    Cell* next = cell->mCdr;
    delete cell;
    cell = next;
  }
}

// @brief 要素を追加する．
// @param[in] item 追加する要素．
template <typename T>
inline
void
AstList<T>::add(T* item)
{
  Cell* cell = new Cell;
  cell->mCar = item;
  cell->mCdr = NULL;
  if ( mTail == NULL ) {
    ASSERT_COND( mTop == NULL );
    mTop = cell;
    mTail = cell;
  }
  else {
    ASSERT_COND( mTop != NULL );
    mTail->mCdr = cell;
    mTail = cell;
  }
  ++ mSize;
}

// @brief 要素数を返す．
template <typename T>
inline
ymuint
AstList<T>::size() const
{
  return mSize;
}

// @brief 先頭の反復子を取り出す．
template <typename T>
inline
AstListIterator<T>
AstList<T>::begin() const
{
  return AstListIterator<T>(mTop);
}


//////////////////////////////////////////////////////////////////////
// クラス AstListIterator<T>
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
template <typename T>
inline
AstListIterator<T>::AstListIterator(AstListCell<T>* cell)
{
  mCell = cell;
}

// @brief デストラクタ
template <typename T>
inline
AstListIterator<T>::~AstListIterator()
{
}

// @brief 内容を得る．
template <typename T>
inline
T*
AstListIterator<T>::operator*() const
{
  if ( mCell != NULL ) {
    return mCell->mCar;
  }
  return NULL;
}

// @brief 末尾の時に true を返す．
template <typename T>
inline
bool
AstListIterator<T>::is_end() const
{
  return mCell == NULL;
}

// @brief 次の要素を指すように進める．
template <typename T>
inline
void
AstListIterator<T>::next()
{
  if ( mCell != NULL ) {
    mCell = mCell->mCdr;
  }
}

END_NAMESPACE_YM_YMSL

#endif // ASTLIST_H
