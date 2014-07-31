#ifndef PATMATCHER_H
#define PATMATCHER_H

/// @file PatMatcher.h
/// @brief PatMatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "YmNetworks/bdn.h"
#include "YmCell/cell_nsdef.h"
#include "Match.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class PatMatcher PatMatcher.h "PatMatcher.h"
/// @brief パタンマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class PatMatcher
{
public:

  /// @brief コンストラクタ
  /// @param[in] library セルライブラリ
  PatMatcher(const CellLibrary& library);

  /// @brief デストラクタ
  ~PatMatcher();


public:

  /// @brief パタンマッチングを行う．
  /// @param[in] sbj_root サブジェクトグラフの根のノード
  /// @param[in] pat_graph パタングラフ
  /// @param[in] match マッチング結果を格納する変数
  /// @retval true マッチングが成功した
  /// @retval false マッチングが失敗した．
  bool
  operator()(const BdnNode* sbj_root,
	     const CellPatGraph& pat_graph,
	     Match& match);

#if 0
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
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

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
  unordered_map<ymuint, ymuint> mPatMap;

  // パタンノードの ID をキーとして極性を入れる配列
  vector<bool> mInvMap;

  // mSbjMap と mInvMap のクリア用キュー
  vector<ymuint> mClearQueue;

#if 0
  // 直前のマッチングにおけるパタンの入力ノードを記録する配列
  vector<const BdnNode*> mLeafNodeArray;

  // 直前のマッチングにおけるパタンの入力の極性を記録する配列
  vector<bool> mLeafInvArray;
#endif

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

#if 0
// @brief 直前のマッチングにおける入力のノードを得る．
// @param[in] pos 入力番号
inline
const BdnNode*
PatMatcher::leaf_node(ymuint pos) const
{
  return mLeafNodeArray[pos];
}

// @brief 直前のマッチングにおける入力の極性を得る．
// @param[in] pos 入力番号
// @retval true 反転あり
// @retval false 反転なし
inline
bool
PatMatcher::leaf_inv(ymuint pos) const
{
  return mLeafInvArray[pos];
}
#endif

END_NAMESPACE_YM_CELLMAP

#endif // PATMATCHER_H
