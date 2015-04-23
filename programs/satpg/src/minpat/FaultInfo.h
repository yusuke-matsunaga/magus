#ifndef FAULTINFO_H
#define FAULTINFO_H

/// @file FaultInfo.h
/// @brief FaultInfo のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "NodeValList.h"
#include "YmLogic/Bool3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultInfo FaultInfo.h "FaultInfo.h"
/// @brief minpat で用いる故障用のデータ構造
//////////////////////////////////////////////////////////////////////
class FaultInfo
{
  friend class FaultAnalyzer;

public:

  /// @brief コンストラクタ
  FaultInfo();

  /// @brief デストラクタ
  ~FaultInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を得る．
  const TpgFault*
  fault() const;

  /// @brief 故障を検出するテストベクタを返す．
  TestVector*
  testvector() const;

  /// @biref 必要割当を返す．
  const NodeValList&
  mandatory_assignment() const;

  /// @brief 十分割当を返す．
  const NodeValList&
  sufficient_assignment() const;

  /// @brief 2番目以降の十分割当のキューブ数を返す．
  ymuint
  other_sufficient_assignment_num() const;

  /// @brief 2番め以降の十分割当を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < other_sufficient_assignment_num() )
  const NodeValList&
  other_sufficient_assignment(ymuint pos) const;

  /// @brief 外部入力上の十分割当(つまりテストベクタ)を返す．
  const NodeValList&
  pi_sufficient_assignment() const;

  /// @brief 条件が single cube の時 true を返す．
  bool
  single_cube() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障
  const TpgFault* mFault;

  // テストベクタ
  TestVector* mTestVector;

  // 必要割当
  NodeValList mMandatoryAssignment;

  // 十分割当
  NodeValList mSufficientAssignment;

  // 外部入力上の十分割当
  NodeValList mPiSufficientAssignment;

  bool mSingleCube;

  vector<NodeValList> mOtherSufListArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FaultInfo::FaultInfo()
{
  mSingleCube = false;
}

// @brief デストラクタ
inline
FaultInfo::~FaultInfo()
{
}

// @brief 故障を得る．
inline
const TpgFault*
FaultInfo::fault() const
{
  return mFault;
}

// @brief 故障を検出するテストベクタを返す．
inline
TestVector*
FaultInfo::testvector() const
{
  return mTestVector;
}

// @biref 必要割当を返す．
inline
const NodeValList&
FaultInfo::mandatory_assignment() const
{
  return mMandatoryAssignment;
}

// @brief 十分割当を返す．
inline
const NodeValList&
FaultInfo::sufficient_assignment() const
{
  return mSufficientAssignment;
}

// @brief 2番目以降の十分割当のキューブ数を返す．
inline
ymuint
FaultInfo::other_sufficient_assignment_num() const
{
  return mOtherSufListArray.size();
}

// @brief 2番め以降の十分割当を返す．
// @param[in] pos 位置番号 ( 0 <= pos < other_sufficient_assignment_num() )
inline
const NodeValList&
FaultInfo::other_sufficient_assignment(ymuint pos) const
{
  ASSERT_COND( pos < other_sufficient_assignment_num() );
  return mOtherSufListArray[pos];
}

// @brief 外部入力上の十分割当(つまりテストベクタ)を返す．
inline
const NodeValList&
FaultInfo::pi_sufficient_assignment() const
{
  return mPiSufficientAssignment;
}

// @brief 条件が single cube の時 true を返す．
inline
bool
FaultInfo::single_cube() const
{
  return mSingleCube;
}

END_NAMESPACE_YM_SATPG

#endif // FAULTINFO_H
