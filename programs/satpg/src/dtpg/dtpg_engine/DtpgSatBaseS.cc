
/// @file DtpgSatBaseS.cc
/// @brief DtpgSatBaseS の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseS.h"

#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"
#include "NodeSet.h"
#include "FaultMgr.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
DtpgSatBaseS::DtpgSatBaseS(const string& sat_type,
			   const string& sat_option,
			   ostream* sat_outp,
			   BackTracer& bt,
			   DetectOp& dop,
			   UntestOp& uop) :
  DtpgSat(sat_type, sat_option, sat_outp, bt, dop, uop)
{
}

// @brief デストラクタ
DtpgSatBaseS::~DtpgSatBaseS()
{
}

// @brief テスト生成を行なう．
// @param[in] network 対象のネットワーク
// @param[in] fault_mgr 故障を管理するクラス
// @param[in] stats 結果を格納する構造体
void
DtpgSatBaseS::run(TpgNetwork& network,
		  FaultMgr& fault_mgr,
		  DtpgStats& stats)
{
  clear_stats();

  NodeSet node_set;

  ymuint nn = network.active_node_num();
  ymuint max_id = network.max_node_id();
  for (ymuint i = 0; i < nn; ++ i) {
    const TpgNode* node = network.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    node_set.mark_region(max_id, node);

    ymuint nf = node->fault_num();
    for (ymuint i = 0; i < nf; ++ i) {
      const TpgFault* f = node->fault(i);
      if ( fault_mgr.status(f) != kFsDetected ) {
	run_single(node_set, f);
      }
    }
  }

  get_stats(stats);
}

END_NAMESPACE_YM_SATPG
