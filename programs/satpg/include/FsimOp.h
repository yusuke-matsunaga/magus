#ifndef FSIMOP_H
#define FSIMOP_H

/// @file FsimOp.h
/// @brief FsimOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "PackedVal.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FsimOp FsimOp.h "FsimOp.h"
/// @brief Fsim::ppsfp() で故障を検出した時の処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class FsimOp
{
public:

  /// @brief デストラクタ
  virtual
  ~FsimOp() { }


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
	     PackedVal dpat) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // FSIMOP2_H
