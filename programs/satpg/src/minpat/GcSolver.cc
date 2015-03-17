
/// @file GcSolver.cc
/// @brief GcSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcSolver.h"
#include "GcNode.h"
#include "GcNodeHeap.h"
#include "McMatrix.h"
#include "YmUtils/Graph.h"


BEGIN_NAMESPACE_YM_SATPG

BEGIN_NONAMESPACE

// @brief ノードに彩色して情報を更新する．
void
color_node(GcNode* node,
	   ymuint color,
	   GcNodeHeap& node_heap)
{
  // node に color の色を割り当てる．
  node->set_color(color);

  // node に隣接するノードの SAT degree を更新する．
  const vector<GcNode*>& node_list = node->link_list();
  for (vector<GcNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    GcNode* node1 = *p;
    if ( node1->is_deleted() ) {
      continue;
    }
    if ( node1->color() == 0 ) {
      // node1 が未着色の場合
      if ( !node1->check_adj_color(color) ) {
	// node1 にとって color は新規の隣り合う色だった．
	node1->add_adj_color(color);

	// SAT degree が変わったのでヒープ上の位置も更新する．
	node_heap.update(node1);
      }
    }
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス GcSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_num ノード数
GcSolver::GcSolver(ymuint node_num)
{
  mNodeNum = node_num;
  if ( mNodeNum > 0 ) {
    ymuint vectlen = (mNodeNum + 63) / 64;
    mNodeArray = new GcNode[mNodeNum];
    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      node->init(i, vectlen);
    }
  }
  else {
    mNodeArray = NULL;
  }
}

// @brief デストラクタ
GcSolver::~GcSolver()
{
  delete [] mNodeArray;
}

// @brief ノード数を得る．
ymuint
GcSolver::node_num() const
{
  return mNodeNum;
}

// @brief ノードを取り出す．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
GcNode*
GcSolver::node(ymuint id)
{
  ASSERT_COND( id < node_num() );
  return &mNodeArray[id];
}

// @brief 彩色する．
// @param[in] color_group 同じ色のノード番号のリストの配列
// @return 彩色数を返す．
ymuint
GcSolver::coloring(vector<vector<ymuint> >& color_group)
{
  cout << "coloring start" << endl;

  // dsatur アルゴリズムを用いる．

  // 最大故障番号の計算
  mMaxFaultId = 0;
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    const vector<ymuint>& fault_list = mNodeArray[i].fault_list();
    for (vector<ymuint>::const_iterator p = fault_list.begin();
	 p != fault_list.end(); ++ p) {
      ymuint f = *p;
      if ( mMaxFaultId < f ) {
	mMaxFaultId = f;
      }
    }
  }
  ++ mMaxFaultId;
  mFaultMark.resize(mMaxFaultId, false);

  McMatrix mcmx(mMaxFaultId, mNodeNum, mNodeArray);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    const vector<ymuint>& fault_list = node->fault_list();
    for (vector<ymuint>::const_iterator p = fault_list.begin();
	 p != fault_list.end(); ++ p) {
      ymuint f = *p;
      mcmx.insert_elem(f, i);
    }
  }

  vector<ymuint> col_set;
  mcmx.reduce(col_set);
  for (vector<ymuint>::const_iterator p = col_set.begin();
       p != col_set.end(); ++ p) {
    ymuint col = *p;
    GcNode* node = &mNodeArray[col];
    node->set_selected();
    if ( 0 ) {
      cout << " Node#" << node->id() << " is selected" << endl;
    }
  }
  {
    ymuint col_pos = 0;
    const McColHead* ch = mcmx.col_front();
    while ( !mcmx.is_col_end(ch) && col_pos < mNodeNum ) {
      if ( col_pos < ch->pos() ) {
	GcNode* node = &mNodeArray[col_pos];
	if ( !node->is_selected() ) {
	  node->set_deleted();
	  if ( 0 ) {
	    cout << " Node#" << node->id() << " is deleted" << endl;
	  }
	  const vector<GcNode*>& link_list = node->link_list();
	  for (vector<GcNode*>::const_iterator p = link_list.begin();
	       p != link_list.end(); ++ p) {
	    GcNode* node1 = *p;
	    node1->delete_link(node);
	  }
	}
	++ col_pos;
      }
      else if ( col_pos == ch->pos() ) {
	++ col_pos;
	ch = ch->next();
      }
      else {
	ASSERT_NOT_REACHED;
      }
    }
    for ( ; col_pos < mNodeNum; ++ col_pos) {
      GcNode* node = &mNodeArray[col_pos];
      if ( !node->is_selected() ) {
	node->set_deleted();
	if ( 0 ) {
	  cout << " Node#" << node->id() << " is deleted" << endl;
	}
	const vector<GcNode*>& link_list = node->link_list();
	for (vector<GcNode*>::const_iterator p = link_list.begin();
	     p != link_list.end(); ++ p) {
	  GcNode* node1 = *p;
	  node1->delete_link(node);
	}
      }
    }
  }

  GcNodeHeap node_heap(mNodeNum);

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    if ( !node->is_deleted() ) {
      node_heap.put_node(node);
    }
  }

  mMaxColor = 1;

  // 1: 隣接するノード数が最大のノードを選び彩色する．
  //    ソートしているので先頭のノードを選べば良い．
  GcNode* max_node = node_heap.get_min();
  color_node(max_node, mMaxColor, node_heap);
  cover_fault(max_node);
  if ( 0 ) {
    cout << "max_node = " << max_node->id();
    if ( max_node->is_selected() ) {
      cout << " [selected]";
    }
    cout << endl;
  }

  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( !node_heap.empty() ) {
    GcNode* max_node = node_heap.get_min();
    if ( max_node->is_deleted() ) {
      continue;
    }
    {
      // カバーすべき故障が残っているか調べる．
      const vector<ymuint>& fault_list = max_node->fault_list();
      bool covered = true;
      for (vector<ymuint>::const_iterator p = fault_list.begin();
	   p != fault_list.end(); ++ p) {
	ymuint f = *p;
	if ( !mFaultMark[f] ) {
	  covered = false;
	  break;
	}
      }
      if ( covered ) {
	// このノードは取り出すだけ．
	max_node->set_deleted();
	const vector<GcNode*>& link_list = max_node->link_list();
	for (vector<GcNode*>::const_iterator p = link_list.begin();
	     p != link_list.end(); ++ p) {
	  GcNode* node = *p;
	  node->delete_link(max_node);
	}
	continue;
      }
    }
    if ( 0 ) {
      cout << "max_node = " << max_node->id();
      if ( max_node->is_selected() ) {
	cout << " [selected]";
      }
      cout << endl;
    }

    // max_node につけることのできる最小の色番号を求める．
    clear_count();
    ymuint cnum = 0;
    const vector<GcNode*>& node_list = max_node->link_list();
    for (vector<GcNode*>::const_iterator p = node_list.begin();
	 p != node_list.end(); ++ p) {
      GcNode* node1 = *p;
      ymuint c = node1->color();
      if ( c > 0 ) {
	if ( !mCountArray[c - 1] ) {
	  mCountArray[c - 1] = true;
	  ++ cnum;
	  if ( cnum == mMaxColor ) {
	    break;
	  }
	}
      }
    }
    ymuint min_col = 0;
    for (ymuint c = 0; c < mMaxColor; ++ c) {
      if ( !mCountArray[c] ) {
	min_col = c + 1;
	break;
      }
    }
    if ( min_col == 0 ) {
      ++ mMaxColor;
      min_col = mMaxColor;
    }
    color_node(max_node, min_col, node_heap);
    if ( !max_node->is_selected() ) {
      max_node->set_selected();
      cover_fault(max_node);
      mcmx.select_col(max_node->id());
      vector<ymuint> col_set;
      mcmx.reduce(col_set);
      for (vector<ymuint>::const_iterator p = col_set.begin();
	   p != col_set.end(); ++ p) {
	ymuint col = *p;
	GcNode* node = &mNodeArray[col];
	node->set_selected();
	node_heap.update(node);
	if ( 0 ) {
	  cout << " Node#" << node->id() << " is selected" << endl;
	}
      }
      {
	ymuint col_pos = 0;
	const McColHead* ch = mcmx.col_front();
	while ( !mcmx.is_col_end(ch) && col_pos < mNodeNum ) {
	  if ( col_pos < ch->pos() ) {
	    GcNode* node = &mNodeArray[col_pos];
	    if ( !node->is_selected() && !node->is_deleted() ) {
	      node->set_deleted();
	      if ( 0 ) {
		cout << " Node#" << node->id() << " is deleted" << endl;
	      }
	      const vector<GcNode*>& link_list = node->link_list();
	      for (vector<GcNode*>::const_iterator p = link_list.begin();
		   p != link_list.end(); ++ p) {
		GcNode* node1 = *p;
		node1->delete_link(node);
	      }
	    }
	    ++ col_pos;
	  }
	  else if ( col_pos == ch->pos() ) {
	    ++ col_pos;
	    ch = ch->next();
	  }
	  else {
	    ASSERT_NOT_REACHED;
	  }
	}
	for ( ; col_pos < mNodeNum; ++ col_pos) {
	  GcNode* node = &mNodeArray[col_pos];
	  if ( !node->is_selected() && !node->is_deleted() ) {
	    node->set_deleted();
	    if ( 0 ) {
	      cout << " Node#" << node->id() << " is deleted" << endl;
	    }
	    const vector<GcNode*>& link_list = node->link_list();
	    for (vector<GcNode*>::const_iterator p = link_list.begin();
		 p != link_list.end(); ++ p) {
	      GcNode* node1 = *p;
	      node1->delete_link(node);
	    }
	  }
	}
      }
    }
  }

  // 検証
  // もちろん最小色数ではないが，同じ色が隣接していないことを確認する．
  // また，未彩色のノードがないことも確認する．
  // 違反が見つかったら例外を送出する．
  if ( true ) {
    ASSERT_COND( mcmx.is_row_end(mcmx.row_front()) );
    vector<bool> tmp_mark(mMaxFaultId, false);
    vector<bool> tmp_mark2(mMaxFaultId, false);
    vector<ymuint> all_fault_list;

    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      const vector<ymuint>& fault_list = node->fault_list();
      for (vector<ymuint>::const_iterator p = fault_list.begin();
	   p != fault_list.end(); ++ p) {
	ymuint f = *p;
	if ( !tmp_mark2[f] ) {
	  tmp_mark2[f] = true;
	  all_fault_list.push_back(f);
	}
      }
    }

    for (ymuint i = 0; i < mNodeNum; ++ i) {
      GcNode* node = &mNodeArray[i];
      if ( node->is_deleted() ) {
	continue;
      }
      ymuint c0 = node->color();
      ASSERT_COND( c0 > 0 );

      const vector<GcNode*>& node_list = node->link_list();
      for (vector<GcNode*>::const_iterator p = node_list.begin();
	   p != node_list.end(); ++ p) {
	GcNode* node1 = *p;
	ymuint c1 = node1->color();
	ASSERT_COND( c0 != c1 );
      }

      const vector<ymuint>& fault_list = node->fault_list();
      for (vector<ymuint>::const_iterator p = fault_list.begin();
	   p != fault_list.end(); ++ p) {
	ymuint f = *p;
	tmp_mark[f] = true;
      }
    }
    bool error = false;
    for (vector<ymuint>::const_iterator p = all_fault_list.begin();
	 p != all_fault_list.end(); ++ p) {
      ymuint f = *p;
      if ( !tmp_mark[f] ) {
	cout << "Fault#" << f << " is not covered"
	     << " mFaultMark = " << mFaultMark[f] << endl;
	error = true;
      }
    }
    ASSERT_COND( !error );
  }

  // 結果を color_group に入れる．
  color_group.clear();
  color_group.resize(mMaxColor);
  for (ymuint i = 0; i < mNodeNum; ++ i) {
    GcNode* node = &mNodeArray[i];
    if ( node->is_deleted() ) {
      continue;
    }

    ymuint c = node->color();

    ASSERT_COND( c > 0 );
    ASSERT_COND( c <= mMaxColor );

    color_group[c - 1].push_back(i);
  }

  return mMaxColor;
}

