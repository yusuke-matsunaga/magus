
/// @file Verifier.cc
/// @brief Verifier の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Verifier.h"
#include "FsimOp.h"
#include "TpgFault.h"
#include "TestVector.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FopVer Verifier.h "Verifier.h"
/// @brief Verifier で用いられる FsimOp
//////////////////////////////////////////////////////////////////////

class FopVer :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  FopVer(Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~FopVer();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f,
	     PackedVal dpat);

  /// @brief det_flag を下ろす．
  void
  clear_det_flag();

  /// @brief 故障が見つかったら true を返す．
  bool
  is_detected(TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 検出済みの故障番号を入れるハッシュ
  HashSet<ymuint> mDetSet;

};


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
FopVer::operator()(TpgFault* fault,
		   PackedVal dpat)
{
  mFsim.set_skip(fault);
  mDetSet.add(fault->id());
}

// @brief det_flag を下ろす．
void
FopVer::clear_det_flag()
{
  mDetSet.clear();
}

// @brief 故障が見つかったら true を返す．
bool
FopVer::is_detected(TpgFault* fault)
{
  return mDetSet.check(fault->id());
}


//////////////////////////////////////////////////////////////////////
// クラス Verifier
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Verifier::Verifier()
{
}

// @brief デストラクタ
Verifier::~Verifier()
{
}

// @brief 与えられたパタンリストで全検出済み故障を検出できるか調べる．
// @param[in] fsim 故障シミュレータ
// @param[in] fault_list 故障のリスト
// @param[in] pat_list パタンのリスト
bool
Verifier::check(Fsim& fsim,
		const vector<TpgFault*>& fault_list,
		const vector<TestVector*>& pat_list)
{
  FopVer op(fsim);
  op.clear_det_flag();

  fsim.set_faults(fault_list);
  vector<TestVector*> cur_array;
  cur_array.reserve(kPvBitLen);
  for (vector<TestVector*>::const_iterator p = pat_list.begin();
       p != pat_list.end(); ++ p) {
    TestVector* tv = *p;
    cur_array.push_back(tv);
    if ( cur_array.size() == kPvBitLen ) {
      fsim.ppsfp(cur_array, op);
      cur_array.clear();
    }
  }
  if ( !cur_array.empty() ) {
    fsim.ppsfp(cur_array, op);
  }

  bool no_error = true;
  for (vector<TpgFault*>::const_iterator p = fault_list.begin();
       p != fault_list.end(); ++ p) {
    TpgFault* fault = *p;
    if ( !op.is_detected(fault) ) {
      cout << "Error: " << fault << " has no patterns" << endl;
      no_error = false;
    }
  }

  return no_error;
}

END_NAMESPACE_YM_SATPG
