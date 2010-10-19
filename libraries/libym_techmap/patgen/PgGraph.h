#ifndef LIBYM_TECHMAP_PATGEN_PGGRAPH_H
#define LIBYM_TECHMAP_PATGEN_PGGRAPH_H

/// @file libym_techmap/patgen/PgNode.h
/// @brief PgGraph のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgNodeMgr;

//////////////////////////////////////////////////////////////////////
/// @class PgGraph PgNode.h "PgNode.h"
/// @brief パタングラフを表すクラス
/// といっても PatGraph とは別物
//////////////////////////////////////////////////////////////////////
class PgGraph
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr PgNodeMgr
  PgGraph(PgNodeMgr& mgr);

  /// @brief コピーコンストラクタ
  PgGraph(const PgGraph& src);

  /// @brief デストラクタ
  ~PgGraph();


public:
  //////////////////////////////////////////////////////////////////////
  // 取得用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  PgNode*
  node(ymuint id) const;

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを得る．
  /// @param[in] id 入力番号 ( 0 <= id < input_num() )
  PgNode*
  input(ymuint id) const;

  /// @brief 根のノードを得る．
  PgNode*
  root() const;

  /// @brief 根のノードの反転属性を得る．
  bool
  root_inv() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを作る．
  PgNode*
  new_input();

  /// @brief AND ノードを作る．
  /// @param[in] fanin0, fanin1 ファンインのノード
  /// @param[in] inv0, inv1 ファンインの反転属性
  PgNode*
  new_and(PgNode* fanin0,
	  PgNode* fanin1,
	  bool inv0,
	  bool inv1);

  /// @brief XOR ノードを作る．
  /// @param[in] fanin0, fanin1 ファンインのノード
  PgNode*
  new_xor(PgNode* fanin0,
	  PgNode* fanin1);

  /// @brief 根のノードを設定する．
  /// @param[in] node 根のノード
  /// @param[in] inv 反転属性
  void
  set_root(PgNode* node,
	   bool inv);

  /// @brief 内部のノードのファンインを入れ替える．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  void
  swap_fanin(ymuint id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // PgNode を管理するクラス
  PgNodeMgr& mMgr;

  // 入力ノードのリスト
  vector<PgNode*> mInputList;

  // 根のノード
  PgNode* mRoot;

  // 根のノードの反転属性
  bool mRootInv;

  // 全ノードのリスト
  list<PgNode*> mNodeList;

};


//////////////////////////////////////////////////////////////////////
// 関連する関数
//////////////////////////////////////////////////////////////////////

/// @brief 内部のノードのファンインを入れ替えたグラフを作る．
PgGraph



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード数を得る．
inline
ymuint
PgGraph::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを得る．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
PgNode*
PgGraph::node(ymuint id) const
{
  return mNodeList[id];
}

// @brief 入力数を得る．
inline
ymuint
PgGraph::input_num() const
{
  return mInputList.size();
}

// @brief 入力ノードを得る．
// @param[in] id 入力番号 ( 0 <= id < input_num() )
inline
PgNode*
PgGraph::input(ymuint id) const
{
  return mInputList[id];
}

// @brief 根のノードを得る．
inline
PgNode*
PgGraph::root() const
{
  return mRoot;
}

// @brief 根のノードの反転属性を得る．
inline
bool
PgGraph::root_inv() const
{
  return mRootInv;
}

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGGRAPH_H
