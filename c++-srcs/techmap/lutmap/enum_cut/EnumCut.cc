﻿
/// @file lutmap/EnumCut.cc
/// @brief EnumCut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "EnumCut.h"

//#define DEBUG_ENUM_RECUR


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス EnumCut
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EnumCut::EnumCut()
{
  mFsSize = 1024;
  mFrontierStack = new const SbjNode*[mFsSize];
  mFsPos = &mFrontierStack[0];
}

// デストラクタ
EnumCut::~EnumCut()
{
  delete [] mFrontierStack;
}

// 入力数が limit 以下のクラスタを列挙する．
SizeType
EnumCut::operator()(
  const SbjGraph& sbjgraph,
  SizeType limit,
  EnumCutOp* op
)
{
  SizeType n = sbjgraph.node_num();
  mNodeTemp.clear();
  mNodeTemp.resize(n);

  mMarkedNodes.resize(n);

  mLimit = limit;
  mOp = op;

  mOp->all_init(sbjgraph, limit);

  mInputs = new const SbjNode*[limit];

  SizeType ni = sbjgraph.input_num();
  SizeType nl = sbjgraph.logic_num();

  mInodeStack = new SizeType[nl];
  mIsPos = &mInodeStack[0];

  // 外部入力用の(ダミーの)クラスタを作る．
  mNall = ni + nl;
  mNcAll = 0;
  mCurPos = 0;
  for ( auto node: sbjgraph.input_list() ) {
    mNcCur = 0;

    mOp->node_init(node, mCurPos);

    // 自分自身のみからなるクラスタを登録する．
    mOp->found(node);
    ++ mNcAll;
    ++ mNcCur;

    // 今の列挙で用いたノードを cut_node_list に格納しておく
    cnode_list(node).push_back(node);

    mOp->node_end(node, mCurPos, mNcCur);

    ++ mCurPos;
  }

  // 入力側から内部ノード用のクラスタを作る．
  for ( auto node: sbjgraph.logic_list() ) {
    mMarkedNodesLast = 0;

    for ( int i: {0, 1} ) {
      // ファンインの cut に含まれるノードに c1mark をつける．
      const SbjNode* inode = node->fanin(i);
      mark_cnode(inode);
    }

    // 自分に c1mark がついており，ファンインには c1mark がついていない
    // ノードに c2mark をつける．
    // c2mark のついたノードが境界ノードとなる．
    for ( int i = 0; i < mMarkedNodesLast; ++ i ) {
      const SbjNode* node = mMarkedNodes[i];
      if ( temp1mark(node) ) {
	if ( node->is_logic() ) {
	  for ( int i: {0, 1} ) {
	    const SbjNode* inode = node->fanin(i);
	    if ( !temp1mark(inode) ) {
	      set_temp2mark(node);
	      break;
	    }
	  }
	}
	else { // is_ppi() == true
	  set_temp2mark(node);
	}
      }
    }

    // クラスタの列挙を行う．
    // ただし c2mark にぶつかったらそれ以上，入力側にはいかない．
    mNcCur = 0;

    mOp->node_init(node, mCurPos);

    mInputPos = 0;
    mRoot = node;

    push_node(node);
    enum_recur();
    pop_node();
    clear_state(node);
    ASSERT_COND(frontier_is_empty() );
    ASSERT_COND(mInputPos == 0 );

    // 今の列挙で用いたノードを cut_node_list に格納しておく
    vector<const SbjNode*>& clist = cnode_list(node);
    clist.reserve(mMarkedNodesLast);
    set_cut_node_list_recur(node, clist);

    mOp->node_end(node, mCurPos, mNcCur);

    ++ mCurPos;

    // マークを消しておく
    for ( int i = 0; i < mMarkedNodesLast; ++ i ) {
      const SbjNode* node = mMarkedNodes[i];
      clear_tempmark(node);
    }
  }

  mOp->all_end(sbjgraph, limit);

  delete [] mInputs;
  delete [] mInodeStack;

  return mNcAll;
}

