
/// @file LrMgr.cc
/// @brief LrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "LrMgr.h"
#include "YmLogic/Aig.h"
#include "YmLogic/AigMgr.h"


#if defined(YM_DEBUG)
#define DEBUG_FLAG 1
#endif


#if !defined(DEBUG_FLAG)
#define DEBUG_FLAG 0
#endif


BEGIN_NAMESPACE_YM_AIG

BEGIN_NONAMESPACE

const ymuint debug = DEBUG_FLAG;

struct Npn4Cannon
{
  ymuint16 mFunc;
  ymuint16 mPerm;
};

// 4入力のNPN同値類の代表関数への変換を表す配列
Npn4Cannon npn4cannon[] = {
#include "npn4cannon.h"
};

// @brief 関数ベクタを代表関数に変換する(4入力版)
ymuint16
cannonical4(ymuint16 func,
	    ymuint16& cperm)
{
  const Npn4Cannon& cannon = npn4cannon[func];
  ymuint32 cfunc = cannon.mFunc;
  cperm = cannon.mPerm;
  return cfunc;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// local rewrite を行うクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LrMgr::LrMgr()
{
  mLogLevel = 0;
  mLogStream = new ofstream("/dev/null");

  init_table();
}

// @brief デストラクタ
LrMgr::~LrMgr()
{
  if ( mLogStream != &cout ) {
    delete mLogStream;
  }
}

BEGIN_NONAMESPACE
#if 0
void
cg_step1(AigNode* node,
	 vector<ymuint32>& marks,
	 list<AigNode*>& node_list)
{
  if ( marks.size() <= node->id() ) {
    marks.resize(node->id() + 1, 0U);
  }
  if ( marks[node->id()] & 2U ) {
    return;
  }
  if ( marks[node->id()] & 4U ) {
    return;
  }
  cg_step1(node->fanin0(), marks, node_list);
  cg_step1(node->fanin1(), marks, node_list);
  marks[node->id()] |= 4U;
  node_list.push_back(node);
}

void
clear_mark(AigNode* node,
	   vector<ymuint32>& marks)
{
  if ( marks[node->id()] & 2U ) {
    marks[node->id()] &= ~2U;
    return;
  }
  if ( marks[node->id()] & 4U ) {
    marks[node->id()] &= ~4U;
    clear_mark(node->fanin0(), marks);
    clear_mark(node->fanin1(), marks);
  }
}

// node を根とする部分グラフで削除可能なノード数を数える．
int
calc_gain(const AigCut* cut,
	  vector<ymuint32>& marks)
{
  for (size_t i = 0; i < cut->input_num(); ++ i) {
    AigNode* inode = cut->input(i);
    marks[inode->id()] |= 2U;
  }
  list<AigNode*> node_list;
  cg_step1(cut->root(), marks, node_list);
  int gain = 0;
  for (list<AigNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    AigNode* node = *p;
    if ( marks[node->id()] & 1U ) {
      continue;
    }
    bool ok = true;
    if ( node != cut->root() ) {
      const AigEdgeList& fo_list = node->fanout_list();
      for (AigEdgeList::const_iterator q = fo_list.begin();
	   q != fo_list.end(); ++ q) {
	AigEdge* e = *q;
	AigNode* onode = e->to_node();
	if ( (marks[onode->id()] & 4U) == 0U ) {
	  // カットの外側にファンアウトしている．
	  ok = false;
	  break;
	}
      }
    }
    if ( ok ) {
      ++ gain;
    }
  }
  clear_mark(cut->root(), marks);
  return gain;
}

void
mark_nnode(AigNode* node,
	   vector<ymuint32>& marks)
{
  if ( (marks[node->id()] & 2U) == 2U ) {
    return;
  }
  if ( (marks[node->id()] & 4U) == 4U ) {
    return;
  }

  marks[node->id()] |= 4U;
  ASSERT_COND(node->is_and() );
  mark_nnode(node->fanin0(), marks);
  mark_nnode(node->fanin1(), marks);
}

void
mark_dnode_sub(AigNode* node,
	       vector<ymuint32>& marks)
{
  if ( marks.size() <= node->id() ) {
    marks.resize(node->id() + 1, 0U);
  }
  if ( (marks[node->id()] & 2U) == 2U ) {
    return;
  }
  if ( (marks[node->id()] & 4U) == 4U ) {
    return;
  }
  if ( (marks[node->id()] & 1U) == 1U ) {
    return;
  }
  marks[node->id()] |= 1U;
  ASSERT_COND(node->is_and() );
  mark_dnode_sub(node->fanin0(), marks);
  mark_dnode_sub(node->fanin1(), marks);
}

void
clear_nnode(AigNode* node,
	    vector<ymuint32>& marks)
{
  if ( marks.size() <= node->id() ) {
    marks.resize(node->id() + 1, 0U);
  }
  if ( (marks[node->id()] & 2U) == 2U ) {
    marks[node->id()] &= ~2U;
    return;
  }
  if ( (marks[node->id()] & 4U) == 4U ) {
    marks[node->id()] &= ~4U;
    ASSERT_COND(node->is_and() );
    clear_nnode(node->fanin0(), marks);
    clear_nnode(node->fanin1(), marks);
  }
}

void
mark_dnode(const AigCut* cut,
	   AigNode* new_node,
	   vector<ymuint32>& marks)
{
  for (ymuint i = 0; i < cut->input_num(); ++ i) {
    AigNode* inode = cut->input(i);
    marks[inode->id()] |= 2U;
  }
  if ( new_node ) {
    mark_nnode(new_node, marks);
  }
  mark_dnode_sub(cut->root(), marks);
  if ( new_node ) {
    clear_nnode(new_node, marks);
  }
}
#endif
END_NONAMESPACE

// @brief local rewrite を行う．
void
LrMgr::local_rewrite(AigMgr& aig_mgr)
{
#if 0
  ymuint nn = aig_mgr.node_num();

  vector<AigNode*> node_array(na);
  for (ymuint i = 0; i < na; ++ i) {
    AigNode* node = network.sorted_andnode(i);
    node_array[i] = node;
  }
  vector<ymuint32> marks(network.max_node_id(), 0U);
  list<pair<AigNode*, AigHandle> > subst_pair;
  for (ymuint i = 0; i < na; ++ i) {
    AigNode* node = node_array[i];
    int best_gain = 0;
    const AigCut* best_cut = NULL;
    AigTemplate best_templ;
    const AigCutList& clist = ch.cut_list(node);
    for (AigCutList::const_iterator p = clist.begin();
	 p != clist.end(); ++ p) {
      const AigCut* cut = *p;
      bool ok = true;
      for (size_t i = 0; i < cut->input_num(); ++ i) {
	AigNode* inode = cut->input(i);
	if ( marks[inode->id()] & 1U ) {
	  // inode は削除されている．
	  ok = false;
	  break;
	}
      }
      if ( !ok ) {
	continue;
      }

      ymuint32 pat = cut->lf_vector();
      AigTemplate templ;
      bool stat = find_aig(cut->input_num(), pat, templ);
      ASSERT_COND(stat );
      int gain = calc_gain(cut, marks) - templ.cost();
      if ( best_gain < gain ) {
	best_gain = gain;
	best_cut = cut;
	best_templ = templ;
      }
    }
    if ( best_gain > 0 ) {
      if ( debug ) {
	cout << "BestGain = " << best_gain << endl;
	cout << "Cut = ";
	best_cut->dump(cout);
	cout << endl;
	cout << "Template = ";
	best_templ.dump(cout);
	cout << endl;
      }
      vector<AigHandle> inputs(best_cut->input_num());
      for (size_t i = 0; i < best_cut->input_num(); ++ i) {
	inputs[i] = AigHandle(best_cut->input(i), false);
      }

      AigHandle new_handle = best_templ.make_aig(manip, inputs);
      subst_pair.push_back(make_pair(node, new_handle));
      mark_dnode(best_cut, new_handle.node(), marks);
    }
  }
  for (list<pair<AigNode*, AigHandle> >::iterator p = subst_pair.begin();
       p != subst_pair.end(); ++ p) {
    AigNode* node = p->first;
    AigHandle new_handle = p->second;
    manip.node_subst(node, new_handle.node(), new_handle.inv());
  }

  manip.clean_up();
#endif
}

// 与えられた関数に対する AigTemplate を求める．
bool
LrMgr::find_aig(ymuint ni,
		ymuint32 pat,
		AigTemplate& templ)
{
  if ( ni == 2 ) {
    templ = aig2table[pat];
    return true;
  }
  if ( ni == 3 ) {
    templ = aig3table[pat];
    return true;
  }
  if ( ni == 4 ) {
    ymuint16 perm;
    ymuint16 cpat = cannonical4(pat, perm);
    unordered_map<ymuint32, ymuint8>::const_iterator p = npn4map.find(cpat);
    ASSERT_COND(p != npn4map.end() );
    ymuint id = p->second;
    templ = aig4table[id];
    templ.xform(perm);
    return true;
  }
  return false;
}

// @brief ログレベルを設定する．
void
LrMgr::set_loglevel(int level)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
void
LrMgr::set_logstream(ostream* out)
{
  mLogStream = out;
}

END_NAMESPACE_YM_AIG
