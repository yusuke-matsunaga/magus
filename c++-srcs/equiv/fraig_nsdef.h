#ifndef FRAIG_NSDEF_H
#define FRAIG_NSDEF_H

/// @file fraig_nsdef.h
/// @brief fraig の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"


// 名前空間の定義
#define BEGIN_NAMESPACE_FRAIG \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsFraig)

#define END_NAMESPACE_FRAIG \
END_NAMESPACE(nsFraig) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_FRAIG

class FraigMgr;
class FraigHandle;

END_NAMESPACE_FRAIG

BEGIN_NAMESPACE_MAGUS

using nsFraig::FraigMgr;
using nsFraig::FraigHandle;

END_NAMESPACE_MAGUS

#endif // FRAIG_NSDEF_H
