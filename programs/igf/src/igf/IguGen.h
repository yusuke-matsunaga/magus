#ifndef IGUGEN_H
#define IGUGEN_H

/// @file IguGen.h
/// @brief IguGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class IguGen IguGen.h "IguGen.h"
/// @brief IGU の入力を求めるクラス
//////////////////////////////////////////////////////////////////////
class IguGen
{
public:

  /// @brief コンストラクタ
  IguGen();

  /// @brief デストラクタ
  ~IguGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を求める．
  /// @param[in] vector_list ベクタのリスト
  /// @param[in] multi 多重度
  /// @param[in] variable_list 変数のリスト
  /// @param[in] solution 解として選ばれた変数を格納するリスト
  void
  solve(const vector<RegVect*>& vector_list,
	ymuint multi,
	const vector<Variable*>& variable_list,
	vector<Variable*>& solution);

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


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief solve() の下請け関数
  /// @param[in] vector_list 分割されたベクタのリスト
  /// @param[in] var_begin 使用可能な変数のリストの先頭の反復子
  /// @param[in] var_end 使用可能な変数のリストの末尾の反復子
  void
  solve_recur(const VectSetList& vector_list,
	      vector<Variable*>::const_iterator var_begin,
	      vector<Variable*>::const_iterator var_end);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 多重度
  ymuint32 mMulti;

  // 現時点の最適値
  ymuint32 mBestSoFar;

  // 現時点の解
  vector<Variable*> mSolutionSoFar;

  // 現時点で選ばれている変数のリスト
  vector<Variable*> mSelectedVariables;

  // いくつの分岐を試すかを制御するパラメータ
  ymuint32 mBranchLimit;

  // 時間制限
  ymuint32 mTimeLimit;

  // デバッグフラグ
  ymuint32 mDebug;

};


END_NAMESPACE_YM_IGF

#endif // IGUGEN_H
