
/// @file DtpgSatBaseM.cc
/// @brief DtpgSatBaseM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseM.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
DtpgSatBaseM::DtpgSatBaseM(const string& sat_type,
			   const string& sat_option,
			   ostream* sat_outp,
			   const TpgNetwork& network,
			   BackTracer& bt,
			   DetectOp& dop,
			   UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, network, bt, dop, uop),
  mDone(network.max_node_id(), false)
{
}

// @brief デストラクタ
DtpgSatBaseM::~DtpgSatBaseM()
{
}

// @brief テスト生成を行なう．
// @param[in] f_tgt 対象の故障
void
DtpgSatBaseM::run_single(TpgFault* f_tgt)
{
  run_multi(vector<TpgFault*>(1, f_tgt));
}

// @brief 故障に関係するノードのリストを作る．
// @param[in] flist 故障のリスト
// @param[out] fnode_list 故障に関係するノードのリスト
void
DtpgSatBaseM::make_fnode_list(const vector<TpgFault*>& flist,
			      vector<TpgNode*>& fnode_list)
{
  ymuint nf = flist.size();
  fnode_list.clear();
  fnode_list.reserve(nf);
  for (ymuint i = 0; i < nf; ++ i) {
    TpgFault* f = flist[i];
    TpgNode* node = f->node();
    if ( !mDone[node->id()] ) {
      mDone[node->id()] = true;
      fnode_list.push_back(node);
    }
  }
}

// @brief fnode の情報をクリアする．
void
DtpgSatBaseM::clear_fnode_list(const vector<TpgNode*>& fnode_list)
{
  ymuint nf = fnode_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    TpgNode* node = fnode_list[i];
    node->clear_oifvar();
    mDone[node->id()] = false;
  }
}

END_NAMESPACE_YM_SATPG
