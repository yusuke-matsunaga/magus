
/// @file DtpgNgMgr.cc
/// @brief DtpgNgMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgNgMgr.h"
#include "DtpgNodeGroup.h"
#include "DtpgNgEdge.h"

#include "utils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス DtpgNgMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DtpgNgMgr::DtpgNgMgr() :
  mAlloc(4096)
{
#if !USE_STL_LIST
  mTop = NULL;
  mTail = NULL;
#endif
}

// @brief デストラクタ
DtpgNgMgr::~DtpgNgMgr()
{
}

// @brief ノードを作る．
DtpgNodeGroup*
DtpgNgMgr::add_node_group(const vector<TpgNode*>& node_list)
{
  void* p = mAlloc.get_memory(sizeof(DtpgNodeGroup));
  DtpgNodeGroup* ng = new (p) DtpgNodeGroup(node_list);
  ng->mId = mNodeList.size();
  mNodeList.push_back(ng);
  return ng;
}

BEGIN_NONAMESPACE

void
mark_tfo(DtpgNodeGroup* node,
	 vector<bool>& mark)
{
  if ( mark[node->id()] ) {
    return;
  }

  mark[node->id()] = true;

  for (vector<DtpgNgEdge*>::const_iterator p = node->fanout_list().begin();
       p != node->fanout_list().end(); ++ p) {
    mark_tfo((*p)->dst_node(), mark);
  }
}

bool
mark_tfi(DtpgNodeGroup* node,
	 vector<bool>& mark,
	 const vector<bool>& tfo_mark)
{
  if ( mark[node->id()] ) {
    return false;
  }

  if ( tfo_mark[node->id()] ) {
    return true;
  }

  mark[node->id()] = true;

  for (vector<DtpgNgEdge*>::const_iterator p = node->fanin_list().begin();
       p != node->fanin_list().end(); ++ p) {
    bool stat = mark_tfi((*p)->src_node(), mark, tfo_mark);
    if ( stat ) {
      return true;
    }
  }

  return false;
}

END_NONAMESPACE

// @brief 枝を作る．
void
DtpgNgMgr::add_edge(DtpgNodeGroup* src_node,
		    DtpgNodeGroup* dst_node)
{
  // 既におなじ接続関係が存在しないか調べる．
  if ( src_node->check_fanout(dst_node) ) {
    // 同じ接続があった．
    return;
  }

#if 0
  // 依存関係をチェックする．
  // 具体的には src_node の TFO と dst_node の TFI に src_node と dst_node 以外の
  // 共通要素があったら枝は作れない．
  {
    ymuint n = mNodeList.size();
    vector<bool> tfo_mark(n, false);
    for (vector<DtpgNgEdge*>::const_iterator p = src_node->fanout_list().begin();
	 p != src_node->fanout_list().end(); ++ p) {
      DtpgNodeGroup* node = (*p)->dst_node();
      if ( node != dst_node ) {
	mark_tfo(node, tfo_mark);
      }
    }
    vector<bool> tfi_mark(n, false);
    for (vector<DtpgNgEdge*>::const_iterator p = dst_node->fanin_list().begin();
	 p != dst_node->fanin_list().end(); ++ p) {
      DtpgNodeGroup* node = (*p)->src_node();
      if ( node != src_node ) {
	bool stat = mark_tfi(node, tfi_mark, tfo_mark);
	if ( stat ) {
	  return;
	}
      }
    }
  }
#endif

  // 新しい枝を作る．
  void* p = mAlloc.get_memory(sizeof(DtpgNgEdge));
  DtpgNgEdge* edge = new (p) DtpgNgEdge(src_node->size() + dst_node->size(), src_node, dst_node);

  // ファンアウトリストに追加する．
  src_node->mFanoutList.push_back(edge);

  // ファンインリストに追加する．
  dst_node->mFaninList.push_back(edge);

#if USE_STL_LIST
  mEdgeList.push_back(edge);
#else
  if ( mTail != NULL ) {
    mTail->mNext = edge;
    edge->mPrev = mTail;
    edge->mNext = NULL;
    mTail = edge;
  }
  else {
    mTop = mTail = edge;
    edge->mPrev = NULL;
    edge->mNext = NULL;
  }
#endif
}

// @brief 制限値を越えない範囲でマージする．
void
DtpgNgMgr::merge(ymuint size_limit)
{
  StopWatch timer;
  timer.start();
  for ( ; ; ) {
    ymuint min_size = size_limit + 1;
    DtpgNgEdge* min_edge = NULL;
#if USE_STL_LIST
    for (vector<DtpgNgEdge*>::iterator p = mEdgeList.begin();
	 p != mEdgeList.end(); ++ p) {
      DtpgNgEdge* edge = *p;
      if ( min_size > edge->size() ) {
	min_size = edge->size();
	min_edge = edge;
      }
    }
#else
    for (DtpgNgEdge* edge = mTop; edge != NULL; ) {
      DtpgNgEdge* next = edge->mNext;
      ymuint size = edge->size();
      if ( size > size_limit ) {
	delete_edge(edge);
      }
      else if ( min_size > size ) {
	min_size = size;
	min_edge = edge;
      }
      edge = next;
    }
#endif
    if ( min_edge == NULL ) {
      break;
    }

    merge_node(min_edge);
  }

  // 依存関係のループをチェック

  timer.stop();
  USTime time = timer.time();
  cout << "Merge Time = " << time << endl;
}

// @brief ノードリストを返す．
const vector<DtpgNodeGroup*>&
DtpgNgMgr::node_list() const
{
  return mNodeList;
}

