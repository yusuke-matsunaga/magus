
/// @file lutmap/CutResubImpl.cc
/// @brief CutResubImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "CutResub.h"
#include "CutResubImpl.h"
#include "SbjGraph.h"
#include "Cut.h"
#include "CutHolder.h"
#include "MapRecord.h"
#include "CrNode.h"


//#define DEBUG_UPDATE

BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス CutResub
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CutResub::CutResub(
) : mImpl{new CutResubImpl}
{
}

// @brief デストラクタ
CutResub::~CutResub()
{
}

// @brief カットの置き換えを行って LUT 数の削減を行う．
void
CutResub::operator()(
  const SbjGraph& sbjgraph,
  const CutHolder& cut_holder,
  MapRecord& maprec,
  int slack
)
{
  mImpl->resub(sbjgraph, cut_holder, maprec, slack);
}


//////////////////////////////////////////////////////////////////////
// クラス CutResubImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
CutResubImpl::CutResubImpl()
{
}

// デストラクタ
CutResubImpl::~CutResubImpl()
{
  for ( auto node: mGarbageList ) {
    delete node;
  }
}

// @brief カットの置き換えを行って LUT 数の削減を行う．
void
CutResubImpl::resub(
  const SbjGraph& sbjgraph,
  const CutHolder& cut_holder,
  MapRecord& maprec,
  int slack
)
{
  // 作業領域の初期化(mHeap はあとで)
  SizeType n = sbjgraph.node_num();
  mNodeArray.resize(n);
  for ( SizeType i = 0; i < n; ++ i ) {
    mNodeArray[i] = nullptr;
  }
  mHasLevelConstr = (slack >= 0);
  SizeType max_size = sbjgraph.level();
  mGQ.init(max_size);
  mLQ.init(max_size);
  mRQ.init(max_size);

  // 外部入力ノードの対応付けを行う．
  SizeType ni = sbjgraph.input_num();
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto sbjnode = sbjgraph.input(i);
    auto node = alloc_node();
    mNodeArray[sbjnode->id()] = node;
    node->set_sbjnode(sbjnode);
  }

  // 外部出力からバックトレースを行う．
  SizeType no = sbjgraph.output_num();
  for ( SizeType i = 0; i < no; ++ i) {
    auto onode = sbjgraph.output(i);
    auto sbjnode = onode->output_fanin();
    if ( sbjnode && !sbjnode->is_input() ) {
      back_trace(sbjnode, maprec, nullptr);
    }
  }

  SizeType nl = sbjgraph.logic_num();

  // 置き換えのための初期計算を行う．
  {
    // 現在のカットの根のノードを root_list に格納
    // 代理カットを求める．
    vector<CrNode*> root_list;
    root_list.reserve(sbjgraph.node_num());
    SizeType max_level = 0;
    for ( SizeType i = 0; i < nl; ++ i ) {
      auto node = sbjgraph.logic(i);
      auto crnode = mNodeArray[node->id()];
      if ( crnode == nullptr ) continue;

      root_list.push_back(crnode);

      // crnode のレベルを求める．
      SizeType level = 0;
      auto cut = crnode->cut();
      SizeType ni = cut->input_num();
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto inode = cut_input(cut, i);
	if ( inode->is_input() ) continue;
	SizeType level1 = inode->level();
	if ( level < level1 ) {
	  level = level1;
	}
      }
      ++ level;
      crnode->set_level(level);
      if ( max_level < level ) {
	max_level = level;
      }

      // 代理カットを求める．
      auto& cut_list = cut_holder.cut_list(node);
      for( auto cut1: cut_list ) {
	if ( cut1 == cut ) continue;
	bool ok = true;
	SizeType ni = cut1->input_num();
	for ( SizeType i = 0; i < ni; ++ i ) {
	  auto inode = cut_input(cut1, i);
	  if ( inode == nullptr ) {
	    ok = false;
	    break;
	  }
	}
	if ( ok ) {
	  crnode->mAltCutList.push_back(cut1);
	}
      }
    }

    // ゲインの計算を行い，置き換え可能なノードをヒープにつむ．
    mHeap.init(root_list.size());
    for ( auto node: root_list ) {
      node->set_gain(calc_gain(node));
      if ( check_structure(node) ) {
	mHeap.put(node);
      }
    }

    if ( mHasLevelConstr ) {
      // 要求レベルの計算を行う．
      SizeType poreq = max_level + slack;
      for ( auto node: root_list ) {
	if ( node->fanout_list().empty() ) {
	  if ( node->is_output() ) {
	    node->set_req_level(poreq);
	  }
	}
	else {
	  const vector<CrNode*>& folist = node->fanout_list();
	  SizeType req = folist.front()->req_level();
	  for ( auto fonode: folist ) {
	    SizeType r1 = fonode->req_level();
	    if ( req > r1 ) {
	      req = r1;
	    }
	  }
	  node->set_req_level(req - 1);
	}
      }
    }
  }

  // 改善ループ
  vector<const Cut*> subst_list;
  if ( mHasLevelConstr ) {
#if 0
    for ( ; ; ) {
      bool changed = false;
      for ( SizeType i = 0; i < nl; ++ i ) {
	auto node = sbjgraph.logic(nl - i - 1);
	auto crnode = mNodeArray[node->id()];
	if ( crnode == nullptr || crnode->is_output() ) continue;
	if ( find_subst2(crnode, subst_list) ) {
	  update(crnode, subst_list);
	  changed = true;
	}
      }
      if ( !changed ) {
	break;
      }
    }
#else
    for ( ; ; ) {
      auto node = mHeap.get();
      if ( node == nullptr ) {
	break;
      }
      if ( node->gain() == 0 ) continue;
      if ( find_subst2(node, subst_list) ) {
	update(node, subst_list);
      }
    }
#endif
  }
  else {
    for ( ; ; ) {
      auto node = mHeap.get();
      if ( node == nullptr ) {
	break;
      }
      if ( node->gain() == 0) continue;
      if ( find_subst(node, subst_list) ) {
	// 情報の更新
	update(node, subst_list);
      }
    }
  }

  // 最終的なカットを maprec にコピーする．
  // 作業領域をクリアする．
  for ( SizeType i = 0; i < n; ++ i ) {
    auto node = mNodeArray[i];
    if ( node ) {
      maprec.set_cut(node->sbjnode(), node->cut());
      free_node(node);
    }
  }
  mNodeArray.clear();
}

