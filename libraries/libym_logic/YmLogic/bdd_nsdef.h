#ifndef YMYMLOGIC_BDD_NSDEF_H
#define YMYMLOGIC_BDD_NSDEF_H

/// @file YmLogic/bdd_nsdef.h
/// @brief BDD に共通な定義ファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup Bdd BDD パッケージ
///
///

#include "YmTools.h"
#include "YmLogic/VarId.h"


//////////////////////////////////////////////////////////////////////
// 名前空間を定義するためのマクロ
//////////////////////////////////////////////////////////////////////

/// @brief BDD 用の名前空間の開始
/// @ingroup Bdd
#define BEGIN_NAMESPACE_YM_BDD \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsBdd)

/// @brief BDD 用の名前空間の終了
/// @ingroup Bdd
#define END_NAMESPACE_YM_BDD \
END_NAMESPACE(nsBdd) \
END_NAMESPACE_YM

/// @namespace nsYm::nsBdd
/// @brief BDD 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス名の前方参照用の宣言
//////////////////////////////////////////////////////////////////////

class Bdd;
class BddMgr;
class BddMgrParam;
class BddVarSet;
class BddLitSet;
class BddVector;
class BddList;

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_YM

using nsBdd::Bdd;
using nsBdd::BddMgr;
using nsBdd::BddMgrParam;
using nsBdd::BddVarSet;
using nsBdd::BddLitSet;
using nsBdd::BddVector;
using nsBdd::BddList;

END_NAMESPACE_YM

#endif // YMYMLOGIC_BDD_NSDEF_H
