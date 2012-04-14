
/// @file RlImp.cc
/// @brief RlImp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "RlImp.h"
#include "StrImp.h"
#include "ImpMgr.h"
#include "ImpNode.h"
#include "ImpInfo.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス RlImp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RlImp::RlImp()
{
  mLevel = 1;
}

// @brief デストラクタ
RlImp::~RlImp()
{
}

// @brief ラーニングのレベルを設定する．
void
RlImp::set_learning_level(ymuint level)
{
  mLevel = level;
}

// @brief ネットワーク中の間接含意を求める．
// @param[in] imp_mgr マネージャ
// @param[in] direct_imp 直接含意のリスト
// @param[in] imp_info 間接含意のリスト
void
RlImp::learning(ImpMgr& imp_mgr,
		const ImpInfo& direct_imp,
		ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  imp_info.set_size(n);

  vector<vector<ImpVal> > imp_list_array(n * 2);
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    if ( imp_mgr.is_const(src_id) ) {
      continue;
    }

    ImpNode* src_node = imp_mgr.node(src_id);

    // src_node に値を割り当てる．
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      vector<ImpVal> imp_list;
      bool ok = make_all_implication(imp_mgr, src_node, src_val, mLevel, imp_list);
      if ( ok ) {
	for (vector<ImpVal>::iterator p = imp_list.begin();
	     p != imp_list.end(); ++ p) {
	  const ImpVal& imp = *p;
	  ymuint dst_id = imp.id();
	  ymuint dst_val = imp.val();
	  imp_list_array[src_id * 2 + src_val].push_back(ImpVal(dst_id, dst_val));
	  imp_list_array[dst_id * 2 + (dst_val ^ 1)].push_back(ImpVal(src_id, src_val ^ 1));
	}
      }
      else {
	// 単一の値割り当てが失敗するということは逆の値で固定されている．
	imp_mgr.set_const(src_id, src_val ^ 1);
	cout << "Node#" << src_id << " is const-" << (src_val ^ 1) << endl;
      }
    }
  }

  imp_info.set(imp_list_array, direct_imp);
}

// @brief recursive learning を行なう．
// @param[in] imp_mgr ImpMgr
// @param[in] node ノード
// @param[in] val 値
// @param[in] level レベル
// @param[in] imp_list 含意のリスト
bool
RlImp::make_all_implication(ImpMgr& imp_mgr,
			    ImpNode* node,
			    ymuint val,
			    ymuint level,
			    vector<ImpVal>& imp_list)
{
  if ( debug ) {
    cout << "make_all_implication(Node#" << node->id()
	 << ": " << val << " @level#" << level << ")" << endl;
  }

  ymuint n2 = imp_mgr.node_num();

  imp_list.clear();
  bool ok = imp_mgr.assert(node, val, imp_list);

  if ( debug ) {
    cout << "direct implications {" << endl;
    for (vector<ImpVal>::iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      ymuint dst_id = p->id();
      ymuint dst_val = p->val();
      cout << "  ==> Node#" << dst_id << ": " << dst_val << endl;
    }
    cout << "}" << endl;
  }

  if ( ok ) {
    if ( level > 0 ) {
      vector<ImpNode*> unode_list;
      imp_mgr.get_unodelist(unode_list);
      // 共通の割り当てを調べるための配列
      vector<bool> vmark(n2, false);
      for (vector<ImpNode*>::iterator p = unode_list.begin();
	   p != unode_list.end(); ++ p) {
	ImpNode* unode = *p;

	if ( debug ) {
	  cout << "Unode: Node#" << unode->id() << endl;
	}

	ymuint np = unode->justification_num();
	bool first = true;
	vector<ymuint> common_val(n2, 2);
	vector<ymuint> common_list;
	for (ymuint i = 0; i < np; ++ i) {
	  ImpDst imp = unode->get_justification(i);
	  ImpNode* inode = imp.node();
	  ymuint ival = imp.val();

	  if ( debug ) {
	    cout << "  Inode: Node#" << inode->id()
		 << ": " << ival << endl;
	  }

	  vector<ImpVal> imp_list1;
	  bool ok1 = make_all_implication(imp_mgr, inode, ival, level - 1,
					  imp_list1);
	  if ( ok1 ) {
	    if ( first ) {
	      first = false;
	      for (vector<ImpVal>::iterator p = imp_list1.begin();
		   p != imp_list1.end(); ++ p) {
		const ImpVal& imp = *p;
		ymuint dst_id = imp.id();
		ymuint val = imp.val();
		if ( vmark[dst_id] ) {
		  common_val[dst_id] = 2;
		}
		else {
		  common_val[dst_id] = val;
		  common_list.push_back(dst_id);
		}
	      }
	    }
	    else {
	      vector<bool> vmark1(n2, false);
	      for (vector<ImpVal>::iterator p = imp_list1.begin();
		   p != imp_list1.end(); ++ p) {
		const ImpVal& imp = *p;
		ymuint dst_id = imp.id();
		ymuint val = imp.val();
		if ( common_val[dst_id] != val ) {
		  common_val[dst_id] = 2;
		}
		vmark1[dst_id] = true;
	      }
	      for (vector<ymuint>::iterator p = common_list.begin();
		   p != common_list.end(); ++ p) {
		ymuint dst_id = *p;
		if ( !vmark1[dst_id] ) {
		  common_val[dst_id] = 2;
		}
	      }
	    }
	  }
	}
	ymuint nc = common_list.size();
	for (ymuint i = 0; i < nc; ++ i) {
	  ymuint dst_id = common_list[i];
	  if ( common_val[dst_id] == 2 ) continue;
	  imp_list.push_back(ImpVal(dst_id, common_val[dst_id]));

	  if ( debug ) {
	    cout << "  Common Implication: Node#" << dst_id
		 << ": " << common_val[dst_id] << endl;
	  }

	  vmark[dst_id] = true;
	}
	if ( debug ) {
	  cout << "Unode: Node#" << unode->id() << " end" << endl;
	}
      }
    }
  }
  else {
    // 単一の割り当てで矛盾が起こった．
    // node は !val 固定
  }
  imp_mgr.backtrack();

  return ok;
}

END_NAMESPACE_YM_NETWORKS
