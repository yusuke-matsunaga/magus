#ifndef FSIMFAULT_H
#define FSIMFAULT_H

/// @file src/fsimx2/FsimFault.h
/// @brief FsimFault のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "fsimx2_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG_FSIMX2

class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class FsimFault FsimFault.h "FsimFault.h"
/// @brief 故障シミュレーション用の故障関係のデータ構造
//////////////////////////////////////////////////////////////////////
struct FsimFault
{
  /// @brief 内容を設定する便利関数
  /// @param[in] f オリジナルの故障
  /// @param[in] node 対応する SimNode
  /// @param[in] ipos 入力番号
  /// @param[in] inode 入力に対応する SimNode
  /// @note ipos と inode は f が入力の故障の時のみ意味を持つ．
  void
  set(TpgFault* f,
      SimNode* node,
      ymuint ipos,
      SimNode* inode)
  {
    mOrigF = f;
    mNode = node;
    mIpos = ipos;
    mInode = inode;
    mSkip = false;
  }

  // 元の故障
  TpgFault* mOrigF;

  // 故障のあるゲート
  SimNode* mNode;

  // 入力の故障の場合の入力位置
  ymuint32 mIpos;

  // 入力の故障の場合の入力のゲート
  SimNode* mInode;

  // 現在計算中のローカルな故障伝搬マスク
  PackedVal mObsMask;

  // スキップフラグ
  bool mSkip;

};

END_NAMESPACE_YM_SATPG_FSIMX2

#endif // FSIMFAULT_H
