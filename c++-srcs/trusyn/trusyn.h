#ifndef TRUSYN_H
#define TRUSYN_H

/// @file trusyn.h
/// @brief trusyn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"


/// @brief trusyn 用の名前空間の開始
#define BEGIN_NAMESPACE_TRUSYN \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsTrusyn)

/// @brief trusyn 用の名前空間の終了
#define END_NAMESPACE_TRUSYN \
END_NAMESPACE(nsTrusyn) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_TRUSYN

class TruReader;

END_NAMESPACE_TRUSYN

BEGIN_NAMESPACE_MAGUS

using nsTrusyn::TruReader;

END_NAMESPACE_MAGUS

#endif // TRUSYN_H
