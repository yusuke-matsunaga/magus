
/// @file EqChecker.cc
/// @brief EqChecker の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "EqChecker.h"

#include "FaultAnalyzer.h"

#include "TpgFault.h"
#include "TvMgr.h"
#include "TestVector.h"
#include "Fsim.h"
#include "DetOp.h"
#include "NodeSet.h"

#include "GvalCnf.h"
#include "FvalCnf.h"
#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス EqChecker
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] analyzer 故障の情報を持つクラス
// @param[in] fsim 故障シミュレータ
// @param[in] tvmgr テストベクタのマネージャ
EqChecker::EqChecker(FaultAnalyzer& analyzer,
		     TvMgr& tvmgr,
		     Fsim& fsim) :
  mAnalyzer(analyzer),
  mTvMgr(tvmgr),
  mFsim(fsim)
{
  mVerbose = mAnalyzer.verbose();
  mMaxNodeId = mAnalyzer.max_node_id();
  mMaxFaultId = mAnalyzer.max_fault_id();
}

// @brief デストラクタ
EqChecker::~EqChecker()
{
}

// @brief verbose フラグを設定する．
// @param[in] verbose 表示を制御するフラグ
void
EqChecker::set_verbose(int verbose)
{
  mVerbose = verbose;
}

// @brief 等価故障の代表故障を求める．
void
EqChecker::get_rep_faults(const vector<const TpgFault*>& src_list,
			  vector<const TpgFault*>& rep_fault_list)
{
  StopWatch local_timer;
  local_timer.start();
  {
    vector<ymuint> elem_list;
    elem_list.reserve(src_list.size());
    for (ymuint i = 0; i < src_list.size(); ++ i) {
      elem_list.push_back(src_list[i]->id());
    }
    mEqSet.init(elem_list);
  }

  do_fsim(src_list);

  USTime fsim_time = local_timer.time();

  ymuint n_check = 0;
  ymuint n_success = 0;

  vector<bool> mark(mMaxFaultId, false);
  ymuint nc = mEqSet.class_num();
  for (ymuint i = 0; i < nc; ++ i) {
    vector<ymuint> elem_list;
    mEqSet.class_list(i, elem_list);
    ymuint n = elem_list.size();
    for (ymuint i1 = 0; i1 < n; ++ i1) {
      ymuint f1_id = elem_list[i1];
      if ( mark[f1_id] ) {
	continue;
      }

      if ( mVerbose > 1 ) {
	cout << "\rEQ:   "
	     << setw(6) << i << " / " << setw(6) << nc
	     << "  " << setw(6) << i1;
      }

      const FaultInfo& fi1 = mAnalyzer.fault_info(f1_id);
      const TpgFault* f1 = fi1.fault();
      rep_fault_list.push_back(f1);

      for (ymuint i2 = i1 + 1; i2 < n; ++ i2) {
	ymuint f2_id = elem_list[i2];
	if ( mark[f2_id] ) {
	  continue;
	}

	const FaultInfo& fi2 = mAnalyzer.fault_info(f2_id);
	const TpgFault* f2 = fi2.fault();
	++ n_check;
	if ( mAnalyzer.check_equivalence(f1, f2) ) {
	  mark[f2_id] = true;
	  ++ n_success;
	}
      }
    }
  }

  local_timer.stop();

  if ( mVerbose > 0 ) {
    if ( mVerbose > 1 ) {
      cout << endl;
    }
    cout << "# original faults:       " << setw(8) << src_list.size() << endl
	 << "# representative faults: " << setw(8) << rep_fault_list.size() << endl
	 << "  # equivalence checks:  " << setw(8) << n_check << endl
	 << "  # sucess:              " << setw(8) << n_success << endl
	 << "  # patterns simulated:  " << setw(8) << mPat << endl
	 << "CPU time:                " << local_timer.time() << endl
	 << "  CPU time (fsim)        " << fsim_time << endl;
    mAnalyzer.print_stats(cout);
  }
}

// @brief 故障シミュレーションを行い，故障検出パタンを記録する．
// @param[in] fault_list 故障リスト
void
EqChecker::do_fsim(const vector<const TpgFault*>& fault_list)
{
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);

  mFsim.set_faults(fault_list);

  DetOp op;

  ymuint nf = fault_list.size();
  ymuint npat = nf;
  for (ymuint i = 0; i < nf; ++ i) {
    const TpgFault* fault = fault_list[i];
    const FaultInfo& fi = mAnalyzer.fault_info(fault->id());
    TestVector* tv = fi.testvector();
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      if ( mVerbose > 1 ) {
	cout << "\rFSIM: " << setw(6) << i;
	cout.flush();
      }
      mFsim.ppsfp(cur_array, op);
      const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
      mEqSet.multi_refinement(det_list);
      cur_array.clear();
      op.clear_det_list();
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, op);
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    mEqSet.multi_refinement(det_list);
    op.clear_det_list();
    cur_array.clear();
  }

  vector<TestVector*> cur_array2(kPvBitLen);
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    TestVector* tv = mTvMgr.new_vector();
    cur_array2[i] = tv;
  }

  for ( ; ; ) {
    npat += kPvBitLen;
    for (ymuint i = 0; i < kPvBitLen; ++ i) {
      cur_array2[i]->set_from_random(mRandGen);
    }
    mFsim.ppsfp(cur_array2, op);
    ymuint nchg = 0;
    const vector<pair<ymuint, PackedVal> >& det_list = op.det_list();
    if ( mEqSet.multi_refinement(det_list) ) {
      ++ nchg;
    }

    op.clear_det_list();
    if ( nchg == 0 ) {
      break;
    }
    if ( mVerbose > 1 ) {
      cout << "\rFSIM: " << setw(6) << npat;
      cout.flush();
    }
  }
  if ( mVerbose > 1 ) {
    cout << endl;
  }

  // 乱数パタンは削除しておく．
  for (ymuint i = 0; i < kPvBitLen; ++ i) {
    mTvMgr.delete_vector(cur_array2[i]);
  }

  mPat = npat;
}

END_NAMESPACE_YM_SATPG
