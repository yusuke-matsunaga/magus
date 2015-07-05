﻿#ifndef LIBYM_TECHMAP_CELLMAP_MATCH_H
#define LIBYM_TECHMAP_CELLMAP_MATCH_H

/// @file libym_techmap/cellmap/Match.h
/// @brief Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "YmNetworks/bdn.h"
#include "YmLogic/npn_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class Match Match.h "Match.h"
/// @brief サブジェクトグラフとパタングラフのマッチを表すクラス
//////////////////////////////////////////////////////////////////////
class Match
{
public:

  /// @brief コンストラクタ
  /// @param[in] nl 葉の数(入力数)
  Match(ymuint nl = 0);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Match(const Match& src);

  /// @brief デストラクタ
  ~Match();

  /// @brief 代入演算子
  const Match&
  operator=(const Match& src);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 葉の数を再設定する．
  void
  resize(ymuint nl);

  /// @brief 葉のノードを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  /// @param[in] leaf_node 葉のノード
  /// @param[in] leaf_inv 葉の極性
  void
  set_leaf(ymuint pos,
	   const BdnNode* leaf_node,
	   bool leaf_inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 葉の数を得る．
  ymuint
  leaf_num() const;

  /// @brief 葉のノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  const BdnNode*
  leaf_node(ymuint pos) const;

  /// @brief 葉の極性を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  bool
  leaf_inv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 葉の数
  ymuint32 mLeafNum;

  // 葉のノードの配列
  const BdnNode** mLeafArray;

  // 根と葉の極性をパックしたもの
  ymuint32 mInvArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 葉の数を再設定する．
inline
void
Match::resize(ymuint nl)
{
  if ( mLeafNum != nl ) {
    delete [] mLeafArray;
    mLeafNum = nl;
    mLeafArray = new const BdnNode*[nl];
  }
  mInvArray = 0U;
}

// @brief 葉のノードを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
// @param[in] leaf_node 葉のノード
// @param[in] leaf_inv 葉の極性
inline
void
Match::set_leaf(ymuint pos,
		const BdnNode* leaf_node,
		bool leaf_inv)
{
  ASSERT_COND( pos < leaf_num() );
  mLeafArray[pos] = leaf_node;
  mInvArray |= (static_cast<ymuint>(leaf_inv) << pos);
}

// @brief 葉の数を得る．
inline
ymuint
Match::leaf_num() const
{
  return mLeafNum;
}

// @brief 葉のノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
inline
const BdnNode*
Match::leaf_node(ymuint pos) const
{
  return mLeafArray[pos];
}

// @brief 葉の極性を得る．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
inline
bool
Match::leaf_inv(ymuint pos) const
{
  return static_cast<bool>((mInvArray >> pos) & 1U);
}

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_MATCH_H
