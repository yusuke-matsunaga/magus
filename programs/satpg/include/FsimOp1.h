#ifndef FSIMOP1_H
#define FSIMOP1_H

/// @file FsimOp1.h
/// @brief FsimOp1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FsimOp1 FsimOp1.h "FsimOp1.h"
/// @brief Fsim で故障を検出した時の処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class FsimOp1
{
public:

  /// @brief デストラクタ
  virtual
  ~FsimOp1() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // FSIMOP1_H