// node のカットになったノードに c1mark を付け，mMarkedNodes に入れる．
void
EnumCut::mark_cnode(
  const SbjNode* node
)
{
  for ( auto node: cnode_list(node) ) {
    if ( !temp1mark(node) ) {
      set_temp1mark(node);
      mMarkedNodes[mMarkedNodesLast] = node;
      ++ mMarkedNodesLast;
    }
  }
}

// node の TFI に c1mark を付ける．
void
EnumCut::mark_cnode2(
  const SbjNode* node
)
{
  if ( !temp1mark(node) ) {
    set_temp1mark(node);
    mMarkedNodes[mMarkedNodesLast] = node;
    ++ mMarkedNodesLast;
    if ( node->is_logic() ) {
      mark_cnode2(node->fanin(0));
      mark_cnode2(node->fanin(1));
    }
  }
}

// node のカットになったノードに c1mark を付け，marked_nodes に入れる．
void
EnumCut::mark_cnode3(
  const SbjNode* node
)
{
  for ( auto node: cnode_list(node) ) {
    if ( !temp1mark(node) ) {
      set_temp1mark(node);
      mMarkedNodes[mMarkedNodesLast] = node;
      ++ mMarkedNodesLast;
      if ( node->fanout_num() > 1 ) {
	set_temp2mark(node);
      }
    }
  }
}

#if 0
// root_depth よりも小さな depth を持つノードを frontier stack に積む．
void
EnumCut::get_frontier(
  const SbjNode* node,
  int root_depth
)
{
  if ( state(node) ) {
    return;
  }
  if ( node->depth() < root_depth ) {
    if ( temp2mark(node) ) {
      // 境界ノードなので入力とする．
      ASSERT_COND(mInputPos < mLimit );
      set_state2(node);
      mInputs[mInputPos] = node;
      ++ mInputPos;
    }
    else {
      // スタックに積む．
      push_node(node);
    }
  }
  else {
    // node に内部ノードの印をつけて子供のノードに再帰する．
    set_state1(node);
    set_cmark(node);
    get_frontier(node->fanin(0), root_depth);
    get_frontier(node->fanin(1), root_depth);
  }
}

// get_frontier でつけた印を消す．
void
EnumCut::clear_frontier(const SbjNode* node)
{
  if ( state(node) ) {
    clear_state(node);
    if ( node->is_logic() ) {
      clear_frontier(node->fanin(0));
      clear_frontier(node->fanin(1));
    }
  }
}
#endif

// frontier stack にノードをプッシュする
inline
void
EnumCut::push_node(
  const SbjNode* node
)
{
  if ( mFsPos == mFrontierStack + mFsSize ) {
    const SbjNode** old_stack = mFrontierStack;
    mFsSize <<= 1;
    mFrontierStack = new const SbjNode*[mFsSize];
    const SbjNode** sp = old_stack;
    const SbjNode** dp = mFrontierStack;
    for ( ; dp != mFsPos; ++ sp, ++ dp) {
      *dp = *sp;
    }
    delete [] old_stack;
  }
  *mFsPos = node;
  ++ mFsPos;
  set_state1(node);
}

// frontier stack からノードをポップする
inline
const SbjNode*
EnumCut::pop_node()
{
  -- mFsPos;
  return *mFsPos;
}

// frontier stack が空のとき true を返す．
inline
bool
EnumCut::frontier_is_empty() const
{
  return mFsPos == mFrontierStack;
}

