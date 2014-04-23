
/// @file TopDown2.cc
/// @brief TopDown2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TopDown2.h"
#include "networks/BdnMgr.h"


//#define DEBUG_ENUM_RECUR 1


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス TopDown2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TopDown2::TopDown2()
{
  mFsSize = 1024;
  mFrontierStack = new const BdnNode*[mFsSize];
  mFsPos = &mFrontierStack[0];
}

// @brief デストラクタ
TopDown2::~TopDown2()
{
  delete [] mFrontierStack;
}

// @brief カット列挙を行う．
// @param[in] network 対象のネットワーク
// @param[in] limit カットサイズの制限
void
TopDown2::operator()(const BdnMgr& network,
		     ymuint limit,
		     EnumCutOp* op)
{
  mOp = op;

  ymuint n = network.max_node_id();
  mNodeTemp.clear();
  mNodeTemp.resize(n);

  mMarkedNodes.resize(n);

  mLimit = limit;

  mOp->all_init(network, limit);

  mInputs = new const BdnNode*[limit];

  mInodeStack = new ymuint32[network.lnode_num()];
  mIsPos = &mInodeStack[0];

  ymuint ni = network.input_num();
  ymuint nf = network.dff_num();
  mNall = ni + nf + network.lnode_num();
  mNcAll = 0;

  // 外部入力用のダミーカットを作る．
  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;

    mOp->node_init(node);

    // 自分自身のみからなるクラスタを登録する．
    mOp->found_cut(node, 0, NULL);

    ++ mNcAll;

    fpnode_list(node).push_back(node);

    mOp->node_end(node);
  }

  // 入力側からのトポロジカル順に内部ノードのカットを列挙する．
  vector<BdnNode*> node_list;
  network._sort(node_list);
  for (vector<BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;

    mMarkedNodesLast = 0;

    // ファンインの footprint ノードに temp1mark をつける．
    mark_cnode(node->fanin0());
    mark_cnode(node->fanin1());

    // 自身に temp1mark がついており，ファンインに temp1mark がついていない
    // ノードに temp2mark をつける．
    // temp2mark のついたノードが境界ノードとなる．
    for (ymuint i = 0; i < mMarkedNodesLast; ++ i) {
      const BdnNode* node = mMarkedNodes[i];
      if ( !node->is_logic() ||
	   !temp1mark(node->fanin0()) ||
	   !temp1mark(node->fanin1()) ) {
	set_temp2mark(node);
      }
    }

    // カットの列挙を行う．ただし c2mark を越えてフロンティアを伸ばさない．

    mOp->node_init(node);

    mInputPos = 0;
    mRoot = node;

    push_node(node);
    enum_recur();
    pop_node();
    clear_state(node);

    assert_cond( frontier_is_empty(), __FILE__, __LINE__);
    assert_cond( mInputPos == 0, __FILE__, __LINE__);

    // 今の列挙で使われたノードを footprint_node_list に格納する．
    vector<const BdnNode*>& fplist = fpnode_list(node);
    fplist.reserve(mMarkedNodesLast);
    set_cur_node_list_recur(node, fplist);

    mOp->node_end(node);

    // マークを消しておく．
    for (ymuint i = 0; i < mMarkedNodesLast; ++ i) {
      const BdnNode* node = mMarkedNodes[i];
      clear_tempmark(node);
    }
  }

  mOp->all_end(network, limit);

  delete [] mInputs;
  delete [] mInodeStack;
}

