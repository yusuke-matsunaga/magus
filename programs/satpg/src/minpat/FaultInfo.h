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


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FaultInfo FaultInfo.h "FaultInfo.h"
/// @brief minpat で用いる故障用のデータ構造
//////////////////////////////////////////////////////////////////////
class FaultInfo
{
public:

  /// @brief コンストラクタ
  FaultInfo();

  /// @brief デストラクタ
  ~FaultInfo();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] fault 対象の故障
  void
  init(TpgFault* fault);

  /// @brief 故障を得る．
  TpgFault*
  fault() const;

  /// @brief パタンリストを得る．
  const vector<ymuint>&
  pat_list() const;

  /// @brief パタンを追加する．
  void
  add_pat(ymuint pat);

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

  // この故障を検出するパタン番号のリスト
  vector<ymuint> mPatList;

  // 同時に検出可能な故障数の和
  ymuint mFnum;

public:

  NodeValList mSufList;

  NodeValList mPiSufList;

  NodeValList mMaList;

  vector<ymuint> mConflictList;

  bool mExact;

  bool mFirstDetect;

  vector<ymuint> mDomCandList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FaultInfo::FaultInfo()
{
  mFnum = 0;
  mExact = false;
  mFirstDetect = true;
}

// @brief デストラクタ
inline
FaultInfo::~FaultInfo()
{
}

// @brief 初期化する．
// @param[in] fault 対象の故障
inline
void
FaultInfo::init(TpgFault* fault)
{
  mFault = fault;
}

// @brief 故障を得る．
inline
TpgFault*
FaultInfo::fault() const
{
  return mFault;
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
