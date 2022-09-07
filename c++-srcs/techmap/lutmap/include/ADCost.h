#ifndef MAGUS_LUTMAP_ADCOST_H
#define MAGUS_LUTMAP_ADCOST_H

/// @file lutmap/ADCost.h
/// @brief ADCost のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;

template <typename AreaT>
class ADCostList;

template <typename AreaT>
class ADCostIterator;

template <typename AreaT>
class ADCostMgr;

//////////////////////////////////////////////////////////////////////
/// @class ADCost ADCost.h "ADCost.h"
/// @brief 面積と段数のコストを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCost
{
  friend class ADCostList<AreaT>;
  friend class ADCostIterator<AreaT>;
  friend class ADCostMgr<AreaT>;

public:

  /// @brief 空のコンストラクタ
  ADCost() = default;

  /// @brief デストラクタ
  ~ADCost() = default;


public:

  /// @brief カットを取り出す．
  const Cut*
  cut() const
  {
    return mCut;
  }

  /// @brief 段数を取り出す．
  int
  depth() const
  {
    return mDepth;
  }

  /// @brief 面積を取り出す．
  AreaT
  area() const
  {
    return mArea;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カット
  const Cut* mCut{nullptr};

  // 深さ
  int mDepth{0};

  // 面積
  AreaT mArea;

  // 次の要素を指すリンク
  ADCost* mLink{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class ADCostIterator ADCost.h "ADCost.h"
/// @brief ADCostList 用の反復子
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCostIterator
{
  friend class ADCostList<AreaT>;

public:

  /// @brief 空のコンストラクタ
  ADCostIterator() = default;

  /// @brief コピーコンストラクタ
  ADCostIterator(
    const ADCostIterator& src ///< [in] コピー元のオブジェクト
  ) : mPtr{src.mPtr}
  {
  }

  /// @brief 要素を取り出す．
  ADCost<AreaT>*
  operator*() const
  {
    return mPtr;
  }

  /// @brief 次の要素に移動する．
  void
  operator++()
  {
    if ( mPtr ) {
      mPtr = mPtr->mLink;
    }
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const ADCostIterator& right
  ) const
  {
    return mPtr == right.mPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const ADCostIterator& right
  ) const
  {
    return !operator==(right);
  }


private:

  /// @brief ポインタを指定したコンストラクタ
  ADCostIterator(
    ADCost<AreaT>* ptr ///< [in] 要素を指すポインタ
  ) : mPtr{ptr}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の要素へのポインタ
  ADCost<AreaT>* mPtr{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class ADCostMgr ADCost.h "ADCost.h"
/// @brief ADCost の確保/解放を行うクラス
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCostMgr
{
public:

  /// @brief コンストラクタ
  ADCostMgr(
  ) : mTopPage(nullptr),
      mNextPos(0),
      mAvail(nullptr)
  {
  }

  /// @brief デストラクタ
  ///
  /// このオブジェクトが確保したすべてのメモリを開放する．
  ~ADCostMgr()
  {
    for ( auto page = mTopPage; page; ) {
      auto p = page;
      page = p->mLink;
      delete p;
    }
  }


public:

  /// @brief ADCost を確保する．
  /// @return 確保したオブジェクトを返す．
  ADCost<AreaT>*
  alloc_cost()
  {
    if ( mAvail ) {
      auto cost = mAvail;
      mAvail = cost->mLink;
      cost->mLink = nullptr;
      return cost;
    }
    if ( mTopPage == nullptr || mNextPos >= kChunkSize ) {
      auto page = new Page;
      page->mLink = mTopPage;
      mTopPage = page;
      mNextPos = 0;
    }
    auto cost = &mTopPage->mChunk[mNextPos];
    ++ mNextPos;
    return cost;
  }

  /// @brief ADCost を再利用リストにもどす
  void
  delete_cost(
    ADCost<AreaT>* cost ///< [in] 戻すオブジェクト
  )
  {
    cost->mLink = mAvail;
    mAvail = cost;
  }

  /// @brief 確保したページ数を返す．
  SizeType
  alloc_num() const
  {
    SizeType n = 0;
    for ( auto page = mTopPage; page; page = page->mLink ) {
      ++ n;
    }
    return n;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数とデータ構造
  //////////////////////////////////////////////////////////////////////

  // 1ページのサイズ
  static
  const SizeType kChunkSize = 1024;

  // 1ページの構造体
  struct Page
  {
    // コスト本体の配列
    ADCost<AreaT> mChunk[kChunkSize];

    // 次のページを指すリンク
    Page* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のページ
  Page* mTopPage;

  // 次に使用可能な位置
  SizeType mNextPos;

  // 再利用リスト
  ADCost<AreaT>* mAvail;

};


//////////////////////////////////////////////////////////////////////
/// @class ADCostList ADCost.h "ADCost.h"
/// @brief ADCost のリストを表すクラス
//////////////////////////////////////////////////////////////////////
template <typename AreaT>
class ADCostList
{
public:

  /// @brief コンストラクタ
  ///
  /// 空のリストで初期化される．
  ADCostList() = default;

  /// @brief デストラクタ
  ~ADCostList() = default;

  /// @brief ADCostMgr を設定する．
  void
  set_mgr(
    ADCostMgr<AreaT>* mgr ///< [in] ADCost のメモリ管理を行うオブジェクト
  )
  {
    mMgr = mgr;
  }


public:

  /// @brief 先頭の反復子を取り出す．
  ADCostIterator<AreaT>
  begin()
  {
    return ADCostIterator<AreaT>(mTop.mLink);
  }

  /// @brief 末尾の反復子を取り出す．
  ADCostIterator<AreaT>
  end()
  {
    return ADCostIterator<AreaT>(nullptr);
  }

  /// @brief 要素を追加する．
  ///
  /// 適当な位置に挿入される．
  /// ただし，深さと面積の両方で他の要素より悪い場合には追加されない．
  /// 逆にこの要素によって無効化される要素があればそれは削除される．
  void
  insert(
    const Cut* cut, ///< [in] カット
    int depth,      ///< [in] 深さ
    AreaT area      ///< [in] 面積コスト
  )
  {
    ADCost<AreaT>* prev = &mTop;
    ADCost<AreaT>* cur;
    while ( (cur = prev->mLink) ) {
      if ( cur->mDepth > depth ) {
	if ( cur->mArea >= area ) {
	  // cur は不要
	  ADCost<AreaT>* next = cur->mLink;
	  cur->mLink = nullptr;
	  prev->mLink = next;
	  mMgr->delete_cost(cur);
	}
	else {
	  prev = cur;
	}
      }
      else if ( cur->mDepth == depth ) {
	if ( cur->mArea <= area ) {
	  // (depth, area) は不要
	  break;
	}
	else {
	  // cur は不要 (というか上書きする)
	  cur->mCut = cut;
	  cur->mArea = area;
	  break;
	}
      }
      else { // cur->mDepth < depth
	if ( cur->mArea > area ) {
	  // (depth, area) をここに追加
	  ADCost<AreaT>* new_cost = mMgr->alloc_cost();
	  new_cost->mCut = cut;
	  new_cost->mDepth = depth;
	  new_cost->mArea = area;
	  prev->mLink = new_cost;
	  new_cost->mLink = cur;
	  break;
	}
	else { // cur->mArea <= area
	  // (depth, area) は不要
	  break;
	}
      }
    }
    if ( cur == nullptr ) {
      cur = mMgr->alloc_cost();
      cur->mCut = cut;
      cur->mDepth = depth;
      cur->mArea = area;
      prev->mLink = cur;
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭の前のダミー要素
  ADCost<AreaT> mTop;

  // メモリ管理オブジェクト
  ADCostMgr<AreaT>* mMgr;

};

END_NAMESPACE_LUTMAP

#endif // MAGUS_LUTMAP_ADCOST_H
