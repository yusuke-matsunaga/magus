
/// @file DtpgSat2.cc
/// @brief DtpgSat2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat2.h"
#include "TpgNetwork.h"
#include "TpgNode.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

Dtpg*
new_DtpgSat2()
{
  return new DtpgSat2();
}

// @brief コンストラクタ
DtpgSat2::DtpgSat2()
{
}

// @brief デストラクタ
DtpgSat2::~DtpgSat2()
{
}

// @brief 使用する SAT エンジンを指定する．
void
DtpgSat2::set_mode(const string& type,
		   const string& option,
		   ostream* outp)
{
  mSatEngine.set_mode(type, option, outp);
}

// @brief テスト生成を行なう．
// @param[in] tpgnetwork 対象のネットワーク
// @param[in] mode メインモード
// @param[in] po_mode PO分割モード
// @param[in] fault_analysis 故障の支配関係を解析する．
// @param[in] bt バックトレーサー
// @param[in] dop_list DetectOp のリスト
// @param[in] uop_list UntestOp のリスト
// @param[in] stats 結果を格納する構造体
void
DtpgSat2::run(TpgNetwork& tgnetwork,
	      DtpgMode mode,
	      tDtpgPoMode po_mode,
	      bool fault_analysis,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop,
	      DtpgStats& stats)
{
  mSatEngine.clear_stats();

  mMaxId = tgnetwork.node_num();

  tgnetwork.activate_all();

  ymuint nn = tgnetwork.active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = tgnetwork.active_node(i);
    if ( node->is_output() ) {
      continue;
    }

    // 入力の故障
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      TpgFault* f0 = node->input_fault(0, j);
      dtpg_single(f0, bt, dop, uop);

      TpgFault* f1 = node->input_fault(1, j);
      dtpg_single(f1, bt, dop, uop);
    }

    // 出力の故障
    TpgFault* f0 = node->output_fault(0);
    dtpg_single(f0, bt, dop, uop);

    TpgFault* f1 = node->output_fault(1);
    dtpg_single(f1, bt, dop, uop);
  }


  mSatEngine.get_stats(stats);
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
void
DtpgSat2::dtpg_single(TpgFault* fault,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop)
{
  if ( fault != NULL &&
       fault->is_rep() &&
       fault->status() != kFsDetected &&
       !fault->is_skip() ) {
    mSatEngine.run(fault, mMaxId, bt, dop, uop);
 }
}

END_NAMESPACE_YM_SATPG
