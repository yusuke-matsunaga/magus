#ifndef PATMATCHER_H
#define PATMATCHER_H

/// @file PatMatcher.h
/// @brief PatMatcher のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "cellmap_nsdef.h"
#include "Cut.h"
#include "ym/clib.h"


BEGIN_NAMESPACE_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class PatMatcher PatMatcher.h "PatMatcher.h"
/// @brief パタンマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class PatMatcher
{
public:

  /// @brief コンストラクタ
  /// @param[in] library セルライブラリ
  PatMatcher(const ClibCellLibrary& library);

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
  operator()(const SbjNode* sbj_root,
	     const ClibPatGraph& pat_graph,
	     Cut& match);

#if 0
  /// @brief 直前のマッチングにおける入力のノードを得る．
  /// @param[in] pos 入力番号
  const SbjNode*
  leaf_node(SizeType pos) const;

  /// @brief 直前のマッチングにおける入力の極性を得る．
  /// @param[in] pos 入力番号
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  leaf_inv(SizeType pos) const;
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
  bind(const SbjNode* sbj_node,
       SizeType pat_id,
       bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  const ClibCellLibrary& mLibrary;

  // パタンノードの ID をキーとしてサブジェクトノードを入れる配列
  vector<const SbjNode*> mSbjMap;

  // サブジェクトノードの ID をキーとしてパタンノードの ID を
  // 入れる配列
  unordered_map<SizeType, SizeType> mPatMap;

  // パタンノードの ID をキーとして極性を入れる配列
  vector<bool> mInvMap;

  // mSbjMap と mInvMap のクリア用キュー
  vector<SizeType> mClearQueue;

#if 0
  // 直前のマッチングにおけるパタンの入力ノードを記録する配列
  vector<const SbjNode*> mLeafNodeArray;

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
const SbjNode*
PatMatcher::leaf_node(SizeType pos) const
{
  return mLeafNodeArray[pos];
}

// @brief 直前のマッチングにおける入力の極性を得る．
// @param[in] pos 入力番号
// @retval true 反転あり
// @retval false 反転なし
inline
bool
PatMatcher::leaf_inv(SizeType pos) const
{
  return mLeafInvArray[pos];
}
#endif

END_NAMESPACE_CELLMAP

#endif // PATMATCHER_H
