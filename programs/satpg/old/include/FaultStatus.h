#ifndef FAULTSTATUS_H
#define FAULTSTATUS_H

/// @file FaultStatus.h
/// @brief FaultStatus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2007, 2012-2013 Yusuke Matsunaga
/// All rights reserved.

#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @brief 故障の状態
//////////////////////////////////////////////////////////////////////
enum FaultStatus {
  /// @brief 未検出
  kFsUndetected,
  /// @brief 検出
  kFsDetected,
  /// @brief テスト不能 (冗長)
  kFsUntestable,
  /// @brief 部分的なテスト不能
  kFsSkipped,
  /// @brief アボート
  kFsAborted
};


/// @brief 内容を表す文字列を返す．
inline
const char*
str(FaultStatus fs)
{
  switch ( fs ) {
  case kFsUndetected: return "undetected";
  case kFsDetected:   return "detected";
  case kFsUntestable: return "untestable";
  case kFsSkipped:    return "skipped";
  case kFsAborted:    return "aborted";
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_SATPG

#endif // FAULTSTATUS_H
