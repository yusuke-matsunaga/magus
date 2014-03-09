
/// @file Verifier.cc
/// @brief Verifier の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Verifier.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FopVer
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fsim 故障シミュレータ
FopVer::FopVer(Fsim& fsim) :
  mFsim(fsim)
{
}

// @brief デeストラクタ
FopVer::~FopVer()
{
}

// @brief 故障を検出したときの処理
// @param[in] f 故障
void
FopVer::operator()(TpgFault* f,
		   PackedVal dpat)
{
  mFsim.set_skip(f);
  mDetSet.insert(f);
}

// @brief det_flag を下ろす．
void
FopVer::clear_det_flag()
{
  mDetSet.clear();
}

// @brief 故障が見つかったら true を返す．
bool
FopVer::is_detected(TpgFault* f)
{
  return mDetSet.count(f) > 0;
}


//////////////////////////////////////////////////////////////////////
// クラス Verifier
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] fault_mgr 故障マネージャ
// @param[in] fsim 故障シミュレータ
Verifier::Verifier(FaultMgr& fault_mgr,
		   Fsim& fsim) :
  mFaultMgr(fault_mgr),
  mFsim(fsim),
  mOp(fsim)
{
}

// @brief デストラクタ
Verifier::~Verifier()
{
}

// @brief 与えられたパタンリストで全検出済み故障を検出できるか調べる．
// @param[in] pat_list パタンのリスト
bool
Verifier::check(const vector<TestVector*>& pat_list)
{
  mOp.clear_det_flag();
  const vector<TpgFault*>& f_list = mFaultMgr.det_list();
  mFsim.set_faults(f_list);
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  for (vector<TestVector*>::const_iterator p = pat_list.begin();
       p != pat_list.end(); ++ p) {
    TestVector* tv = *p;
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      mFsim.ppsfp(cur_array, mOp);
      cur_array.clear();
    }
  }
  if ( !cur_array.empty() ) {
    mFsim.ppsfp(cur_array, mOp);
  }

  bool no_error = true;
  for (vector<TpgFault*>::const_iterator p = f_list.begin();
       p != f_list.end(); ++ p) {
    TpgFault* f = *p;
    if ( !mOp.is_detected(f) ) {
      cout << "Error: " << f << " has no patterns" << endl;
      no_error = false;
    }
  }

  return no_error;
}

END_NAMESPACE_YM_SATPG
