#ifndef TOPDOWN2_H
#define TOPDOWN2_H

/// @file TopDown2.h
/// @brief TopDown2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut2.h"
#include "networks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TopDown2 TopDown2.h "TopDown2.h"
/// @brief トップダウンのカット列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class TopDown2 :
  public EnumCut2
{
public:

  /// @brief コンストラクタ
  TopDown2();

  /// @brief デストラクタ
  virtual
  ~TopDown2();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(BdnMgr& network,
	     ymuint limit,
	     EnumCutOp2* op);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  class NodeTemp
  {
  public:

    /// @brief コンストラクタ
    NodeTemp();

    /// @brief 状態を取り出す．
    /// - 0: 初期状態
    /// - 1: カットの内側
    /// - 2: カットの入力(境界)
    ymuint
    state() const;

    /// @brief 内部の印をつける．
    void
    set_inside_state();

    /// @brief 入力の印をつける．
    void
    set_input_state();

    /// @brief 状態を初期化する．
    void
    clear_state();

    /// @brief footprint マークを返す．
    bool
    fpmark() const;

    /// @brief footprint マークをつける．
    void
    set_fpmark();

    /// @brief footprint マークを消す．
    void
    clear_fpmark();

    /// @brief テンポラリマーク1を取り出す．
    bool
    temp1mark() const;

    /// @brief テンポラリマーク1をつける．
    void
    set_temp1mark();

    /// @brief テンポラリマーク2を取り出す．
    bool
    temp2mark() const;

    /// @brief テンポラリマーク2をつける．
    void
    set_temp2mark();

    /// @brief テンポラリマーク1/2 を消す．
    void
    clear_tempmark();

    /// @brief 枝のマークを取り出す．
    /// @param[in] pos 枝番号 ( 0 or 1 )
    bool
    edge_mark(ymuint pos) const;

    /// @brief 枝のマークをつける．
    /// @param[in] pos 枝番号 ( 0 or 1 )
    void
    set_edge_mark(ymuint pos);

    /// @brief 枝のマークを消す．
    /// @param[in] pos 枝番号 ( 0 or 1 )
    void
    clear_edge_mark(ymuint pos);

    /// @brief footprint ノードのリストを返す．
    vector<BdnNode*>&
    fpnode_list();

    /// @brief footprint ノードのリストを返す．
    const vector<BdnNode*>&
    fpnode_list() const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // このノードの footprint ノード
    vector<BdnNode*> mFpNodeList;

    // 種々のマークをパックしたもの
    ymuint32 mMarks;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カット列挙を実際に行う再帰関数
  bool
  enum_recur();

  /// @brief node のカットになったノードに c1mark をつけ，mMarkedNodes に入れる．
  void
  mark_cnode(BdnNode* node);

  /// @brief node の TFI に c1mark をつける．
  void
  mark_cnode2(BdnNode* node);

  /// @brief node のカットになったノードに c1mark をつけ，mMarkedNodes に入れる．
  void
  mark_cnode3(BdnNode* node);

  /// @brief cmark のついているノードを cnode_list に入れてcmarkを消す．
  void
  set_cur_node_list_recur(BdnNode* node,
			  vector<BdnNode*>& cnode_list);

  /// @brief フロンティアスタックにプッシュする．
  void
  push_node(BdnNode* node);

  /// @brief フロンティアスタックからノードをポップする．
  BdnNode*
  pop_node();

  /// @brief フロンティアスタックが空のとき true を返す．
  bool
  frontier_is_empty() const;

  /// @brief ノードを入力に確定する．
  void
  set_input(BdnNode* node);

  /// @brief ノードの作業領域を得る．
  NodeTemp&
  node_temp(BdnNode* node);

  /// @brief ノードの作業領域を得る．
  const NodeTemp&
  node_temp(BdnNode* node) const;

  /// @brief 状態を取り出す．
  /// - 0: 初期状態
  /// - 1: カットの内側
  /// - 2: カットの入力(境界)
  ymuint
  state(BdnNode* node) const;

  /// @brief 内部の印をつける．
  void
  set_inside_state(BdnNode* node);

  /// @brief 入力の印をつける．
  void
  set_input_state(BdnNode* node);

  /// @brief 状態を初期化する．
  void
  clear_state(BdnNode* node);

  /// @brief footprint マークを返す．
  bool
  fpmark(BdnNode* node) const;

  /// @brief footprint マークをつける．
  void
  set_fpmark(BdnNode* node);

  /// @brief footprint マークを消す．
  void
  clear_fpmark(BdnNode* node);

  /// @brief テンポラリマーク1を取り出す．
  bool
  temp1mark(BdnNode* node) const;

  /// @brief テンポラリマーク1をつける．
  void
  set_temp1mark(BdnNode* node);

  /// @brief テンポラリマーク2を取り出す．
  bool
  temp2mark(BdnNode* node) const;

  /// @brief テンポラリマーク2をつける．
  void
  set_temp2mark(BdnNode* ndoe);

  /// @brief テンポラリマーク1/2 を消す．
  void
  clear_tempmark(BdnNode* node);

  /// @brief 枝のマークを取り出す．
  /// @param[in] pos 枝番号 ( 0 or 1 )
  bool
  edge_mark(BdnNode* node,
	    ymuint pos) const;

  /// @brief 枝のマークをつける．
  /// @param[in] pos 枝番号 ( 0 or 1 )
  void
  set_edge_mark(BdnNode* node,
		ymuint pos);

  /// @brief 枝のマークを消す．
  /// @param[in] pos 枝番号 ( 0 or 1 )
  void
  clear_edge_mark(BdnNode* node,
		  ymuint pos);

  /// @brief footprint ノードのリストを返す．
  vector<BdnNode*>&
  fpnode_list(BdnNode* node);

  /// @brief footprint ノードのリストを返す．
  const vector<BdnNode*>&
  fpnode_list(BdnNode* node) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットが見つかったときの処理を行うファンクター
  EnumCutOp2* mOp;

  // カットサイズの制限値
  ymuint32 mLimit;

  // 全ノード数
  ymuint32 mNall;

  // 全カット数
  ymuint32 mNcAll;

  // 現在処理中のノードのカット数
  ymuint32 mNcCur;

  // 現在処理中の根のノード
  BdnNode* mRoot;

  // mFrontierStack のサイズ
  ymuint32 mFsSize;

  // フロンティアノードのスタック
  BdnNode** mFrontierStack;

  // mFrontierStack の先頭を指すポインタ
  BdnNode** mFsPos;

  // 確定した境界ノードを入れる配列
  BdnNode** mInputs;

  // mInputs の次の書き込み位置
  ymuint32 mInputPos;

  // 内部ノードのスタック
  ymuint32* mInodeStack;

  // 内部ノードスタックのスタックポインタ
  ymuint32* mIsPos;

  // 各ノードごとの作業領域
  vector<NodeTemp> mNodeTemp;

  // マークのついたノードを入れておく配列
  vector<BdnNode*> mMarkedNodes;

  // mMarkedNodes の末尾
  ymuint32 mMarkedNodesLast;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TopDown2::NodeTemp::NodeTemp() :
  mMarks(0U)
{
}

// @brief 状態を取り出す．
// - 0: 初期状態
// - 1: カットの内側
// - 2: カットの入力(境界)
inline
ymuint
TopDown2::NodeTemp::state() const
{
  return mMarks & 3U;
}

// @brief 内部の印をつける．
inline
void
TopDown2::NodeTemp::set_inside_state()
{
  clear_state();
  mMarks |= 1U;
}

// @brief 入力の印をつける．
inline
void
TopDown2::NodeTemp::set_input_state()
{
  clear_state();
  mMarks |= 2U;
}

// @brief 状態を初期化する．
inline
void
TopDown2::NodeTemp::clear_state()
{
  mMarks &= ~3U;
}

// @brief footprint マークを返す．
inline
bool
TopDown2::NodeTemp::fpmark() const
{
  return static_cast<bool>((mMarks >> 2) & 1U);
}

// @brief footprint マークをつける．
inline
void
TopDown2::NodeTemp::set_fpmark()
{
  mMarks |= 4U;
}

// @brief footprint マークを消す．
inline
void
TopDown2::NodeTemp::clear_fpmark()
{
  mMarks &= ~4U;
}

// @brief テンポラリマーク1を取り出す．
inline
bool
TopDown2::NodeTemp::temp1mark() const
{
  return static_cast<bool>((mMarks >> 3) & 1U);
}

// @brief テンポラリマーク1をつける．
inline
void
TopDown2::NodeTemp::set_temp1mark()
{
  mMarks |= 8U;
}

// @brief テンポラリマーク2を取り出す．
inline
bool
TopDown2::NodeTemp::temp2mark() const
{
  return static_cast<bool>((mMarks >> 4) & 1U);
}

// @brief テンポラリマーク2をつける．
inline
void
TopDown2::NodeTemp::set_temp2mark()
{
  mMarks |= 16U;
}

// @brief テンポラリマーク1/2 を消す．
inline
void
TopDown2::NodeTemp::clear_tempmark()
{
  mMarks &= ~(8U | 16U);
}

// @brief 枝のマークを取り出す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
bool
TopDown2::NodeTemp::edge_mark(ymuint pos) const
{
  return static_cast<bool>((mMarks >> (pos + 5)) & 1U);
}

// @brief 枝のマークをつける．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown2::NodeTemp::set_edge_mark(ymuint pos)
{
  mMarks |= (32U << pos);
}

// @brief 枝のマークを消す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown2::NodeTemp::clear_edge_mark(ymuint pos)
{
  mMarks &= ~(32U << pos);
}

// @brief footprint ノードのリストを返す．
inline
vector<BdnNode*>&
TopDown2::NodeTemp::fpnode_list()
{
  return mFpNodeList;
}

// @brief footprint ノードのリストを返す．
inline
const vector<BdnNode*>&
TopDown2::NodeTemp::fpnode_list() const
{
  return mFpNodeList;
}

// @brief ノードの作業領域を得る．
inline
TopDown2::NodeTemp&
TopDown2::node_temp(BdnNode* node)
{
  return mNodeTemp[node->id()];
}

// @brief ノードの作業領域を得る．
inline
const TopDown2::NodeTemp&
TopDown2::node_temp(BdnNode* node) const
{
  return mNodeTemp[node->id()];
}

// @brief 状態を取り出す．
// - 0: 初期状態
// - 1: カットの内側
// - 2: カットの入力(境界)
inline
ymuint
TopDown2::state(BdnNode* node) const
{
  return node_temp(node).state();
}

// @brief 内部の印をつける．
inline
void
TopDown2::set_inside_state(BdnNode* node)
{
  node_temp(node).set_inside_state();
}

// @brief 入力の印をつける．
inline
void
TopDown2::set_input_state(BdnNode* node)
{
  node_temp(node).set_input_state();
}

// @brief 状態を初期化する．
inline
void
TopDown2::clear_state(BdnNode* node)
{
  node_temp(node).clear_state();
}

// @brief footprint マークを返す．
inline
bool
TopDown2::fpmark(BdnNode* node) const
{
  return node_temp(node).fpmark();
}

// @brief footprint マークをつける．
inline
void
TopDown2::set_fpmark(BdnNode* node)
{
  node_temp(node).set_fpmark();
}

// @brief footprint マークを消す．
inline
void
TopDown2::clear_fpmark(BdnNode* node)
{
  node_temp(node).clear_fpmark();
}

// @brief テンポラリマーク1を取り出す．
inline
bool
TopDown2::temp1mark(BdnNode* node) const
{
  return node_temp(node).temp1mark();
}

// @brief テンポラリマーク1をつける．
inline
void
TopDown2::set_temp1mark(BdnNode* node)
{
  node_temp(node).set_temp1mark();
}

// @brief テンポラリマーク2を取り出す．
inline
bool
TopDown2::temp2mark(BdnNode* node) const
{
  return node_temp(node).temp2mark();
}

// @brief テンポラリマーク2をつける．
inline
void
TopDown2::set_temp2mark(BdnNode* node)
{
  node_temp(node).set_temp2mark();
}

// @brief テンポラリマーク1/2 を消す．
inline
void
TopDown2::clear_tempmark(BdnNode* node)
{
  node_temp(node).clear_tempmark();
}

// @brief 枝のマークを取り出す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
bool
TopDown2::edge_mark(BdnNode* node,
		   ymuint pos) const
{
  return node_temp(node).edge_mark(pos);
}

// @brief 枝のマークをつける．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown2::set_edge_mark(BdnNode* node,
		       ymuint pos)
{
  node_temp(node).set_edge_mark(pos);
}

// @brief 枝のマークを消す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown2::clear_edge_mark(BdnNode* node,
			 ymuint pos)
{
  node_temp(node).clear_edge_mark(pos);
}