// node の最適カットを選ぶ．
void
CutResubImpl::back_trace(
  const SbjNode* sbjnode,
  MapRecord& maprec,
  CrNode* from
)
{
  auto node = mNodeArray[sbjnode->id()];
  if ( node == nullptr ) {
    node = alloc_node();
    mNodeArray[sbjnode->id()] = node;
    node->set_sbjnode(sbjnode);
    auto cut = maprec.get_cut(sbjnode);
    ASSERT_COND(cut != nullptr );
    node->set_cut(cut);

    // ファンインのノードのカットを選ぶ．
    // 同時にファンアウトリストを構築する．
    for ( SizeType i = 0; i < cut->input_num(); ++ i ) {
      auto inode = cut->input(i);
      if ( !inode->is_input() ) {
	back_trace(inode, maprec, node);
      }
    }
  }
  if ( from ) {
    node->add_fanout(from);
  }
}

// ゲインの計算を行う．
SizeType
CutResubImpl::calc_gain(
  CrNode* node
)
{
  if ( node->is_output() ) return 0;

  auto cut = node->cut();
  SizeType ni = cut->input_num();
  SizeType gain = 1;
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto inode = cut_input(cut, i);
    if ( !inode->is_input() && inode->fanout_list().size() == 1 ) {
      gain += inode->gain();
    }
  }
  return gain;
}

