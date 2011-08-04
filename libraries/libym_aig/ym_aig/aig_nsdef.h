#ifndef YM_AIG_AIG_NSDEF_H
#define YM_AIG_AIG_NSDEF_H

/// @file ym_aig/aig_nsdef.h
/// @brief AIG パッケージ用の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


/// @brief ym_aig 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_AIG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsAig)

/// @brief ym_aig 用の名前空間の終了
#define END_NAMESPACE_YM_AIG \
END_NAMESPACE(nsAig) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_AIG

class AigMgr;
class AigHandle;
class AigNode;

class AigSatMgr;

END_NAMESPACE_YM_AIG


BEGIN_NAMESPACE_YM

using nsAig::AigMgr;
using nsAig::AigHandle;
using nsAig::AigNode;

using nsAig::AigSatMgr;

END_NAMESPACE_YM

#endif // YM_AIG_AIG_NSDEF_H
