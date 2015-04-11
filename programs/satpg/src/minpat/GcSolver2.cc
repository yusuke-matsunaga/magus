
/// @file GcSolver2.cc
/// @brief GcSolver2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GcSolver2.h"
#include "GcNode2.h"
#include "GcNodeHeap2.h"
#include "TpgFault.h"
#include "TpgCnf1.h"
#include "TpgCnf2.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

ymuint n_int;
ymuint n_conf;
ymuint n_compat;
ymuint n_triv;

extern
void
verify_suf_list(TpgFault* fault,
		ymuint max_id,
		const NodeValList& suf_list);


//////////////////////////////////////////////////////////////////////
// クラス GcSolver2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcSolver2::GcSolver2()
{
}

// @brief デストラクタ
GcSolver2::~GcSolver2()
{
  for (ymuint i = 0; i < mColList.size(); ++ i) {
    delete mColList[i];
  }
}

// @brief 彩色する．
// @return 彩色数を返す．
ymuint
GcSolver2::coloring(const vector<TpgFault*>& fault_list,
		    const vector<FaultInfo>& fault_info_array,
		    const vector<vector<ymuint> >& input_list_array,
		    ymuint max_node_id)
{
  mMaxId = max_node_id;
  ymuint fault_num = fault_list.size();
  ymuint vectlen = (fault_num + 63) / 64;
  GcNode2* node_array = new GcNode2[fault_num];
  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode2* node = &node_array[i];
    TpgFault* fault = fault_list[i];
    node->init(i, fault, vectlen);
  }

  // dsatur アルゴリズムを用いる．
  // 最初は同時検出故障数の少ない故障を選ぶ．
  GcNode2* min_node = NULL;
  ymuint min_count = 0;
  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode2* node = &node_array[i];
    ymuint fnum = fault_info_array[node->fault()->id()].fnum();
    if ( min_count == 0 || min_count > fnum ) {
      min_count = fnum;
      min_node = node;
    }
  }

  ymuint c0 = new_color(min_node->fault());
  min_node->set_color(c0);

  GcNodeHeap2 node_heap(fault_num);

  ymuint n_compat = 0;
  n_int = 0;
  n_conf = 0;
  n_compat = 0;
  n_triv = 0;

  // min_node と衝突する故障を探す．
  TpgFault* fault0 = min_node->fault();
  for (ymuint i = 0; i < fault_num; ++ i) {
    GcNode2* node = &node_array[i];
    if ( node == min_node ) {
      continue;
    }
    ++ n_compat;
    check_compat(mColList[c0 - 1], node, fault_info_array, input_list_array);
    //node_heap.put_node(node);
  }

#if 0
  // 2: saturation degree が最大の未彩色ノードを選び最小の色番号で彩色する．
  while ( !node_heap.empty() ) {
    GcNode2* max_node = node_heap.get_min();
    cout << "\r                       ";
    cout << "\r   " << node_heap.size() << " | " << mColList.size();
    cout.flush();

    // max_node につけることのできる最小の色番号を求める．
    ymuint min_col = 0;
    for (ymuint c = 0; c < mColList.size(); ++ c) {
      if ( !max_node->check_adj_color(c) ) {
	min_col = c + 1;
	break;
      }
    }
    TpgFault* f1 = max_node->fault();
    if ( min_col == 0 ) {
      // 新しい色を割り当てる．
      min_col = new_color(f1);
      max_node->set_color(min_col);
      for (ymuint i = 0; i < fault_num; ++ i) {
	GcNode2* node = &node_array[i];
	if ( node->color() > 0 ) {
	  continue;
	}
	++ n_compat;
	if ( !check_compat(mColList[min_col - 1], node, fault_info_array, input_list_array) ) {
	  node_heap.update(node);
	}
      }
    }
    else {
      ColInfo* cip0 = mColList[min_col - 1];
      cip0->mFaultList.push_back(f1);
      const NodeValList& suf_list = max_node->suf_list(min_col - 1);
      if ( suf_list.size() == 0 ) {
	TpgCnf1& tpg_cnf = max_node->tpg_cnf(mMaxId);
	const NodeValList& suf_list0 = cip0->mSufList;
	NodeValList suf_list;
	bool stat = tpg_cnf.get_suf_list(suf_list0, suf_list);
	ASSERT_COND( stat );
	cip0->mSufList.merge(suf_list);
      }
      else {
	cip0->mSufList.merge(suf_list);
      }
      max_node->set_color(min_col);
      max_node->clear_cnf();

      vector<GcNode2*> tmp_compat_list = cip0->mCompatList;
      cip0->mCompatList.clear();
      for (ymuint i = 0; i < tmp_compat_list.size(); ++ i) {
	GcNode2* node = tmp_compat_list[i];
	if ( node->color() > 0 ) {
	  continue;
	}
	++ n_compat;
	if ( !check_compat(cip0, node, fault_info_array, input_list_array) ) {
	  node_heap.update(node);
	}
      }
    }
  }
