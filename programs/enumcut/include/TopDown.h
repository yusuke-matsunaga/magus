#ifndef TOPDOWN_H
#define TOPDOWN_H

/// @file TopDown.h
/// @brief TopDown のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class TopDown TopDown.h "TopDown.h"
/// @brief トップダウンのカット列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class TopDown :
  public EnumCut
{
public:

  /// @brief コンストラクタ
  TopDown();

  /// @brief デストラクタ
  virtual
  ~TopDown();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(const BdnMgr& network,
	     ymuint limit,
	     EnumCutOp* op);


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
    vector<const BdnNode*>&
    fpnode_list();

    /// @brief footprint ノードのリストを返す．
    const vector<const BdnNode*>&
    fpnode_list() const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // このノードの footprint ノード
    vector<const BdnNode*> mFpNodeList;

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
  mark_cnode(const BdnNode* node);

  /// @brief node の TFI に c1mark をつける．
  void
  mark_cnode2(const BdnNode* node);

  /// @brief node のカットになったノードに c1mark をつけ，mMarkedNodes に入れる．
  void
  mark_cnode3(const BdnNode* node);

  /// @brief cmark のついているノードを cnode_list に入れてcmarkを消す．
  void
  set_cur_node_list_recur(const BdnNode* node,
			  vector<const BdnNode*>& cnode_list);

  /// @brief フロンティアスタックにプッシュする．
  void
  push_node(const BdnNode* node);

  /// @brief フロンティアスタックからノードをポップする．
  const BdnNode*
  pop_node();

  /// @brief フロンティアスタックが空のとき true を返す．
  bool
  frontier_is_empty() const;

  /// @brief ノードを入力に確定する．
  void
  set_input(const BdnNode* node);

  /// @brief ノードの作業領域を得る．
  NodeTemp&
  node_temp(const BdnNode* node);

  /// @brief ノードの作業領域を得る．
  const NodeTemp&
  node_temp(const BdnNode* node) const;

  /// @brief 状態を取り出す．
  /// - 0: 初期状態
  /// - 1: カットの内側
  /// - 2: カットの入力(境界)
  ymuint
  state(const BdnNode* node) const;

  /// @brief 内部の印をつける．
  void
  set_inside_state(const BdnNode* node);

  /// @brief 入力の印をつける．
  void
  set_input_state(const BdnNode* node);

  /// @brief 状態を初期化する．
  void
  clear_state(const BdnNode* node);

  /// @brief footprint マークを返す．
  bool
  fpmark(const BdnNode* node) const;

  /// @brief footprint マークをつける．
  void
  set_fpmark(const BdnNode* node);

  /// @brief footprint マークを消す．
  void
  clear_fpmark(const BdnNode* node);

  /// @brief テンポラリマーク1を取り出す．
  bool
  temp1mark(const BdnNode* node) const;

  /// @brief テンポラリマーク1をつける．
  void
  set_temp1mark(const BdnNode* node);

  /// @brief テンポラリマーク2を取り出す．
  bool
  temp2mark(const BdnNode* node) const;

  /// @brief テンポラリマーク2をつける．
  void
  set_temp2mark(const BdnNode* ndoe);

  /// @brief テンポラリマーク1/2 を消す．
  void
  clear_tempmark(const BdnNode* node);

  /// @brief 枝のマークを取り出す．
  /// @param[in] pos 枝番号 ( 0 or 1 )
  bool
  edge_mark(const BdnNode* node,
	    ymuint pos) const;

  /// @brief 枝のマークをつける．
  /// @param[in] pos 枝番号 ( 0 or 1 )
  void
  set_edge_mark(const BdnNode* node,
		ymuint pos);

  /// @brief 枝のマークを消す．
  /// @param[in] pos 枝番号 ( 0 or 1 )
  void
  clear_edge_mark(const BdnNode* node,
		  ymuint pos);

  /// @brief footprint ノードのリストを返す．
  vector<const BdnNode*>&
  fpnode_list(const BdnNode* node);

  /// @brief footprint ノードのリストを返す．
  const vector<const BdnNode*>&
  fpnode_list(const BdnNode* node) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットが見つかったときの処理を行うファンクター
  EnumCutOp* mOp;

  // カットサイズの制限値
  ymuint32 mLimit;

  // 全ノード数
  ymuint32 mNall;

  // 全カット数
  ymuint32 mNcAll;

  // 現在処理中のノードのカット数
  ymuint32 mNcCur;

  // 現在処理中の根のノード
  const BdnNode* mRoot;

  // mFrontierStack のサイズ
  ymuint32 mFsSize;

  // フロンティアノードのスタック
  const BdnNode** mFrontierStack;

  // mFrontierStack の先頭を指すポインタ
  const BdnNode** mFsPos;

  // 確定した境界ノードを入れる配列
  const BdnNode** mInputs;

  // mInputs の次の書き込み位置
  ymuint32 mInputPos;

  // 内部ノードのスタック
  ymuint32* mInodeStack;

  // 内部ノードスタックのスタックポインタ
  ymuint32* mIsPos;

  // 各ノードごとの作業領域
  vector<NodeTemp> mNodeTemp;

  // マークのついたノードを入れておく配列
  vector<const BdnNode*> mMarkedNodes;

  // mMarkedNodes の末尾
  ymuint32 mMarkedNodesLast;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
TopDown::NodeTemp::NodeTemp() :
  mMarks(0U)
{
}

// @brief 状態を取り出す．
// - 0: 初期状態
// - 1: カットの内側
// - 2: カットの入力(境界)
inline
ymuint
TopDown::NodeTemp::state() const
{
  return mMarks & 3U;
}

// @brief 内部の印をつける．
inline
void
TopDown::NodeTemp::set_inside_state()
{
  clear_state();
  mMarks |= 1U;
}

// @brief 入力の印をつける．
inline
void
TopDown::NodeTemp::set_input_state()
{
  clear_state();
  mMarks |= 2U;
}

// @brief 状態を初期化する．
inline
void
TopDown::NodeTemp::clear_state()
{
  mMarks &= ~3U;
}

// @brief footprint マークを返す．
inline
bool
TopDown::NodeTemp::fpmark() const
{
  return static_cast<bool>((mMarks >> 2) & 1U);
}

// @brief footprint マークをつける．
inline
void
TopDown::NodeTemp::set_fpmark()
{
  mMarks |= 4U;
}

// @brief footprint マークを消す．
inline
void
TopDown::NodeTemp::clear_fpmark()
{
  mMarks &= ~4U;
}

// @brief テンポラリマーク1を取り出す．
inline
bool
TopDown::NodeTemp::temp1mark() const
{
  return static_cast<bool>((mMarks >> 3) & 1U);
}

// @brief テンポラリマーク1をつける．
inline
void
TopDown::NodeTemp::set_temp1mark()
{
  mMarks |= 8U;
}

// @brief テンポラリマーク2を取り出す．
inline
bool
TopDown::NodeTemp::temp2mark() const
{
  return static_cast<bool>((mMarks >> 4) & 1U);
}

// @brief テンポラリマーク2をつける．
inline
void
TopDown::NodeTemp::set_temp2mark()
{
  mMarks |= 16U;
}

// @brief テンポラリマーク1/2 を消す．
inline
void
TopDown::NodeTemp::clear_tempmark()
{
  mMarks &= ~(8U | 16U);
}

// @brief 枝のマークを取り出す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
bool
TopDown::NodeTemp::edge_mark(ymuint pos) const
{
  return static_cast<bool>((mMarks >> (pos + 5)) & 1U);
}

// @brief 枝のマークをつける．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown::NodeTemp::set_edge_mark(ymuint pos)
{
  mMarks |= (32U << pos);
}

// @brief 枝のマークを消す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown::NodeTemp::clear_edge_mark(ymuint pos)
{
  mMarks &= ~(32U << pos);
}

// @brief footprint ノードのリストを返す．
inline
vector<const BdnNode*>&
TopDown::NodeTemp::fpnode_list()
{
  return mFpNodeList;
}

// @brief footprint ノードのリストを返す．
inline
const vector<const BdnNode*>&
TopDown::NodeTemp::fpnode_list() const
{
  return mFpNodeList;
}

// @brief ノードの作業領域を得る．
inline
TopDown::NodeTemp&
TopDown::node_temp(const BdnNode* node)
{
  return mNodeTemp[node->id()];
}

// @brief ノードの作業領域を得る．
inline
const TopDown::NodeTemp&
TopDown::node_temp(const BdnNode* node) const
{
  return mNodeTemp[node->id()];
}

// @brief 状態を取り出す．
// - 0: 初期状態
// - 1: カットの内側
// - 2: カットの入力(境界)
inline
ymuint
TopDown::state(const BdnNode* node) const
{
  return node_temp(node).state();
}

// @brief 内部の印をつける．
inline
void
TopDown::set_inside_state(const BdnNode* node)
{
  node_temp(node).set_inside_state();
}

// @brief 入力の印をつける．
inline
void
TopDown::set_input_state(const BdnNode* node)
{
  node_temp(node).set_input_state();
}

// @brief 状態を初期化する．
inline
void
TopDown::clear_state(const BdnNode* node)
{
  node_temp(node).clear_state();
}

// @brief footprint マークを返す．
inline
bool
TopDown::fpmark(const BdnNode* node) const
{
  return node_temp(node).fpmark();
}

// @brief footprint マークをつける．
inline
void
TopDown::set_fpmark(const BdnNode* node)
{
  node_temp(node).set_fpmark();
}

// @brief footprint マークを消す．
inline
void
TopDown::clear_fpmark(const BdnNode* node)
{
  node_temp(node).clear_fpmark();
}

// @brief テンポラリマーク1を取り出す．
inline
bool
TopDown::temp1mark(const BdnNode* node) const
{
  return node_temp(node).temp1mark();
}

// @brief テンポラリマーク1をつける．
inline
void
TopDown::set_temp1mark(const BdnNode* node)
{
  node_temp(node).set_temp1mark();
}

// @brief テンポラリマーク2を取り出す．
inline
bool
TopDown::temp2mark(const BdnNode* node) const
{
  return node_temp(node).temp2mark();
}

// @brief テンポラリマーク2をつける．
inline
void
TopDown::set_temp2mark(const BdnNode* node)
{
  node_temp(node).set_temp2mark();
}

// @brief テンポラリマーク1/2 を消す．
inline
void
TopDown::clear_tempmark(const BdnNode* node)
{
  node_temp(node).clear_tempmark();
}

// @brief 枝のマークを取り出す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
bool
TopDown::edge_mark(const BdnNode* node,
		   ymuint pos) const
{
  return node_temp(node).edge_mark(pos);
}

// @brief 枝のマークをつける．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown::set_edge_mark(const BdnNode* node,
		       ymuint pos)
{
  node_temp(node).set_edge_mark(pos);
}

