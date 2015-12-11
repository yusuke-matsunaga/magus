#ifndef MAGUS_CELLMAP_PAT2MATCHER_H
#define MAGUS_CELLMAP_PAT2MATCHER_H

/// @file Pat2Matcher.h
/// @brief Pat2Matcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "YmNetworks/bdn.h"
#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class Pat2Matcher Pat2Matcher.h "Pat2Matcher.h"
/// @brief パタンマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class Pat2Matcher
{
public:

  /// @brief コンストラクタ
  /// @param[in] library セルライブラリ
  Pat2Matcher(const CellLibrary& library);

  /// @brief デストラクタ
  ~Pat2Matcher();


public:

  /// @brief パタンマッチングを行う．
  /// @param[in] sbj_root サブジェクトグラフの根のノード
  /// @param[in] pat_graph パタングラフ
  /// @retval true マッチした．
  /// @retval false マッチしなかった．
  bool
  operator()(const BdnNode* sbj_root,
	     const CellPat2Graph& pat_graph);

  /// @brief 直前のマッチングにおける入力のノードを得る．
  /// @param[in] pos 入力番号
  const BdnNode*
  leaf_node(ymuint pos) const;

  /// @brief 直前のマッチングにおける入力の極性を得る．
  /// @param[in] pos 入力番号
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  leaf_inv(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マッチングを行なう下請け関数
  /// @param[in] pat_graph パタングラフ
  /// @param[in] pos 枝番号
  bool
  match_sub(const CellPat2Graph& pat_graph,
	    ymuint pos);

  /// @brief サブジェクトノードとパタンノードをバインドする．
  /// @param[in] sbj_node サブジェクトノード
  /// @param[in] pat_id パタンノードのID
  /// @param[in] inv 反転フラグ
  /// @retval true バインドが成功した．
  /// @retval false バインドが失敗した．
  bool
  bind(const BdnNode* sbj_node,
       ymuint pat_id,
       bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  const CellLibrary& mLibrary;

  // パタンノードの ID をキーとしてサブジェクトノードを入れる配列
  vector<const BdnNode*> mSbjMap;

  // サブジェクトノードの ID をキーとしてパタンノードの ID を
  // 入れる配列
  hash_map<ymuint, ymuint> mPatMap;

  // パタンノードの ID をキーとして極性を入れる配列
  vector<bool> mInvMap;

  // mSbjMap と mInvMap のクリア用キュー
  vector<ymuint> mClearQueue;

  // 直前のマッチングにおけるパタンの入力ノードを記録する配列
  vector<const BdnNode*> mLeafNodeArray;

  // 直前のマッチングにおけるパタンの入力の極性を記録する配列
  vector<bool> mLeafInvArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前のマッチングにおける入力のノードを得る．
// @param[in] pos 入力番号
inline
const BdnNode*
Pat2Matcher::leaf_node(ymuint pos) const
{
  return mLeafNodeArray[pos];
}

// @brief 直前のマッチングにおける入力の極性を得る．
// @param[in] pos 入力番号
// @retval true 反転あり
// @retval false 反転なし
inline
bool
Pat2Matcher::leaf_inv(ymuint pos) const
{
  return mLeafInvArray[pos];
}

END_NAMESPACE_YM_CELLMAP

#endif // MAGUS_CELLMAP_PAT2MATCHER_H