// @brief カット列挙を実際に行う再帰関数
bool
TopDown2::enum_recur()
{
  if ( frontier_is_empty() ) {
#if defined(DEBUG_ENUM_RECUR)
    cout << "FRONTIER IS EMPTY" << endl
	 << "  CUTS: ";
    for (ymuint i = 0; i < mInputPos; ++ i) {
      cout << " " << mInputs[i]->id();
    }
    cout << endl;
#endif

    for (ymuint i = 0; i < mInputPos; ++ i) {
      set_fpmark(mInputs[i]);
    }

    if ( mInputPos > 1 ) {
      mOp->found_cut(mRoot, mInputPos, mInputs);
    }
    else {
      mOp->found_cut(mRoot, 0, NULL);
    }

    ++ mNcAll;

    return true;
  }

  const BdnNode* node = pop_node();

  assert_cond( node->is_logic(), __FILE__, __LINE__);

#if defined(DEBUG_ENUM_RECUR)
  cout << "POP[1] " << node->id() << endl;
#endif

  bool has_cuts = false;

  if ( mInputPos < mLimit &&
       ( !node->is_logic() || node == mRoot || node->fanout_num() > 1 ) ) {
    // node を入力に固定して再帰を続ける
    set_input(node);

#if defined(DEBUG_ENUM_RECUR)
    cout << "MARK[1] " << node->id() << endl;
#endif

    has_cuts = enum_recur();

    // node を内部ノードにする．
    set_inside_state(node);
    -- mInputPos;

#if defined(DEBUG_ENUM_RECUR)
    cout << "UNMARK[1] " << node->id() << endl;
#endif
  }

  const BdnNode** old_fs_pos = mFsPos;
  ymuint old_input_pos = mInputPos;

  bool inode0_stat = false;
  const BdnNode* inode0 = node->fanin0();
  bool inode1_stat = false;
  const BdnNode* inode1 = node->fanin1();

  bool go = true;

  if ( state(inode0) == 0 ) {
    // inode0 はまだ未処理
    if ( temp2mark(inode0) ) {
      // inode0 は内部ノードにはなれない．
      if ( mInputPos < mLimit &&
	   ( !inode0->is_logic() || inode0->fanout_num() > 1 ) ) {
	// inode0 を入力にする．
	set_input(inode0);
	inode0_stat = true;
	set_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
	cout << "MARK[2] " << inode0->id() << endl;
#endif
      }
      else {
	// 内部ノードにも入力にもなれないのでエラー
	go = false;
      }
    }
    else {
      assert_cond( !temp2mark(inode0), __FILE__, __LINE__);
      assert_cond( inode0->is_logic(), __FILE__, __LINE__);
      // inode0 をフロンティアスタックに積む．
      push_node(inode0);
      inode0_stat = true;
      set_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
      cout << "PUSH[2] " << inode0->id() << endl;
#endif
    }
  }
  if ( go ) {
    if ( state(inode1) == 0 ) {
      // inode1 はまだ未処理
      if ( temp2mark(inode1) ) {
	// inode1 は内部ノードになれない．
	if ( mInputPos < mLimit &&
	     ( !inode0->is_logic() || inode0->fanout_num() > 1 ) ) {
	  // inode1 を入力にする．
	  set_input(inode1);
	  inode1_stat = true;
	  set_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	  cout << "MARK[3] " << inode0->id() << endl;
#endif
	}
	else {
	  // 内部ノードにも入力にもなれないのでエラー
	  go = false;
	}
      }
      else {
	assert_cond( !temp2mark(inode1), __FILE__, __LINE__);
	assert_cond( inode1->is_logic(), __FILE__, __LINE__);
	// inode1 をフロンティアスタックに積む．
	push_node(inode1);
	inode1_stat = true;
	set_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	cout << "PUSH[3] " << inode1->id() << endl;
#endif
      }
    }
  }
  if ( go ) {
    // node を内部ノードにして再帰を続ける．
    bool has_cuts1 = enum_recur();
    if ( has_cuts1 ) {
      set_fpmark(node);
      has_cuts = true;
    }
  }
  if ( inode1_stat ) {
    clear_state(inode1);
    clear_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
    cout << "UNMARK[3] " << inode1->id() << endl;
#endif
  }

  if ( inode0_stat ) {
    clear_state(inode0);
    clear_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
    cout << "UNMARK[2] " << inode0->id() << endl;
#endif
  }
  mFsPos = old_fs_pos;
  mInputPos = old_input_pos;

  push_node(node);

#if defined(DEBUG_ENUM_RECUR)
  cout << "PUSH[4] " << node->id() << endl;
#endif

  return has_cuts;
}

// @brief node のカットになったノードに c1mark をつけ，mMarkedNodes に入れる．
void
TopDown2::mark_cnode(const BdnNode* node)
{
  const vector<const BdnNode*>& fpnode_list1 = fpnode_list(node);
  for (vector<const BdnNode*>::const_iterator p = fpnode_list1.begin();
       p != fpnode_list1.end(); ++ p) {
    const BdnNode* node1 = *p;
    if ( !temp1mark(node1) ) {
      set_temp1mark(node1);
      mMarkedNodes[mMarkedNodesLast] = node1;
      ++ mMarkedNodesLast;
    }
  }
}

// @brief cmark のついているノードを cnode_list に入れてcmarkを消す．
void
TopDown2::set_cur_node_list_recur(const BdnNode* node,
				  vector<const BdnNode*>& cnode_list)
{
  if ( !fpmark(node) ) {
    return;
  }

  clear_fpmark(node);
  cnode_list.push_back(node);
  if ( node->is_logic() ) {
    set_cur_node_list_recur(node->fanin0(), cnode_list);
    set_cur_node_list_recur(node->fanin1(), cnode_list);
  }
}

END_NAMESPACE_YM