// 構造のみで置き換えが可能かどうか判断する．
bool
CutResubImpl::check_structure(
  CrNode* node
)
{
  if ( node->is_output() ) {
    return false;
  }

  auto& fo_list = node->fanout_list();
  for( auto fo: fo_list ) {
    bool found = false;
    auto& cut_list = fo->mAltCutList;
    for ( auto cut: cut_list ) {
      SizeType ni = cut->input_num();
      bool ok = true;
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto inode = cut_input(cut, i);
	if ( inode == node ) {
	  ok = false;
	  break;
	}
      }
      if ( ok ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      return false;
    }
  }
  return true;
}

// node を冗長にする置き換えカットを求める．
bool
CutResubImpl::find_subst(
  CrNode* node,
  vector<const Cut*>& subst_list
)
{
  subst_list.clear();
  auto& fo_list = node->fanout_list();
  for( auto fo: fo_list ) {
    bool found = false;
    auto& cut_list = fo->mAltCutList;
    for ( auto cut: cut_list ) {
      SizeType ni = cut->input_num();
      bool ok = true;
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto inode = cut_input(cut, i);
	if ( inode == nullptr || inode == node ) {
	  ok = false;
	  break;
	}
      }
      if ( ok ) {
	found = true;
	subst_list.push_back(cut);
	break;
      }
    }
    if ( !found ) {
      return false;
    }
  }
  return true;
}

BEGIN_NONAMESPACE

struct CrNodeLt
{
  bool
  operator()(
    CrNode* node1,
    CrNode* node2
  )
  {
    return node1->sbjnode()->level() < node2->sbjnode()->level();
  }
};

END_NONAMESPACE

// レベルを考慮しつつ node を冗長にする置き換えカットを求める．
bool
CutResubImpl::find_subst2(
  CrNode* node,
  vector<const Cut*>& subst_list
)
{
  subst_list.clear();

  // 現在処理中のノードを「ロック」しておく
  auto& fo_list = node->fanout_list();
  for( auto fo: fo_list ) {
    fo->lock();
  }

  // 処理順を SbjNode のレベルの低い順にする．
  // こうすることによって fo が他のノードをファンインに持つ場合でも
  // ただしくレベルの計算が行える．
  bool ans = true;
  vector<CrNode*> tmp_list(fo_list.begin(), fo_list.end());
  sort(tmp_list.begin(), tmp_list.end(), CrNodeLt());
  for( auto fo: tmp_list ) {
    const Cut* best_cut = nullptr;
    SizeType best_level = 0;
    auto& cut_list = fo->mAltCutList;
    for ( auto cut: cut_list ) {
      SizeType ni = cut->input_num();
      bool ok = true;
      SizeType level = 0;
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto inode = cut_input(cut, i);
	if ( inode == nullptr || inode == node ) {
	  ok = false;
	  break;
	}
	// 現在処理中のノードの場合 level() は使えない．
	SizeType level1 = inode->is_locked() ? inode->mTmpLevel : inode->level();
	if ( level < level1 ) {
	  level = level1;
	}
      }
      level += 1;
      if ( !ok || level > fo->req_level() ) {
	continue;
      }

      if ( best_cut == nullptr || best_level > level ) {
	best_cut = cut;
	best_level = level;
      }
    }
    if ( best_cut ) {
      subst_list.push_back(best_cut);
      fo->mTmpLevel = best_level;
    }
    else {
      ans =false;
      break;
    }
  }

  for( auto fo: fo_list ) {
    fo->unlock();
  }

  return ans;
}

