#ifndef VERIFIER_H
#define VERIFIER_H

/// @file Verifier.h
/// @brief Verifier のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "Fsim.h"
#include "FaultMgr.h"
#include "TestVector.h"
#include "FsimOp.h"
#include "TpgFault.h"


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

  // 検出済みの故障を入れるハッシュ
  unordered_set<TpgFault*> mDetSet;

};


//////////////////////////////////////////////////////////////////////
/// @class Verifier Verifier.h "Verifier.h"
/// @brief 圧縮したパタンが正しいかチェックするクラス
//////////////////////////////////////////////////////////////////////
class Verifier
{
public:

  /// @brief コンストラクタ
  /// @param[in] fault_mgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  Verifier(FaultMgr& fault_mgr,
	   Fsim& fsim);

  /// @brief デストラクタ
  ~Verifier();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられたパタンリストで全検出済み故障を検出できるか調べる．
  /// @param[in] pat_list パタンのリスト
  bool
  check(const vector<TestVector*>& pat_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障マネージャ
  FaultMgr& mFaultMgr;

  // 故障シミュレータ
  Fsim& mFsim;

  FopVer mOp;
};

END_NAMESPACE_YM_SATPG

#endif // VERIFIER_H
