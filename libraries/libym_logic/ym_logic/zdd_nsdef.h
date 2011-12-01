#ifndef YM_LOGIC_ZDD_NSDEF_H
#define YM_LOGIC_ZDD_NSDEF_H

/// @file ym_logic/zdd_nsdef.h
/// @brief ZDD に共通な定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Zdd ZDD パッケージ
///
///

#include "ymtools.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief ZDD 用の名前空間の開始
/// @ingroup Zdd
#define BEGIN_NAMESPACE_YM_ZDD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsZdd)

/// @brief ZDD 用の名前空間の終了
/// @ingroup Zdd
#define END_NAMESPACE_YM_ZDD \
END_NAMESPACE(nsZdd) \
END_NAMESPACE_YM

/// @namespace nsYm::nsZdd
/// @brief ZDD 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_ZDD


//////////////////////////////////////////////////////////////////////
// クラス名の前方参照用の宣言
//////////////////////////////////////////////////////////////////////

class Zdd;
class ZddMgr;
class ZddMgrParam;
class ZddMgrImpl;
class ZddEdge;


//////////////////////////////////////////////////////////////////////
/// @name 型名の定義
/// @{

/// @brief ZDD のベクタ
/// @ingroup Zdd
typedef vector<Zdd> ZddVector;

/// @brief ZDD のリスト
/// @ingroup Zdd
typedef list<Zdd> ZddList;

/// @}
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_ZDD

BEGIN_NAMESPACE_YM

using nsZdd::Zdd;
using nsZdd::ZddMgr;
using nsZdd::ZddMgrParam;
using nsZdd::ZddVector;
using nsZdd::ZddList;

END_NAMESPACE_YM

#endif // YM_ZDD_ZDD_NSDEF_H