void
CutResubImpl::update(
  CrNode* node,
  const vector<const Cut*>& subst_list
)
{
#ifdef DEBUG_UPDATE
  cout << "update at " << node->sbjnode()->id_str()
       << " begins" << endl;
#endif

  mGQ.clear();
  mLQ.clear();
  mRQ.clear();

  // カットの置き換えを行う．
  // 古いカットは mDeletedCuts に入れる．
  mDeletedCuts.clear();
  for ( auto new_cut: subst_list ) {
    auto root = cut_root(new_cut);
    auto old_cut = root->cut();
    subst_cut_fanouts(root, old_cut, new_cut);
    root->set_cut(new_cut);

    mDeletedCuts.push_back(old_cut);
    put_lq(root);

#ifdef DEBUG_UPDATE
    cout << "subst cut at " << root->sbjnode()->id_str() << endl;
    old_cut->dump(cout);
    new_cut->dump(cout);
    cout << endl;
#endif
  }

  // mDeletedCuts に含まれるカットの入力が他にファンアウトしていなければ
  // そのカットも削除する．
  mDeletedNodes.clear();
  for ( auto cut: mDeletedCuts ) {
    SizeType ni = cut->input_num();
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto inode = cut_input(cut, i);
      if ( inode->is_input() ) continue;
      if ( !inode->is_output() &&
	   !inode->deleted() &&
	   inode->fanout_list().empty() ) {
	inode->set_deleted();
	mDeletedNodes.push_back(inode);
      }
    }
  }
  while ( !mDeletedNodes.empty() ) {
    auto crnode = mDeletedNodes.back();
    mDeletedNodes.pop_back();

#ifdef DEBUG_UPDATE
    cout << " remove cut at " << crnode->sbjnode()->id_str() << endl;
#endif

    auto cut = crnode->cut();
    SizeType ni = cut->input_num();
    for ( SizeType i = 0; i < ni; ++ i ) {
      auto inode = cut_input(cut, i);
      if ( inode->is_input() ) continue;
      inode->delete_fanout(crnode);
      put_rq(inode);
      if ( !inode->is_output() &&
	   !inode->deleted() &&
	   inode->fanout_list().empty() ) {
	inode->set_deleted();
	mDeletedNodes.push_back(inode);
      }
    }
    mNodeArray[crnode->sbjnode()->id()] = nullptr;
    mHeap.remove(crnode);
    free_node(crnode);
  }

  // ゲインの再計算
  while ( mGQ.num() > 0 ) {
    auto node = get_gq();
    if ( node->deleted() ) {
      continue;
    }
    SizeType new_gain = calc_gain(node);
    if ( node->gain() != new_gain ) {
      mHeap.update(node, new_gain);
      auto& folist = node->fanout_list();
      if ( folist.size() == 1 ) {
	auto fo = folist[0];
	put_gq(fo);
      }
    }
  }

  if ( mHasLevelConstr ) {
    // レベルの再計算
    while ( mLQ.num() > 0 ) {
      auto node = get_lq();
      auto cut = node->cut();
      SizeType max_level = 0;
      SizeType ni = cut->input_num();
      for ( SizeType i = 0; i < ni; ++ i ) {
	auto inode = cut_input(cut, i);
	SizeType level = inode->level();
	if ( max_level < level ) {
	  max_level = level;
	}
      }
      max_level += 1;
      if ( node->level() != max_level ) {
	node->set_level(max_level);
	auto& fo_list = node->fanout_list();
	for ( auto fo: fo_list ) {
	  put_lq(fo);
	}
      }
    }

    // 要求レベルの再計算
    while ( mRQ.num() > 0 ) {
      auto node = get_rq();
      if ( node->deleted() ) {
	continue;
      }
      auto& fo_list = node->fanout_list();
#if 0
      if ( fo_list.empty() ) continue;
#endif
      SizeType min_req = fo_list.front()->req_level();
      for ( auto fo: fo_list ) {
	SizeType req = fo->req_level();
	if ( min_req > req ) {
	  min_req = req;
	}
      }
      min_req -= 1;
      if ( node->req_level() != min_req ) {
	node->set_req_level(min_req);
	auto cut = node->cut();
	SizeType ni = cut->input_num();
	for ( SizeType i = 0; i < ni; ++ i ) {
	  auto inode = cut_input(cut, i);
	  if ( !inode->is_input() ) {
	    put_rq(inode);
	  }
	}
      }
    }
  }

#ifdef DEBUG_UPDATE
  cout << "update ends" << endl << endl;
#endif
}

