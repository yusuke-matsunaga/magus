#ifndef IGUGEN2_H
#define IGUGEN2_H

/// @file IguGen2.h
/// @brief IguGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"
#include "logic/BddMgr.h"
#include "logic/Bdd.h"
#include "utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_IGF

class IgVar;

//////////////////////////////////////////////////////////////////////
/// @class IguGen2 IguGen2.h "IguGen2.h"
/// @brief IGU の入力を求めるクラス (BDDバージョン)
//////////////////////////////////////////////////////////////////////
class IguGen2
{
public:

  /// @brief コンストラクタ
  IguGen2();

  /// @brief デストラクタ
  ~IguGen2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ベクタのリストをセットする．
  void
  set_vector_list(const vector<const RegVect*>& vector_list);

  /// @brief 変数を求める．
  /// @param[in] variable_list 変数のリスト
  /// @param[in] best_so_far 今までに求められている最適解
  /// @param[in] solution 解として選ばれた変数を格納するリスト
  void
  solve(const vector<const Variable*>& variable_list,
	ymuint best_so_far,
	vector<const Variable*>& solution);

  /// @brief 分岐制限を設定する．
  /// @param[in] limit 分岐制限
  /// @note limit = 0 の場合には制限なし
  void
  set_branch_limit(ymuint limit);

  /// @brief 時間制限を設定する．
  /// @param[in] limit_min 制限の分の部分
  /// @param[in] limit_sec 制限の秒の部分
  void
  set_time_limit(ymuint32 limit_min,
		 ymuint32 limit_sec = 0);

  /// @brief デバッグレベルを指定する．
  void
  set_debug_level(ymuint32 level);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve() の下請け関数
  /// @param[in] ds_so_far 既に区別されている対の集合を表す BDD
  /// @param[in] var_list 使用可能な変数のリスト
  /// @param[in] pos var_list の開始位置
  void
  solve_recur(Bdd ds_so_far,
	      const vector<IgVar*>& var_list,
	      ymuint pos);

  /// @brief 下界を計算する．
  /// @param[in] num 要素数
  ymuint
  lower_bound(ymuint num) const;

  /// @brief IgVar を生成する．
  IgVar*
  new_IgVar(const Variable* var,
	    Bdd ds_func);

  /// @brief IgVar を削除する．
  void
  delete_IgVar(IgVar* ig_var);

  /// @brief ds_func を計算する．
  Bdd
  calc_ds_func(const Variable* var);

  /// @brief 要素数を計算する．
  ymuint
  elem_count(Bdd f);

  /// @brief ベクタのリストから BDD を作る．
  Bdd
  vects_to_bdd(const vector<const RegVect*>& vect_list,
	       ymuint pos,
	       ymuint var_base,
	       Bdd leaf_bdd);

  /// @brief ベクタのリストから BDD を作る．
  Bdd
  vects_to_bdd(const vector<const RegVect*>& vect_list0,
	       const vector<const RegVect*>& vect_list1,
	       ymuint pos,
	       Bdd leaf0,
	       Bdd leaf1);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddMgr
  BddMgr mBddMgr;

  // IgVar の確保用アロケータ
  UnitAlloc mAlloc;

  // 現在のビットベクタ長
  ymuint32 mVectorLength;

  // 現在のベクターリスト
  vector<const RegVect*> mVectorList;

  // 全 distinguished set を表す BDD
  Bdd mDsAll;

  // mDsAll の要素数
  ymuint32 mDsAllCount;

  // 現時点の最適値
  ymuint32 mBestSoFar;

  // 現時点の解
  vector<const Variable*> mSolutionSoFar;

  // 現時点で選ばれている変数のリスト
  vector<const Variable*> mSelectedVariables;

  // いくつの分岐を試すかを制御するパラメータ
  ymuint32 mBranchLimit;

  // 時間制限
  ymuint32 mTimeLimit;

  // デバッグフラグ
  ymuint32 mDebug;

};


END_NAMESPACE_YM_IGF

#endif // IGUGEN_H
