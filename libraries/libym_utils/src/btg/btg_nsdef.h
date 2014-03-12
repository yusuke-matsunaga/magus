#ifndef BTG_NSDEF_H
#define BTG_NSDEF_H

/// @file btg_nsdef.h
/// @brief btg の名前空間の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


/// @brief btg 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_BTG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBtg)

/// @brief mincov 用の名前空間の終了
#define END_NAMESPACE_YM_BTG \
END_NAMESPACE(nsBtg) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_BTG

class BtgMatchImpl;
class BtgNode;
class BtgEdge;

END_NAMESPACE_YM_BTG


#endif // BTG_NSDEF_H
