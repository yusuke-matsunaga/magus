#ifndef CUTLIST_H
#define CUTLIST_H

/// @file CutList.h
/// @brief CutList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


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
  CutListIterator() = default;

  /// @brief デストラクタ
  ~CutListIterator() = default;


private:
  //////////////////////////////////////////////////////////////////////
  // CutList が用いるコンストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  CutListIterator(
    Cut* cut
  ) : mCut{cut}
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットを得る．
  const Cut*
  operator*() const
  {
    return mCut;
  }

  /// @brief 次の要素に移動する．
  void
  operator++()
  {
    if ( mCut ) {
      mCut = mCut->mLink;
    }
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const CutListIterator b
  ) const
  {
    return mCut == b.mCut;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const CutListIterator b
  ) const
  {
    return !operator==(b);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のカット
  Cut* mCut{nullptr};

};


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
  CutList() = default;

  /// @brief デストラクタ
  ~CutList() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear()
  {
    mTop = nullptr;
    mTail = nullptr;
    mNum = 0;
  }

  /// @brief カットを末尾に追加する．
  void
  push_back(
    Cut* cut ///< [in] 対象のカット
  )
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

  /// @brief 先頭を表す反復子を返す．
  CutListIterator
  begin() const
  {
    return CutListIterator{mTop};
  }

  /// @brief 末尾を表す反復子を返す．
  CutListIterator
  end() const
  {
    return CutListIterator{nullptr};
  }

  /// @brief 要素数を返す．
  SizeType
  size() const
  {
    return mNum;
  }

  /// @brief 空のとき true を返す．
  bool
  empty() const
  {
    return mTop == nullptr;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の要素
  Cut* mTop{nullptr};

  // 末尾の要素
  Cut* mTail{nullptr};

  // 要素数
  SizeType mNum{0};

};

END_NAMESPACE_LUTMAP

#endif // CUTLIST_H
