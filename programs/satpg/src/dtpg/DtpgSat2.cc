
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
// @param[in] option_str オプション文字列
// @param[in] bt バックトレーサー
// @param[in] dop_list DetectOp のリスト
// @param[in] uop_list UntestOp のリスト
// @param[in] stats 結果を格納する構造体
void
DtpgSat2::run(TpgNetwork& tgnetwork,
	      DtpgMode mode,
	      tDtpgPoMode po_mode,
	      const string& option_str,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop,
	      DtpgStats& stats)
{
  mSatEngine.set_option(option_str);
  mSatEngine.clear_stats();

  mMaxId = tgnetwork.max_node_id();

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
      dtpg_single(f0, tgnetwork, bt, dop, uop);

      TpgFault* f1 = node->input_fault(1, j);
      dtpg_single(f1, tgnetwork, bt, dop, uop);
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
    mSatEngine.run(fault, fault->node(), fault->val(), mMaxId, bt, dop, uop);
 }
}

// @brief 一つの故障に対してテストパタン生成を行う．
// @param[in] f 故障
void
DtpgSat2::dtpg_single(TpgFault* fault,
		      TpgNetwork& network,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop)
{
  if ( fault != NULL &&
       fault->is_rep() &&
       fault->status() != kFsDetected &&
       !fault->is_skip() ) {
    network.begin_fault_injection();
    TpgNode* fnode = network.inject_fnode(fault->node(), fault->pos());
    mSatEngine.run(fault, fnode, fault->val(), network.max_node_id(), bt, dop, uop);
    network.end_fault_injection();
 }
}

END_NAMESPACE_YM_SATPG
