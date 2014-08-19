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

class VectSetList;

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

  /// @brief ベクタのリストをセットする．
  void
  set_vector_list(const vector<const RegVect*>& vector_list);

  /// @brief 変数を求める．
  /// @param[in] multi 多重度
  /// @param[in] variable_list 変数のリスト
  /// @param[in] best_so_far 今までに求められている最適解
  /// @param[in] solution 解として選ばれた変数を格納するリスト
  void
  solve(ymuint multi,
	const vector<const Variable*>& variable_list,
	ymuint best_so_far,
	vector<const Variable*>& solution);

  /// @brief 再帰呼び出しの回数制限を設定する．
  void
  set_recur_limit(ymuint limit);

  /// @brief 時間制限を設定する．
  /// @param[in] limit_min 制限の分の部分
  /// @param[in] limit_sec 制限の秒の部分
  void
  set_time_limit(ymuint32 limit_min,
		 ymuint32 limit_sec = 0);

  /// @brief 順序付けのヒューリスティックを指定する．
  void
  set_ordering_mode(ymuint id);

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
	      vector<const Variable*>::const_iterator var_begin,
	      vector<const Variable*>::const_iterator var_end);

  /// @brief 下界を計算する．
  /// @param[in] num 要素数
  ymuint
  lower_bound(ymuint num) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 多重度
  ymuint32 mMulti;

  // 現在のビットベクタ長
  ymuint32 mVectorLength;

  // 現在のベクターリスト
  vector<const RegVect*> mVectorList;

  // 現時点の最適値
  ymuint32 mBestSoFar;

  // mBestSorFar がセットされた．
  bool mBeforeHasSolution;

  // mBestSoFar が更新されていない回数
  ymuint32 mNoChangeCount;

  // mNoChangeCount の制限値
  ymuint32 mNoChangeLimit;

  // mNoChangeCount の最大値
  ymuint32 mMaxNoChangeCount;

  // 現時点の解
  vector<const Variable*> mSolutionSoFar;

  // 現時点で選ばれている変数のリスト
  vector<const Variable*> mSelectedVariables;

  // 順序づけのヒューリスティック
  ymuint32 mOrderingMode;

  // 時間制限
  ymuint32 mTimeLimit;

  // デバッグフラグ
  ymuint32 mDebug;

};


END_NAMESPACE_YM_IGF

#endif // IGUGEN_H