bool
EnumCut::enum_recur()
{
  if ( frontier_is_empty() ) {
#if defined(DEBUG_ENUM_RECUR)
    cout << "FRONTIER IS EMPTY" << endl;
#endif

    for ( int i = 0; i < mInputPos; ++ i ) {
      set_cmark(mInputs[i]);
    }
    if ( mInputPos > 1 ) {
      mOp->found(mRoot, mInputPos, mInputs);
    }
    else {
      mOp->found(mRoot);
    }
    ++ mNcAll;
    ++ mNcCur;
    return true;
  }
  else {
    const SbjNode* node = pop_node();

#if defined(DEBUG_ENUM_RECUR)
    cout << "POP[1] " << node->id_str() << endl;
#endif

    // 速度に効くのでコメントアウトしている
    //ASSERT_COND(node->bmark() == 1 );
    //ASSERT_COND(node->is_logic() );

    bool has_cuts = false;

    if ( mInputPos < mLimit ) {
      // node を入力にして再帰を続ける．
      set_state2(node);
      mInputs[mInputPos] = node;
      ++ mInputPos;

#if defined(DEBUG_ENUM_RECUR)
      cout << "MARK[1] " << node->id_str() << endl;
#endif

      has_cuts = enum_recur();
      set_state1(node);
      -- mInputPos;

#if defined(DEBUG_ENUM_RECUR)
      cout << "UNMARK[1] " << node->id_str() << endl;
#endif
    }

    const SbjNode** old_fs_pos = mFsPos;
    int old_input_pos = mInputPos;
    bool recur = true;
    bool inode0_stat = false;
    const SbjNode* inode0 = node->fanin(0);
    if ( state(inode0) == 0 ) {
      if ( !temp2mark(inode0) ) {
	push_node(inode0);
	inode0_stat = true;
	set_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
	cout << "PUSH[2] " << inode0->id_str() << endl;
#endif
      }
      else if ( mInputPos < mLimit ) {
	set_state2(inode0);
	mInputs[mInputPos] = inode0;
	++ mInputPos;
	inode0_stat = true;
	set_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
	cout << "MARK[2] " << inode0->id_str() << endl;
#endif
      }
      else {
	recur = false;
      }
    }
    if ( recur ) {
      bool inode1_stat = false;
      const SbjNode* inode1 = node->fanin(1);
      if ( state(inode1) == 0 ) {
	if ( !temp2mark(inode1) ) {
	  push_node(inode1);
	  inode1_stat = true;
	  set_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	  cout << "PUSH[3] " << inode1->id_str() << endl;
#endif
	}
	else if ( mInputPos < mLimit ) {
	  set_state2(inode1);
	  mInputs[mInputPos] = inode1;
	  ++ mInputPos;
	  inode1_stat = true;
	  set_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	  cout << "MARK[3] " << inode0->id_str() << endl;
#endif
	}
	else {
	  recur = false;
	}
      }
      if ( recur ) {
	// node を内部ノードにして再帰を続ける．
	bool has_cuts1 = enum_recur();
	if ( has_cuts1 ) {
	  set_cmark(node);
	  has_cuts = true;
	}
      }
      if ( inode1_stat ) {
	clear_state(inode1);
	clear_edge_mark(node, 1);

#if defined(DEBUG_ENUM_RECUR)
	cout << "UNMARK[3] " << inode1->id_str() << endl;
#endif
      }
    }
    if ( inode0_stat ) {
      clear_state(inode0);
      clear_edge_mark(node, 0);

#if defined(DEBUG_ENUM_RECUR)
      cout << "UNMARK[2] " << inode0->id_str() << endl;
#endif
    }
    mFsPos = old_fs_pos;
    mInputPos = old_input_pos;

    push_node(node);

#if defined(DEBUG_ENUM_RECUR)
    cout << "PUSH[4] " << node->id_str() << endl;
#endif

    return has_cuts;
  }
}

// cmark の付いているノードを cnode_list に入れて cmark を消す．
void
EnumCut::set_cut_node_list_recur(
  const SbjNode* node,
  vector<const SbjNode*>& cnode_list
)
{
  if ( !cmark(node) ) {
    return;
  }
  clear_cmark(node);
  cnode_list.push_back(node);
  if ( node->is_logic() ) {
    set_cut_node_list_recur(node->fanin(0), cnode_list);
    set_cut_node_list_recur(node->fanin(1), cnode_list);
  }
}

END_NAMESPACE_LUTMAP
