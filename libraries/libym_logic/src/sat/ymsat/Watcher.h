#ifndef WATCHER_H
#define WATCHER_H

/// @file Watcher.h
/// @brief Watcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/sat_nsdef.h"
#include "SatReason.h"
#include "YmUtils/Alloc.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class Watcher ImplInk.h "Watcher.h"
/// @brief watch literal に関連付けられた節のリストを表すクラス
///
/// (a1 + a2 + a3 + ... an) という節があるとする．
/// この時， a1 と a2 が watch literal ならば，~a1 または ~a2
/// の割り当てが起こったときに，この節の watch literal の更新を行う
/// 必要がある．
/// そのような節のリストを作るためのクラス
//////////////////////////////////////////////////////////////////////
class Watcher :
  public SatReason
{
public:

  /// @brief コンストラクタ
  Watcher();

  /// @brief コンストラクタ
  /// @param[in] src もととなる SatReason
  explicit
  Watcher(SatReason src);

};


//////////////////////////////////////////////////////////////////////
/// @class WatcherList Watcher.h "Watcher.h"
/// @brief Watcher のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class WatcherList
{
public:

  /// @brief コンストラクタ
  WatcherList();

  /// @brief デストラクタ
  ~WatcherList();


public:

  /// @brief クリアする．
  void
  clear();

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief 要素を追加する．
  /// @param[in] elem 追加する要素
  void
  add(Watcher elem,
      Alloc& alloc);

  /// @brief pos 番目の要素を返す．
  /// @param[in] pos 位置
  Watcher
  elem(ymuint pos) const;

  /// @brief pos 番目の要素を設定する．
  /// @param[in] pos 位置
  /// @param[in] elem 要素
  void
  set_elem(ymuint pos,
	   Watcher elem);

  /// @brief 要素を切り詰める．
  /// @param[in] num 切り詰めるサイズ
  void
  erase(ymuint num);

  /// @brief from の内容を移す．
  /// @param[in] from もとのリスト
  /// @note from は空になる．
  void
  move(WatcherList& from);

  /// @brief このオブジェクトが使っているメモリを回収する．
  /// @note かなり危険な関数．
  /// @note この関数を呼んだらもうこのオブジェクトは使えない．
  void
  finish(Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 配列を拡張する．
  /// @param[in] req_size 拡張するサイズ
  void
  expand(ymuint req_size,
	 Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 配列のサイズ
  ymuint32 mSize;

  // 要素数
  ymuint32 mNum;

  // 配列
  Watcher* mArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
Watcher::Watcher()
{
}

// @brief コンストラクタ
// @param[in] src もととなる SatReason
inline
Watcher::Watcher(SatReason src) :
  SatReason(src)
{
}

// @brief コンストラクタ
inline
WatcherList::WatcherList() :
  mSize(0),
  mNum(0),
  mArray(NULL)
{
}

// @brief デストラクタ
inline
WatcherList::~WatcherList()
{
}

// @brief クリアする．
inline
void
WatcherList::clear()
{
  mNum = 0;
}

// @brief 要素を追加する．
inline
void
WatcherList::add(Watcher elem,
		 Alloc& alloc)
{
  if ( mNum >= mSize ) {
    expand(mNum + 1, alloc);
  }
  set_elem(mNum, elem);
  ++ mNum;
}

// @brief 要素数を返す．
inline
ymuint
WatcherList::num() const
{
  return mNum;
}

// @brief pos 番目の要素を返す．
inline
Watcher
WatcherList::elem(ymuint pos) const
{
  return mArray[pos];
}

// @brief pos 番目の要素を設定する．
inline
void
WatcherList::set_elem(ymuint pos,
		      Watcher elem)
{
  mArray[pos] = elem;
}

// @brief 要素を切り詰める．
inline
void
WatcherList::erase(ymuint num)
{
  mNum = num;
}

// @brief from の内容を移す．
// @note from は空になる．
inline
void
WatcherList::move(WatcherList& from)
{
  mSize = from.mSize;
  mNum = from.mNum;
  mArray = from.mArray;
  from.mSize = 0;
  from.mNum = 0;
  from.mArray = NULL;
}

// @brief 配列を拡張する．
inline
void
WatcherList::expand(ymuint req_size,
		    Alloc& alloc)
{
  if ( mSize < req_size ) {
    ymuint old_size = mSize;
    Watcher* old_array = mArray;
    if ( mSize == 0 ) {
      mSize = 4;
    }
    while ( mSize < req_size ) {
      mSize <<= 1;
    }
    void* p = alloc.get_memory(sizeof(Watcher) * mSize);
    mArray = new (p) Watcher[mSize];

    if ( old_size > 0 ) {
      for (ymuint i = 0; i < old_size; ++ i) {
	mArray[i] = old_array[i];
      }
      alloc.put_memory(sizeof(Watcher) * old_size, old_array);
    }
  }
}

// @brief このオブジェクトが使っているメモリを回収する．
// @note かなり危険な関数．
// @note この関数を呼んだらもうこのオブジェクトは使えない．
inline
void
WatcherList::finish(Alloc& alloc)
{
  if ( mSize > 0 ) {
    alloc.put_memory(sizeof(Watcher) * mSize, mArray);
  }
}

END_NAMESPACE_YM_SAT

#endif // SATREASON_H
