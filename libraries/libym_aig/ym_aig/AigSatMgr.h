#ifndef YM_AIG_AIGSATMGR_H
#define YM_AIG_AIGSATMGR_H

/// @file ym_aig/AigSatMgr.h
/// @brief AigSatMgrのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigMgr.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigHandle.h"
#include "ym_sat/SatSolver.h"


#error "Who need this?"

BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigSatMgr AigSatMgr.h "ym_aig/AigSatMgr.h"
/// @brief AIG 上の充足可能性判定器
//////////////////////////////////////////////////////////////////////
class AigSatMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] aitmgr AigMgr
  /// @param[in] solver SAT-solver
  AigSatMgr(AigMgr& aigmgr,
	    SatSolver& solver);

  /// @brief デストラクタ
  ~AigSatMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // SAT solver とのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SAT 問題を解く．
  /// @param[in] edge この出力を1にできるか調べる．
  /// @param[out] model 外部入力の割り当てを入れる配列
  /// @retval kB3False 充足不能
  /// @retval kB3True 充足可能
  /// @retval kB3X 不明
  Bool3
  sat(AigHandle edge,
      vector<Bool3>& model);

  /// @brief SAT 問題を解く．
  /// @param[in] edge_list これらの出力を同時に1にできるか調べる．
  /// @param[out] model 外部入力の割り当てを入れる配列
  /// @retval kB3False 充足不能
  /// @retval kB3True 充足可能
  /// @retval kB3X 不明
  Bool3
  sat(const vector<AigHandle>& edge_list,
      vector<Bool3>& model);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AigMgr
  AigMgr& mAigMgr;

  // SAT-solver
  SatSolver& mSolver;

};

END_NAMESPACE_YM_AIG

#endif // YM_AIG_AIGMGR_H
