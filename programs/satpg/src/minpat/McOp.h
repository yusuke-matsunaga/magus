#ifndef MCOP_H
#define MCOP_H

/// @file McOp.h
/// @brief McOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"
#include "YmAlgo/MinCov.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// @class McOp McOp.h "McOp.h"
//////////////////////////////////////////////////////////////////////
class McOp :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mincov 最小被覆問題のオブジェクト
  /// @param[in] row_map 故障番号から行番号を得る表
  /// @param[in] col_pos 列番号
  McOp(MinCov& mincov,
       const vector<ymuint>& row_map,
       ymuint col_pos);

  /// @brief デストラクタ
  virtual
  ~McOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(const TpgFault* f,
	     PackedVal dpat);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最小被覆問題
  MinCov& mMinCov;

  // 行番号を得るための表
  const vector<ymuint>& mRowMap;

  // 列番号
  ymuint mColPos;

};

END_NAMESPACE_YM_SATPG

#endif // MCOP_H