// @brief ノードをマージする．
void
DtpgNgMgr::merge_node(DtpgNgEdge* edge)
{
  DtpgNodeGroup* src_node = edge->src_node();
  DtpgNodeGroup* dst_node = edge->dst_node();

#if 0
  cout << "merge_node(" << src_node->id() << ", " << dst_node->id() << ")" << endl;
#endif

  // dst_node のノードリストを src_node にマージする．
  src_node->mNodeList.insert(src_node->mNodeList.end(), dst_node->mNodeList.begin(), dst_node->mNodeList.end());

  // src_node のファンインリストに dst_node のファンインリストをマージする．
  for (vector<DtpgNgEdge*>::iterator p = dst_node->mFaninList.begin();
       p != dst_node->mFaninList.end(); ++ p) {
    DtpgNgEdge* edge1 = *p;
    if ( edge1 == edge ) {
      continue;
    }
    DtpgNodeGroup* src_node1 = edge1->src_node();
    if ( src_node->check_fanin(src_node1) ) {
      // 重複していた．
      src_node1->delete_fanout(edge1);
      delete_edge(edge1);
    }
    else {
#if 0
      cout << "(" << edge1->mSrcNode->id()
	   << ", " << edge1->mDstNode->id()
	   << ") ==> (" << edge1->mSrcNode->id()
	   << ", " << src_node->id()
	   << ")" << endl;
#endif
      edge1->mDstNode = src_node;
      assert_cond( edge1->mSrcNode != edge1->mDstNode, __FILE__, __LINE__);
      src_node->mFaninList.push_back(edge1);
    }
  }

  // src_node のファンアウトのリストから dst_node への枝を取り除く
  src_node->delete_fanout(edge);

  delete_edge(edge);

  // src_node のファンアウトリストに dst_node のファンアウトリストをマージする．
  for (vector<DtpgNgEdge*>::iterator p = dst_node->mFanoutList.begin();
       p != dst_node->mFanoutList.end(); ++ p) {
    DtpgNgEdge* edge1 = *p;
    DtpgNodeGroup* dst_node1 = edge1->dst_node();
    if ( dst_node1 == src_node ) {
      // 逆方向の枝は無視する．
      src_node->delete_fanin(edge1);
      delete_edge(edge1);
    }
    else if ( src_node->check_fanout(dst_node1) ) {
      // 重複していた．
      dst_node1->delete_fanin(edge1);
      delete_edge(edge1);
    }
    else {
#if 0
      cout << "(" << edge1->mSrcNode->id()
	   << ", " << edge1->mDstNode->id()
	   << ") ==> (" << src_node->id()
	   << ", " << edge1->mDstNode->id()
	   << ")" << endl;
#endif
      edge1->mSrcNode = src_node;
      assert_cond( edge1->mSrcNode != edge1->mDstNode, __FILE__, __LINE__);
      src_node->mFanoutList.push_back(edge1);
    }
  }

  // 枝のサイズを更新する．
  for (vector<DtpgNgEdge*>::iterator p = src_node->mFaninList.begin();
       p != src_node->mFaninList.end(); ++ p) {
    DtpgNgEdge* edge = *p;
    edge->mSize = edge->src_node()->size() + edge->dst_node()->size();
  }

  for (vector<DtpgNgEdge*>::iterator p = src_node->mFanoutList.begin();
       p != src_node->mFanoutList.end(); ++ p) {
    DtpgNgEdge* edge = *p;
    edge->mSize = edge->src_node()->size() + edge->dst_node()->size();
  }

  // dst_node を削除する．
  {
    vector<DtpgNodeGroup*>::iterator wpos = mNodeList.begin();
    for (vector<DtpgNodeGroup*>::iterator rpos = wpos;
	 rpos != mNodeList.end(); ++ rpos) {
      DtpgNodeGroup* node = *rpos;
      if ( node != dst_node ) {
	if ( wpos != rpos ) {
	  *wpos = node;
	}
	++ wpos;
      }
    }
    if ( wpos != mNodeList.end() ) {
      mNodeList.erase(wpos, mNodeList.end());
    }
  }
  //dst_node->~DtpgNodeGroup();
}

// @brief 枝を削除する．
void
DtpgNgMgr::delete_edge(DtpgNgEdge* edge)
{
#if 0
  cout << "  delete_edge(" << edge->src_node()->id() << ", " << edge->dst_node()->id() << ")" << endl;
#endif

#if USE_STL_LIST
  vector<DtpgNgEdge*>::iterator wpos = mEdgeList.begin();
  for (vector<DtpgNgEdge*>::iterator rpos = wpos;
       rpos != mEdgeList.end(); ++ rpos) {
    DtpgNgEdge* edge1 = *rpos;
    if ( edge1 != edge ) {
      if ( wpos != rpos ) {
	*wpos = edge1;
      }
      ++ wpos;
    }
  }
  if ( wpos != mEdgeList.end() ) {
    mEdgeList.erase(wpos, mEdgeList.end());
  }
#else
  DtpgNgEdge* p_edge = edge->mPrev;
  DtpgNgEdge* n_edge = edge->mNext;
  if ( p_edge != NULL ) {
    p_edge->mNext = n_edge;
  }
  else {
    mTop = n_edge;
  }
  if ( n_edge != NULL ) {
    n_edge->mPrev = p_edge;
  }
  else {
    mTail = p_edge;
  }
#endif
  edge->~DtpgNgEdge();
}


END_NAMESPACE_YM_SATPG
