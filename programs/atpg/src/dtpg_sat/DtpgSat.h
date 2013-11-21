#ifndef DTPGSAT_H
#define DTPGSAT_H

/// @file include/DtpgSat.h
/// @brief DtpgSat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DtpgSat.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "dtpgsat_nsdef.h"
#include "ym_networks/tgnet.h"
#include "ym_networks/TgNode.h"
#include "ym_logic/VarId.h"
#include "ym_logic/SatStats.h"


BEGIN_NAMESPACE_YM_ATPG

class SaFault;
class TestVector;

END_NAMESPACE_YM_ATPG

BEGIN_NAMESPACE_YM_ATPG_DTPGSAT

//////////////////////////////////////////////////////////////////////
/// @class DtpgSat DtpgSat.h "DtpgSat.h"
/// @brief SAT を用いたパタン生成器
//////////////////////////////////////////////////////////////////////
class DtpgSat
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr AtpgMgr
  DtpgSat();

  /// @brief デストラクタ
  virtual
  ~DtpgSat();


public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 一つの故障に対してテストパタン生成を行う．
  /// @param[in] network 対象の回路
  /// @param[in] f 対象の故障
  /// @param[out] tv 生成したパタンを入れるベクタ
  /// @retval kDetect パタン生成が成功した．
  /// @retval kUntest テスト不能故障だった．
  /// @retval kAbort アボートした．
  tStat
  run(const TgNetwork& network,
      SaFault* f,
      TestVector* tv);

  /// @brief 一つの故障に対してテストパタン生成を行なう．
  /// @param[in] network 対象の回路
  /// @param[in] fnode 故障ノード
  /// @param[in] is_input_fault 入力の故障の時 true
  /// @param[in] ipos 故障の入力位置
  /// @param[in] fsrc 故障ノードの入力
  /// @param[in] fval 故障値
  /// @param[in] tv 生成したパタンを入れるベクタ
  /// @retval kDetect パタン生成が成功した．
  /// @retval kUntest テスト不能故障だった．
  /// @retval kAbort アボートした．
  tStat
  dtpg_single(const TgNetwork& network,
	      const TgNode* fnode,
	      bool is_input_fault,
	      ymuint ipos,
	      const TgNode* fsrc,
	      int fval,
	      TestVector* tv);

  /// @brief 同じ位置の2つの故障に対してテストパタン生成を行なう．
  /// @param[in] network 対象の回路
  /// @param[in] fnode 故障ノード
  /// @param[in] is_input_fault 入力の故障の時 true
  /// @param[in] ipos 故障の入力位置
  /// @param[in] fsrc 故障ノードの入力
  /// @param[in] tv 生成したパタンを入れるベクタ
  /// @retval kDetect パタン生成が成功した．
  /// @retval kUntest テスト不能故障だった．
  /// @retval kAbort アボートした．
  pair<tStat, tStat>
  dtpg_dual(const TgNetwork& network,
	    const TgNode* fnode,
	    bool is_input_fault,
	    ymuint ipos,
	    const TgNode* fsrc,
	    TestVector* tv[]);

  /// @brief 直前の実行結果を得る．
  const SatStats&
  stats() const;

  /// @brief 使用する SAT エンジンを指定する．
  void
  set_mode(int mode);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 同じ位置の故障に対する CNF を作る．
  void
  make_cnf(SatSolver& solver,
	   const TgNetwork& network,
	   const TgNode* fnode,
	   bool is_input_fault,
	   ymuint ipos,
	   const TgNode* fsrc);

  enum Mark {
    kNone,
    kTFO,
    kTFI
  };

  /// @brief マークを得る．
  Mark&
  mark(const TgNode* node);

  /// @brief 正常回路の変数番号を得る．
  VarId
  gvar(const TgNode* node);

  /// @brief 故障回路の変数番号を得る．
  VarId
  fvar(const TgNode* node);

  /// @brief 故障差の変数番号を得る．
  VarId
  dvar(const TgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使うデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Var
  {
    // マーク
    Mark mMark;

    // 正常回路の変数番号
    VarId mGid;

    // 故障回路の変数番号
    VarId mFid;

    // 故障差の変数番号
    VarId mDid;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号の割り当て表
  vector<Var> mVarMap;

  // 直前の SAT の実行結果
  SatStats mStats;

  // SAT のモード
  int mMode;

};

// @brief マークを得る．
inline
DtpgSat::Mark&
DtpgSat::mark(const TgNode* node)
{
  return mVarMap[node->gid()].mMark;
}

// @brief 正常回路の変数番号を得る．
inline
VarId
DtpgSat::gvar(const TgNode* node)
{
  Var& var = mVarMap[node->gid()];
  assert_cond(var.mMark != kNone, __FILE__, __LINE__);
  return var.mGid;
}

// @brief 故障回路の変数番号を得る．
inline
VarId
DtpgSat::fvar(const TgNode* node)
{
  Var& var = mVarMap[node->gid()];
  assert_cond(var.mMark != kNone, __FILE__, __LINE__);
  return var.mFid;
}

// @brief 故障差の変数番号を得る．
inline
VarId
DtpgSat::dvar(const TgNode* node)
{
  Var& var = mVarMap[node->gid()];
  assert_cond(var.mMark == kTFO, __FILE__, __LINE__);
  return var.mDid;
}

END_NAMESPACE_YM_ATPG_DTPGSAT

#endif // MODBASE_H