// @brief 枝のマークを消す．
// @param[in] pos 枝番号 ( 0 or 1 )
inline
void
TopDown::clear_edge_mark(const BdnNode* node,
			 ymuint pos)
{
  node_temp(node).clear_edge_mark(pos);
}

// @brief footprint ノードのリストを返す．
inline
vector<const BdnNode*>&
TopDown::fpnode_list(const BdnNode* node)
{
  return node_temp(node).fpnode_list();
}

// @brief footprint ノードのリストを返す．
inline
const vector<const BdnNode*>&
TopDown::fpnode_list(const BdnNode* node) const
{
  return node_temp(node).fpnode_list();
}

// @brief フロンティアスタックにプッシュする．
inline
void
TopDown::push_node(const BdnNode* node)
{
  if ( mFsPos == mFrontierStack + mFsSize ) {
    const BdnNode** old_stack = mFrontierStack;
    mFsSize <<= 1;
    mFrontierStack = new const BdnNode*[mFsSize];
    const BdnNode** sp = old_stack;
    const BdnNode** dp = mFrontierStack;
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
const BdnNode*
TopDown::pop_node()
{
  -- mFsPos;
  return *mFsPos;
}

// @brief フロンティアスタックが空のとき true を返す．
inline
bool
TopDown::frontier_is_empty() const
{
  return mFsPos == mFrontierStack;
}

// @brief ノードを入力に確定する．
inline
void
TopDown::set_input(const BdnNode* node)
{
  set_input_state(node);
  mInputs[mInputPos] = node;
  ++ mInputPos;
}

END_NAMESPACE_YM

#endif // TOPDOWN_H
