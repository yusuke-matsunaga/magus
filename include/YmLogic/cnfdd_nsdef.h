#ifndef YMYMLOGIC_CNFDD_NSDEF_H
#define YMYMLOGIC_CNFDD_NSDEF_H

/// @file YmLogic/cnfdd_nsdef.h
/// @brief CNFDD に共通な定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup CNFDD CNFDD パッケージ
///
///

#include "YmTools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief CNFDD 用の名前空間の開始
/// @ingroup CNFdd
#define BEGIN_NAMESPACE_YM_CNFDD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCNFdd)

/// @brief CNFDD 用の名前空間の終了
/// @ingroup CNFDD
#define END_NAMESPACE_YM_CNFDD \
END_NAMESPACE(nsCNFdd) \
END_NAMESPACE_YM

/// @namespace nsYm::nsCNFdd
/// @brief CNFDD 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス名の前方参照用の宣言
//////////////////////////////////////////////////////////////////////

class CNFdd;
class CNFddMgr;
class CNFddMgrParam;

END_NAMESPACE_YM_CNFDD

BEGIN_NAMESPACE_YM

using nsCNFdd::CNFdd;
using nsCNFdd::CNFddMgr;
using nsCNFdd::CNFddMgrParam;

END_NAMESPACE_YM

#endif // YMYMLOGIC_CNFDD_NSDEF_H