#else
  ymuint remain = fault_num - 1;
  for ( ; ; ) {
    GcNode2* max_node = NULL;
    ymuint max_sat = 0;
    ymuint max_sat2 = 0;
    for (ymuint i = 0; i < fault_num; ++ i) {
      GcNode2* node = &node_array[i];
      if ( node->color() > 0 ) {
	continue;
      }
      ymuint sat = node->sat_degree();
      ymuint pnum = node->pending_num();
      ymuint sat2 = sat + pnum;
      if ( max_node == NULL || max_sat2 < sat2 ||
	   (max_sat2 == sat2 && max_sat < sat) ) {
	max_sat = sat;
	max_sat2 = sat2;
	max_node = node;
      }
    }
    if ( max_node == NULL ) {
      break;
    }

    if ( max_sat < max_sat2 ) {
      // ペンディング要求がある．
      ASSERT_COND( max_node->pending_num() > 0 );
      while ( max_node->pending_num() > 0 ) {
	ymuint cid = max_node->get_check();
	if ( _check_compat(mColList[cid], max_node) ) {
	  if ( max_node->suf_list(cid).size() == 0 ) {
	    // 他の故障に影響を与えない．
	    max_node->set_color(cid + 1);
	    ++ n_triv;
	    break;
	  }
	}
      }
      continue;
    }

    -- remain;
    cout << "\r                       ";
    cout << "\r   " << remain << " | " << mColList.size();
    cout.flush();

    // max_node につけることのできる最小の色番号を求める．
    ymuint min_col = 0;
    for (ymuint c = 0; c < mColList.size(); ++ c) {
      if ( !max_node->check_adj_color(c) ) {
	min_col = c + 1;
	break;
      }
    }
    TpgFault* f1 = max_node->fault();
    if ( min_col == 0 ) {
      // 新しい色を割り当てる．
      min_col = new_color(f1);
      max_node->set_color(min_col);
      for (ymuint i = 0; i < fault_num; ++ i) {
	GcNode2* node = &node_array[i];
	if ( node->color() > 0 ) {
	  continue;
	}
	++ n_compat;
	if ( !check_compat(mColList[min_col - 1], node, fault_info_array, input_list_array) ) {
	  //node_heap.update(node);
	}
      }
    }
    else {
      ColInfo* cip0 = mColList[min_col - 1];
      cip0->mFaultList.push_back(f1);
      const NodeValList& suf_list = max_node->suf_list(min_col - 1);
      if ( suf_list.size() == 0 ) {
	TpgCnf1& tpg_cnf = max_node->tpg_cnf(mMaxId);
	const NodeValList& suf_list0 = cip0->mSufList;
	NodeValList suf_list;
	bool stat = tpg_cnf.get_suf_list(suf_list0, suf_list);
	ASSERT_COND( stat );
	cip0->mSufList.merge(suf_list);
      }
      else {
	cip0->mSufList.merge(suf_list);
      }
      max_node->set_color(min_col);
      max_node->clear_cnf();

      vector<GcNode2*> tmp_compat_list = cip0->mCompatList;
      cip0->mCompatList.clear();
      for (ymuint i = 0; i < tmp_compat_list.size(); ++ i) {
	GcNode2* node = tmp_compat_list[i];
	if ( node->color() > 0 ) {
	  continue;
	}
	++ n_compat;
	if ( !check_compat(cip0, node, fault_info_array, input_list_array) ) {
	  //node_heap.update(node);
	}
      }
    }
  }
#endif

  cout << endl;
  cout << " # of compat check: " << n_compat << endl;
  cout << " # of structual independent: " << n_int << endl;
  cout << " # of conflict test: " << n_conf << endl;
  cout << " # of compatible case: " << n_compat << endl;
  cout << " # of trivial case: " << n_triv << endl;

  if ( false ) { // 検証
    for (ymuint i = 0; i < mColList.size(); ++ i) {
      ColInfo* cip = mColList[i];
      cout << "Color Group#" << cip->mColId << endl;
      for (ymuint j = 0; j < cip->mFaultList.size(); ++ j) {
	TpgFault* f = cip->mFaultList[j];
	cout << "   " << f->str() << endl;
	verify_suf_list(f, mMaxId, cip->mSufList);
      }
    }
  }

  delete [] node_array;

  return mColList.size();
}

// @brief 故障リストを返す．
// @param[in] col 色番号(1が最初)
const vector<TpgFault*>&
GcSolver2::fault_list(ymuint col) const
{
  ASSERT_COND( col <= mColList.size() );
  return mColList[col - 1]->mFaultList;
}

