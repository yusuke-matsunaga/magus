#ifndef DG_H
#define DG_H

/// @file dg.h
/// @brief dg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"


/// @brief dg 用の名前空間の開始
#define BEGIN_NAMESPACE_DG \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsDg)

/// @brief trusyn 用の名前空間の終了
#define END_NAMESPACE_DG \
END_NAMESPACE(nsDg) \
END_NAMESPACE_MAGUS

#define DG_NAMESPACE MAGUS_NAMESPACE::nsDg

BEGIN_NAMESPACE_DG


END_NAMESPACE_DG

BEGIN_NAMESPACE_MAGUS


END_NAMESPACE_MAGUS

#endif // DG_H
