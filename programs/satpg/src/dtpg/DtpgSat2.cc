
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

bool debug_fault_analysis = false;

Dtpg*
new_DtpgSat2()
{
  return new DtpgSat2();
}

// @brief コンストラクタ
DtpgSat2::DtpgSat2()
{
  mNetwork = NULL;
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

  mNetwork = &tgnetwork;
  mMaxId = mNetwork->node_num();

  switch ( po_mode ) {
  case kDtpgPoNone:
    // PO 分割を行わないモード

    mNetwork->activate_all();

    dtpg1(bt, dop, uop);

    break;

  case kDtpgPoInc:
    { // 正順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint po_pos = 0; po_pos < no; ++ po_pos) {
	mNetwork->activate_po(po_pos);

	dtpg1(bt, dop, uop);
      }
    }
    break;

  case kDtpgPoDec:
    { // 逆順で PO を選び分割するモード
      ymuint no = mNetwork->output_num2();
      for (ymuint i = 0; i < no; ++ i) {
	ymuint po_pos = no - i - 1;
	mNetwork->activate_po(po_pos);

	dtpg1(bt, dop, uop);
      }
    }
    break;
  }

  mSatEngine.get_stats(stats);
}

// @brief activate された部分回路に対してテスト生成を行う．
// @param[in] bt バックトレーサー
void
DtpgSat2::dtpg1(BackTracer& bt,
		DetectOp& dop,
		UntestOp& uop)
{
  ymuint nn = mNetwork->active_node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    TpgNode* node = mNetwork->active_node(i);
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
    mSatEngine.run(vector<TpgFault*>(1, fault), mMaxId, bt, dop, uop);
 }
}

END_NAMESPACE_YM_SATPG
