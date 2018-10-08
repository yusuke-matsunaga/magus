#ifndef CUTLIST_H
#define CUTLIST_H

/// @file CutList.h
/// @brief CutList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class CutListIterator Cut.h "Cut.h"
/// @brief Cut のリストをたどるための反復子
//////////////////////////////////////////////////////////////////////
class CutListIterator
{
  friend class CutList;

public:
  //////////////////////////////////////////////////////////////////////
  // 通常のコンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////


  /// @brief コンストラクタ
  ///
  /// 内容は不定
  CutListIterator();

  /// @brief デストラクタ
  ~CutListIterator();


private:
  //////////////////////////////////////////////////////////////////////
  // CutList が用いるコンストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  CutListIterator(Cut* cut);


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットを得る．
  const Cut*
  operator*() const;

  /// @brief 次の要素に移動する．
  void
  operator++();

  /// @brief 等価比較演算子
  friend
  bool
  operator==(CutListIterator a,
	     CutListIterator b);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のカット
  Cut* mCut;

};

/// @relates CutListIterator
/// @brief 非等価比較演算子
bool
operator!=(CutListIterator a,
	   CutListIterator b);


//////////////////////////////////////////////////////////////////////
/// @class CutList Cut.h "Cut.h"
/// @brief Cut のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class CutList
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  ///
  /// 空のリストとして初期化される．
  CutList();

  /// @brief デストラクタ
  ~CutList();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief カットを末尾に追加する．
  /// @param[in] cut 対象のカット
  void
  push_back(Cut* cut);

  /// @brief 先頭を表す反復子を返す．
  CutListIterator
  begin() const;

  /// @brief 末尾を表す反復子を返す．
  CutListIterator
  end() const;

  /// @brief 要素数を返す．
  int
  size() const;

  /// @brief 空のとき true を返す．
  bool
  empty() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cut* mTop;

  // 末尾の要素
  Cut* mTail;

  // 要素数
  int mNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CutListIterator::CutListIterator() :
  mCut(nullptr)
{
}

// @brief デストラクタ
inline
CutListIterator::~CutListIterator()
{
}

// @brief 内容を指定したコンストラクタ
inline
CutListIterator::CutListIterator(Cut* cut) :
  mCut(cut)
{
}

// @brief カットを得る．
inline
const Cut*
CutListIterator::operator*() const
{
  return mCut;
}

// @brief 次の要素に移動する．
inline
void
CutListIterator::operator++()
{
  if ( mCut ) {
    mCut = mCut->mLink;
  }
}

// @brief 等価比較演算子
inline
bool
operator==(CutListIterator a,
	   CutListIterator b)
{
  return a.mCut == b.mCut;
}

// @relates CutListIterator
// @brief 非等価比較演算子
inline
bool
operator!=(CutListIterator a,
	   CutListIterator b)
{
  return !operator==(a, b);
}

// @brief コンストラクタ
inline
CutList::CutList() :
  mTop(nullptr),
  mTail(nullptr),
  mNum(0)
{
}

// @brief デストラクタ
inline
CutList::~CutList()
{
}

// @brief 内容をクリアする．
inline
void
CutList::clear()
{
  mTop = nullptr;
  mTail = nullptr;
  mNum = 0;
}

// @brief カットを末尾に追加する．
inline
void
CutList::push_back(Cut* cut)
{
  if ( mTail ) {
    mTail->mLink = cut;
    mTail = cut;
  }
  else {
    mTop = mTail = cut;
  }
  ++ mNum;
  // 念のため
  cut->mLink = nullptr;
}

// @brief 先頭を表す反復子を返す．
inline
CutListIterator
CutList::begin() const
{
  return CutListIterator(mTop);
}

// @brief 末尾を表す反復子を返す．
inline
CutListIterator
CutList::end() const
{
  return CutListIterator(nullptr);
}

// @brief 要素数を返す．
inline
int
CutList::size() const
{
  return mNum;
}

// @brief 空のとき true を返す．
inline
bool
CutList::empty() const
{
  return mTop == nullptr;
}

END_NAMESPACE_LUTMAP

#endif // CUTLIST_H