// @brief 十分割当リストを返す．
// @param[in] col 色番号(1が最初)
const NodeValList&
GcSolver2::suf_list(ymuint col) const
{
  ASSERT_COND( col <= mColList.size() );
  return mColList[col - 1]->mSufList;
}

// @brief 新しい色を割り当てる．
ymuint
GcSolver2::new_color(TpgFault* fault)
{
  TpgCnf1 tpg_cnf(string(), string(), NULL);

  tpg_cnf.make_fval_cnf(fault, mMaxId);

  ColInfo* cip = new ColInfo;
  cip->mColId = mColList.size();
  cip->mFaultList.push_back(fault);
  NodeValList suf_list;
  bool stat = tpg_cnf.get_suf_list(NodeValList(), suf_list);
  ASSERT_COND( stat );
  cip->mSufList = suf_list;
  mColList.push_back(cip);
  return cip->mColId + 1;
}

// @brief 故障が両立するか調べる．
bool
GcSolver2::check_compat(ColInfo* cip,
			GcNode2* node,
			const vector<FaultInfo>& fault_info_array,
			const vector<vector<ymuint> >& input_list_array)
{
  if ( true ) {
    HashSet<ymuint> input_hash;
    TpgNode* node1 = node->fault()->node();
    const vector<ymuint>& input_list1 = input_list_array[node1->id()];
    for (ymuint i = 0; i < input_list1.size(); ++ i) {
      input_hash.add(input_list1[i]);
    }
    bool intersect = false;
    for (ymuint i = 0; i < cip->mFaultList.size(); ++i) {
      TpgFault* f = cip->mFaultList[i];
      TpgNode* node2 = f->node();
      const vector<ymuint>& input_list2 = input_list_array[node2->id()];
      for (ymuint j = 0; j < input_list2.size(); ++ j) {
	if ( input_hash.check(input_list2[j]) ) {
	  intersect = true;
	  break;
	}
      }
    }
    if ( !intersect ) {
      // 構造的に独立なので両立する．
      cip->mCompatList.push_back(node);
      node->set_suf_list(cip->mColId, NodeValList());
      ++ n_int;
      return true;
    }
  }
  TpgFault* fault1 = node->fault();

  if ( true ) {
    const FaultInfo& fi1 = fault_info_array[fault1->id()];

    HashSet<ymuint> conf_hash;
    for (ymuint i = 0; i < cip->mFaultList.size(); ++ i) {
      TpgFault* fault2 = cip->mFaultList[i];
      conf_hash.add(fault2->id());
    }
    bool conflict = false;
    for (ymuint i = 0; i < fi1.mConflictList.size(); ++ i) {
      if ( conf_hash.check(fi1.mConflictList[i]) ) {
	conflict = true;
	break;
      }
    }
    if ( conflict ) {
      node->add_adj_color(cip->mColId);
      ++ n_conf;
      return false;
    }
  }

  node->reg_check(cip->mColId);
  return true;
#if 0
  TpgCnf1& tpg_cnf = node->tpg_cnf(mMaxId);

  const NodeValList& suf_list0 = cip->mSufList;
  NodeValList suf_list;
  if ( tpg_cnf.get_suf_list(suf_list0, suf_list) ) {
    cip->mCompatList.push_back(node);
    node->set_suf_list(cip->mColId, suf_list);
    ++ n_compat;
    return true;
  }
  else {
    node->add_adj_color(cip->mColId);
    return false;
  }
#endif
}


// @brief 両立チェックの本体
bool
GcSolver2::_check_compat(ColInfo* cip,
			 GcNode2* node)
{
  TpgCnf1& tpg_cnf = node->tpg_cnf(mMaxId);

  const NodeValList& suf_list0 = cip->mSufList;
  NodeValList suf_list;
  if ( tpg_cnf.get_suf_list(suf_list0, suf_list) ) {
    cip->mCompatList.push_back(node);
    suf_list.diff(suf_list0);
    node->set_suf_list(cip->mColId, suf_list);
    ++ n_compat;
    return true;
  }
  else {
    node->add_adj_color(cip->mColId);
    return false;
  }
}

//////////////////////////////////////////////////////////////////////
// クラス GcNode2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GcNode2::GcNode2()
{
  mColorSet = NULL;
  mSatDegree = 0;
  mColor = 0;
  mTpgCnf = NULL;
  mPendingNum = 0;
}

// @brief デストラクタ
GcNode2::~GcNode2()
{
  delete [] mColorSet;
}

// @brief 初期化する．
void
GcNode2::init(ymuint id,
	      TpgFault* fault,
	      ymuint vectlen)
{
  mId = id;
  mFault = fault;
  mColorSet = new ymuint64[vectlen];
  for (ymuint i = 0; i < vectlen; ++ i) {
    mColorSet[i] = 0UL;
  }
}

END_NAMESPACE_YM_SATPG
