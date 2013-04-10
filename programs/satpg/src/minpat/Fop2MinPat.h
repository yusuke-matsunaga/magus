#ifndef FOP2MINPAT_H
#define FOP2MINPAT_H

/// @file Fop2MinPat.h
/// @brief Fop2MinPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Fop2MinPat Fop2MinPat.h "Fop2MinPat.h"
/// @brief MinPat で用いる FsimOp1
//////////////////////////////////////////////////////////////////////
class Fop2MinPat :
  public FsimOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fmgr 故障マネージャ
  Fop2MinPat(Fsim& fsim,
	     FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~Fop2MinPat();


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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // しきい値
  ymuint32 mLimit;

  // 現在のパタンリスト
  vector<TestVector*> mCurPatList;

  // 故障の検出回数
  vector<ymuint32> mDetCount;

  // 故障を検出するパタンのリスト
  vector<vector<TestVector*> > mPatListArray;

};

END_NAMESPACE_YM_SATPG

#endif // FOP1MINPAT_H
