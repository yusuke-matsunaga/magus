#ifndef IMPVALLIST_H
#define IMPVALLIST_H

/// @file ImpValList.h
/// @brief ImpValList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ImpVal.h"
#include "ym_utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpValListIter;

//////////////////////////////////////////////////////////////////////
/// @class ImpValList ImpValList.h "ImpValList.h"
/// @brief ImpVal のリストを表すクラス
//////////////////////////////////////////////////////////////////////
class ImpValList
{
  friend class ImpValListIter;
public:

  /// @brief 空のコンストラクタ
  ImpValList();

  /// @brief デストラクタ
  ~ImpValList();


public:

  /// @brief 要素数を得る．
  ymuint
  num() const;

  /// @brief 要素のリストを追加する．
  void
  insert(const vector<ImpVal>& val_list);

  /// @brief リストの内容をマージする．
  void
  merge(const ImpValList& src);

  /// @brief 2つのリストの共通部分をマージする．
  void
  cap_merge(const ImpValList& src1,
	    const ImpValList& src2);

  /// @brief 先頭を表す反復子を返す．
  ImpValListIter
  begin() const;

  /// @brief 末尾を表す反復子を返す．
  ImpValListIter
  end() const;

  /// @brief 増分を得る．
  ymuint
  delta() const;

  /// @brief 増分のカウントをリセットする．
  void
  reset_delta();

  /// @brief 内容を出力する
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    Cell() : mLink(NULL) { }

    // 値
    ImpVal mVal;
    // 次の要素を指すポインタ
    Cell* mLink;
  };

  /// @brief Cell を確保する関数
  Cell*
  get_cell();

  /// @brief Cell を開放する関数
  void
  put_cell(Cell* cell);

  /// @brief 結果が整列されているかと mNum が正しいかのテスト
  void
  sanity_check() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // 先頭を表すダミー
  Cell mDummyTop;

  // 増分計算用の変数
  ymuint32 mDeltaBase;

  // Cell のメモリ確保用オブジェクト
  static
  UnitAlloc mAlloc;

};


//////////////////////////////////////////////////////////////////////
/// @class ImpValListIter ImpValList.h "ImpValList.h"
/// @brief ImpValList の反復子
//////////////////////////////////////////////////////////////////////
class ImpValListIter
{
public:

  /// @brief コンストラクタ
  ImpValListIter();

  /// @brief Cell を指定したコンストラクタ
  ImpValListIter(ImpValList::Cell* cell);


public:

  /// @brief 内容を取り出す．
  ImpVal
  operator*() const;

  /// @brief 次に進める．
  const ImpValListIter&
  operator++();

  /// @brief 等価比較演算子
  bool
  operator==(const ImpValListIter& right) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const ImpValListIter& right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ImpValList::Cell* mCell;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 増分を得る．
inline
ymuint
ImpValList::delta() const
{
  return mNum - mDeltaBase;
}

// @brief 増分のカウントをリセットする．
inline
void
ImpValList::reset_delta()
{
  mDeltaBase = mNum;
}

// @brief コンストラクタ
inline
ImpValListIter::ImpValListIter() :
  mCell(NULL)
{
}

// @brief Cell を指定したコンストラクタ
inline
ImpValListIter::ImpValListIter(ImpValList::Cell* cell) :
  mCell(cell)
{
}

// @brief 内容を取り出す．
inline
ImpVal
ImpValListIter::operator*() const
{
  if ( mCell ) {
    return mCell->mVal;
  }
  else {
    return ImpVal(0, 0);
  }
}

// @brief 次に進める．
inline
const ImpValListIter&
ImpValListIter::operator++()
{
  if ( mCell ) {
    mCell = mCell->mLink;
  }
  return *this;
}

// @brief 等価比較演算子
inline
bool
ImpValListIter::operator==(const ImpValListIter& right) const
{
  return mCell == right.mCell;
}

// @brief 非等価比較演算子
inline
bool
ImpValListIter::operator!=(const ImpValListIter& right) const
{
  return !operator==(right);
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPVALLIST_H
