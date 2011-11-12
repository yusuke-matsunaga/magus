#ifndef RWTPATLIST_H
#define RWTPATLIST_H

/// @file RwtPatList.h
/// @brief RwtPatList のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/TvFunc.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class RwtPatList RwtPatList.h "RwtPatList.h"
/// @brief 同一の関数を表す RwtPat のリスト
/// と同時に関数を引数としたハッシュ表の要素でもある．
//////////////////////////////////////////////////////////////////////
class RwtPatList
{
  friend class RwtMgr;

public:

  /// @brief コンストラクタ
  RwtPatList();

  /// @brief デストラクタ
  ~RwtPatList();


public:

  /// @brief 対応する関数を得る．
  const TvFunc&
  func() const;

  /// @brief パタン数を得る．
  ymuint
  pat_num() const;

  /// @brief パタンを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
  const RwtPat&
  pat(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する論理関数
  TvFunc mFunc;

  // パタン数
  ymuint32 mPatNum;

  // パタンの配列
  RwtPat* mPatList;

  // ハッシュ表の次の要素を指すリンクポインタ
  RwtPatList* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
RwtPatList::RwtPatList()
{
}

// @brief デストラクタ
inline
RwtPatList::~RwtPatList()
{
}

// @brief 対応する関数を得る．
inline
const TvFunc&
RwtPatList::func() const
{
  return mFunc;
}

// @brief パタン数を得る．
inline
ymuint
RwtPatList::pat_num() const
{
  return mPatNum;
}

// @brief パタンを得る．
// @param[in] pos 位置番号 ( 0 <= pos < pat_num() )
inline
const RwtPat&
RwtPatList::pat(ymuint pos) const
{
  assert_cond( pos < mPatNum, __FILE__, __LINE__);
  return mPatList[pos];
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTPATLIST_H
