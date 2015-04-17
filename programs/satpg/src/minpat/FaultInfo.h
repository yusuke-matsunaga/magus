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
  TpgFault*
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

  /// @brief 外部入力上の十分割当(つまりテストベクタ)を返す．
  const NodeValList&
  pi_sufficient_assignment() const;

  /// @brief パタンリストを得る．
  const vector<ymuint>&
  pat_list() const;

  /// @brief パタンを追加する．
  void
  add_pat(ymuint pat);

  /// @brief この故障を検出するパタン数を返す．
  ymuint
  detnum() const;

  /// @brief パタンリストをソートする．
  void
  sort_pat_list();

  /// @brief 同時に検出可能な故障数を得る．
  ymuint
  fnum() const;

  /// @brief fnum を加える．
  void
  add_fnum(ymuint fnum);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障
  TpgFault* mFault;

  // テストベクタ
  TestVector* mTestVector;

  // 必要割当
  NodeValList mMandatoryAssignment;

  // 十分割当
  NodeValList mSufficientAssignment;

  // 外部入力上の十分割当
  NodeValList mPiSufficientAssignment;

  // この故障を検出するパタン番号のリスト
  vector<ymuint> mPatList;

  // この故障を検出するパタン数
  ymuint mDetNum;

  // 同時に検出可能な故障数の和
  ymuint mFnum;

public:

  Bool3 mStat;

  vector<ymuint> mConflictList;

  bool mSingleCube;

  bool mFirstDetect;

  vector<ymuint> mDomCandList;

  ymuint mDomCandListSize;

  vector<ymuint> mConflictCandList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FaultInfo::FaultInfo()
{
  mFnum = 0;
  mSingleCube = false;
  mFirstDetect = true;
}

// @brief デストラクタ
inline
FaultInfo::~FaultInfo()
{
}

// @brief 故障を得る．
inline
TpgFault*
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

// @brief 外部入力上の十分割当(つまりテストベクタ)を返す．
inline
const NodeValList&
FaultInfo::pi_sufficient_assignment() const
{
  return mPiSufficientAssignment;
}

// @brief パタンリストを得る．
inline
const vector<ymuint>&
FaultInfo::pat_list() const
{
  return mPatList;
}

// @brief パタンを追加する．
inline
void
FaultInfo::add_pat(ymuint pat)
{
  mPatList.push_back(pat);
  ++ mDetNum;
}

// @brief この故障を検出するパタン数を返す．
inline
ymuint
FaultInfo::detnum() const
{
  return mDetNum;
}

// @brief パタンリストをソートする．
inline
void
FaultInfo::sort_pat_list()
{
  sort(mPatList.begin(), mPatList.end());
}

// @brief 同時に検出可能な故障数を得る．
inline
ymuint
FaultInfo::fnum() const
{
  return mFnum;
}

// @brief fnum を加える．
inline
void
FaultInfo::add_fnum(ymuint fnum)
{
  mFnum += fnum;
}

END_NAMESPACE_YM_SATPG

#endif // FAULTINFO_H
