
/// @file DtpgSatM.cc
/// @brief DtpgSatM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatM.h"

#include "DtpgStats.h"
#include "NodeSet.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "MvalCnf.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief Multi エンジンを作る．
// @param[in] sat_type SATソルバの種類を表す文字列
// @param[in] sat_option SATソルバに渡すオプション文字列
// @param[in] sat_outp SATソルバ用の出力ストリーム
// @param[in] bt バックトレーサー
// @param[in] dop パタンが求められた時に実行されるファンクタ
// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop)
{
  return new DtpgSatM(sat_type, sat_option, sat_outp, bt, dop, uop);
}

// @brief コンストラクタ
DtpgSatM::DtpgSatM(const string& sat_type,
		   const string& sat_option,
		   ostream* sat_outp,
		   BackTracer& bt,
		   DetectOp& dop,
		   UntestOp& uop) :
  DtpgSatBaseM(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatM::~DtpgSatM()
{
}

// @brief テストパタン生成を行なう．
// @param[in] node_set 対象のノード集合
// @param[in] fnode_list 対象の故障を持つノードのリスト
// @param[in] flist 故障リスト
void
DtpgSatM::run_multi(const NodeSet& node_set,
		    const vector<const TpgNode*>& fnode_list,
		    const vector<const TpgFault*>& flist)
{
  cnf_begin();

  SatEngine engine(sat_type(), sat_option(), sat_outp());

  ymuint nf = flist.size();

  ymuint max_id = node_set.max_id();

  MvalCnf mval_cnf(max_id);

  engine.make_mval_cnf(mval_cnf, flist, fnode_list, node_set);

  cnf_end();

  mMarkArray.clear();
  mMarkArray.resize(max_id, false);

  // 個々の故障に対するテスト生成を行なう．
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* f = flist[i];

#if 0
    if ( f->status() == kFsDetected ) {
      // 他の故障のパタンで検出済みになっている場合がある．
      continue;
    }
#endif

    vector<Literal> assumptions;

    // 該当の故障に対する変数のみ1にする．
    for (ymuint j = 0; j < nf; ++ j) {
      VarId fdvar = mval_cnf.fault_var(j);
      bool inv = (j != i);
      assumptions.push_back(Literal(fdvar, inv));
    }

    const TpgNode* fnode = f->node();

    // 故障ノードの TFO 以外の dlit を0にする．
    mTmpNodeList.clear();
    mTmpNodeList.reserve(node_set.tfo_tfi_size());
    mMarkArray[fnode->id()] = true;
    mTmpNodeList.push_back(fnode);
    for (ymuint rpos = 0; rpos < mTmpNodeList.size(); ++ rpos) {
      const TpgNode* node = mTmpNodeList[rpos];
      ymuint nfo = node->active_fanout_num();
      for (ymuint i = 0; i < nfo; ++ i) {
	const TpgNode* fonode = node->active_fanout(i);
	if ( !mMarkArray[fonode->id()] ) {
	  mMarkArray[fonode->id()] = true;
	  mTmpNodeList.push_back(fonode);
	}
      }
    }
    for (ymuint i = 0; i < node_set.tfo_tfi_size(); ++ i) {
      const TpgNode* node = node_set.tfo_tfi_node(i);
      if ( node_set.tfo_mark(node) && !mMarkArray[node->id()] ) {
	Literal dlit(mval_cnf.dvar(node));
	assumptions.push_back(~dlit);
      }
    }
    for (vector<const TpgNode*>::iterator p = mTmpNodeList.begin();
	 p != mTmpNodeList.end(); ++ p) {
      const TpgNode* node = *p;
      mMarkArray[node->id()] = false;
    }
    mTmpNodeList.clear();

    // dominator ノードの dvar は1でなければならない．
    for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
      Literal dlit(mval_cnf.dvar(node));
      assumptions.push_back(dlit);
    }

    solve(engine, assumptions, f, node_set, mval_cnf.gvar_map(), mval_cnf.fvar_map());
  }
}

END_NAMESPACE_YM_SATPG
