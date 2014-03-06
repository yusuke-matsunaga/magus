#ifndef FOPMINPAT_H
#define FOPMINPAT_H

/// @file FopMinPat.h
/// @brief FopMinPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FopMinPat FopMinPat.h "FopMinPat.h"
/// @brief MinPat で用いる FsimOp
//////////////////////////////////////////////////////////////////////
class FopMinPat :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fmgr 故障マネージャ
  FopMinPat(Fsim& fsim,
	    FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~FopMinPat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(TpgFault* f,
	     PackedVal dpat);

  /// @brief 検出回数をクリアする．
  void
  clear_count();

  /// @brief 検出回数のしきい値をセットする．
  void
  set_limit(ymuint limit);

  /// @brief パタンをセットする．
  void
  set_pattern(const vector<TestVector*>& pat_list);

  /// @brief 故障に対するパタンのリストを返す．
  /// @param[in] f_id 故障の ID
  const vector<TestVector*>&
  pat_list(ymuint f_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 1つの故障に関連した情報
  struct FaultInfo {

    FaultInfo() { }

    // 検出回数
    ymuint32 mDetCount;

    // 検出するパタンのリスト
    vector<TestVector*> mPatList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // しきい値
  ymuint32 mLimit;

  // 現在のパタンリスト
  vector<TestVector*> mCurPatList;

  // 故障に関連した情報の配列
  // 故障のIDでインデクシングされている．
  vector<FaultInfo> mFinfoArray;

};

END_NAMESPACE_YM_SATPG

#endif // FOPMINPAT_H