// node のカットを old_cut から new_cut に置き換える．
void
CutResubImpl::subst_cut_fanouts(
  CrNode* node,
  const Cut* old_cut,
  const Cut* new_cut
)
{
  SizeType old_ni = old_cut->input_num();
  SizeType new_ni = new_cut->input_num();

  // old/new フラグをつける．
  for ( SizeType i = 0; i < old_ni; ++ i ) {
    auto inode = cut_input(old_cut, i);
    inode->set_oldmark();
  }
  for ( SizeType i = 0; i < new_ni; ++ i ) {
    auto inode = cut_input(new_cut, i);
    inode->set_newmark();
  }

  // 古いファンインで新しいファンインに含まれていないものの処理
  for ( SizeType i = 0; i < old_ni; ++ i ) {
    auto inode = cut_input(old_cut, i);
    if ( inode->is_input() || inode->newmark() ) continue;

    inode->delete_fanout(node);
    auto& fo_list = inode->fanout_list();
    if ( fo_list.size() == 1 ) {
      auto fo = fo_list[0];
      put_gq(fo);
    }
    put_rq(inode);
  }

  // 新しいファンインで古いファンインに含まれていないものの処理
  for ( SizeType i = 0; i < new_ni; ++ i ) {
    auto inode = cut_input(new_cut, i);
    if ( inode->is_input() || inode->oldmark() ) continue;

    if ( inode->add_fanout(node) ) {
      auto& fo_list = inode->fanout_list();
      if ( fo_list.size() == 2 ) {
	auto fo = fo_list[0];
	put_gq(fo);
      }
      put_gq(node);
      put_rq(inode);
    }
  }

  // old/new フラグを消す．
  for ( SizeType i = 0; i < old_ni; ++ i ) {
    auto inode = cut_input(old_cut, i);
    inode->clear_oldmark();
  }
  for ( SizeType i = 0; i < new_ni; ++ i ) {
    auto inode = cut_input(new_cut, i);
    inode->clear_newmark();
  }
}

// ゲイン計算用のキューにつむ．
void
CutResubImpl::put_gq(
  CrNode* node
)
{
  if ( !node->is_output() && !node->in_GQ() ) {
    node->set_GQ();
    mGQ.put(node);
  }
}

// ゲイン計算用のキューから取り出す．
CrNode*
CutResubImpl::get_gq()
{
  auto node = mGQ.getmin();
  node->clear_GQ();
  return node;
}

// レベル計算用のキューにつむ．
void
CutResubImpl::put_lq(
  CrNode* node
)
{
  if ( !node->in_LQ() ) {
    node->set_LQ();
    mLQ.put(node);
  }
}

// レベル計算用のキューから取り出す．
CrNode*
CutResubImpl::get_lq()
{
  auto node = mLQ.getmin();
  node->clear_LQ();
  return node;
}

// 要求レベル計算用のキューにつむ．
void
CutResubImpl::put_rq(
  CrNode* node
)
{
  if ( !node->in_RQ() ) {
    node->set_RQ();
    mRQ.put(node);
  }
}

// 要求レベル計算用のキューから取り出す．
CrNode*
CutResubImpl::get_rq()
{
  auto node = mRQ.getmin();
  node->clear_RQ();
  return node;
}

// カットの根に対応するノードを取り出す．
CrNode*
CutResubImpl::cut_root(
  const Cut* cut
)
{
  return mNodeArray[cut->root()->id()];
}

// カットの入力に対応するノードを取り出す．
CrNode*
CutResubImpl::cut_input(
  const Cut* cut,
  SizeType pos
)
{
  return mNodeArray[cut->input(pos)->id()];
}

// CrNode を確保する．
CrNode*
CutResubImpl::alloc_node()
{
  if ( mGarbageList.empty() ) {
    return new CrNode;
  }
  else {
    auto node = mGarbageList.back();
    mGarbageList.pop_back();
    return node;
  }
}

// CrNode を解放する．
void
CutResubImpl::free_node(
  CrNode* node
)
{
  mGarbageList.push_back(node);
}

END_NAMESPACE_LUTMAP