// @brief footprint ノードのリストを返す．
inline
vector<BdnNode*>&
TopDown2::fpnode_list(BdnNode* node)
{
  return node_temp(node).fpnode_list();
}

// @brief footprint ノードのリストを返す．
inline
const vector<BdnNode*>&
TopDown2::fpnode_list(BdnNode* node) const
{
  return node_temp(node).fpnode_list();
}

// @brief フロンティアスタックにプッシュする．
inline
void
TopDown2::push_node(BdnNode* node)
{
  if ( mFsPos == mFrontierStack + mFsSize ) {
    BdnNode** old_stack = mFrontierStack;
    mFsSize <<= 1;
    mFrontierStack = new BdnNode*[mFsSize];
    BdnNode** sp = old_stack;
    BdnNode** dp = mFrontierStack;
    for ( ; dp != mFsPos; ++ sp, ++ dp) {
      *dp = *sp;
    }
    delete [] old_stack;
  }
  *mFsPos = node;
  ++ mFsPos;
  set_inside_state(node);
}

// @brief フロンティアスタックからノードをポップする．
inline
BdnNode*
TopDown2::pop_node()
{
  -- mFsPos;
  return *mFsPos;
}

// @brief フロンティアスタックが空のとき true を返す．
inline
bool
TopDown2::frontier_is_empty() const
{
  return mFsPos == mFrontierStack;
}

// @brief ノードを入力に確定する．
inline
void
TopDown2::set_input(BdnNode* node)
{
  set_input_state(node);
  mInputs[mInputPos] = node;
  ++ mInputPos;
}

END_NAMESPACE_YM

#endif // TOPDOWN2_H