// @brief mCountArray をクリアする．
void
GcSolver::clear_count()
{
  mCountArray.clear();
  mCountArray.resize(mMaxColor, false);
}

// @brief ノードがカバーしている故障に印をつける．
void
GcSolver::cover_fault(GcNode* node)
{
  const vector<ymuint>& fault_list = node->fault_list();
  for (vector<ymuint>::const_iterator p = fault_list.begin();
       p != fault_list.end(); ++ p) {
    ymuint f = *p;
    mFaultMark[f] = true;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス GcNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcNode::GcNode()
{
  mColorSet = NULL;
  mAdjDegree = 0;
  mSatDegree = 0;
  mColor = 0;
  mSelected = false;
  mDeleted = false;
}

// @brief デストラクタ
GcNode::~GcNode()
{
  delete [] mColorSet;
}

// @brief 初期化する．
void
GcNode::init(ymuint id,
	     ymuint vectlen)
{
  mId = id;
  mColorSet = new ymuint64[vectlen];
  for (ymuint i = 0; i < vectlen; ++ i) {
    mColorSet[i] = 0UL;
  }
}

// @brief 隣接するノードを削除する．
void
GcNode::delete_link(GcNode* node)
{
  -- mAdjDegree;
}

END_NAMESPACE_YM_SATPG
