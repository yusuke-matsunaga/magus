#ifndef LIBYM_TECHMAP_MATCH_H
#define LIBYM_TECHMAP_MATCH_H

/// @file libym_techmap/Match.h
/// @brief Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class Match Match.h "Match.h"
/// @brief サブジェクトグラフとパタングラフのマッチを表すクラス
//////////////////////////////////////////////////////////////////////
class Match
{
public:

  /// @brief コンストラクタ
  Match();

  /// @brief デストラクタ
  ~Match();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードと入力数を設定する．
  /// @param[in] root_node 根のノード
  /// @param[in] root_inv 根の極性
  /// @param[in] ni 入力数
  void
  set_root(const SbjNode* root_node,
	   bool root_inv,
	   ymuint ni);

  /// @brief 葉のノードを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  /// @param[in] leaf_node 葉のノード
  /// @param[in] leaf_inv 葉の極性
  void
  set_leaf(ymuint pos,
	   const SbjNode* leaf_node,
	   bool leaf_inv);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のノードを得る．
  const SbjNode*
  root_node() const;

  /// @brief 根の極性を得る．
  bool
  root_inv() const;

  /// @brief 葉の数を得る．
  ymuint
  leaf_num() const;

  /// @brief 葉のノードを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  const SbjNode*
  leaf_node(ymuint pos) const;

  /// @brief 葉の極性を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
  bool
  leaf_inv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const SbjNode* mRoot;

  // 葉のノードの配列
  vector<const SbjNode*> mLeafArray;

  // 根と葉の極性をパックしたもの
  // 0 〜 (ni - 1) ビットが葉の極性
  // ni ビットが根の極性
  // ただし ni は葉の数
  ymuint32 mInvArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
Match::Match()
{
}

// @brief デストラクタ
inline
Match::~Match()
{
}

// @brief 根のノードと入力数を設定する．
// @param[in] root_node 根のノード
// @param[in] root_inv 根の極性
// @param[in] ni 入力数
inline
void
Match::set_root(const SbjNode* root_node,
		bool root_inv,
		ymuint ni)
{
  mRoot = root_node;
  mLeafArray.clear();
  mLeafArray.resize(ni);
  mInvArray = (static_cast<ymuint>(root_inv) << ni);
}

// @brief 葉のノードを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
// @param[in] leaf_node 葉のノード
// @param[in] leaf_inv 葉の極性
inline
void
Match::set_leaf(ymuint pos,
		const SbjNode* leaf_node,
		bool leaf_inv)
{
  mLeafArray[pos] = leaf_node;
  mInvArray |= (static_cast<ymuint>(leaf_inv) << pos);
}

// @brief 根のノードを得る．
inline
const SbjNode*
Match::root_node() const
{
  return mRoot;
}

// @brief 根の極性を得る．
inline
bool
Match::root_inv() const
{
  return static_cast<bool>((mInvArray >> leaf_num()) & 1U);
}

// @brief 葉の数を得る．
inline
ymuint
Match::leaf_num() const
{
  return mLeafArray.size();
}

// @brief 葉のノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
inline
const SbjNode*
Match::leaf_node(ymuint pos) const
{
  return mLeafArray[pos];
}

// @brief 葉の極性を得る．
// @param[in] pos 位置番号 ( 0 <= pos < leaf_num() )
bool
Match::leaf_inv(ymuint pos) const
{
  return static_cast<bool>((mInvArray >> pos) & 1U);
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_MATCH_H
