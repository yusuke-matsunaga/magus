#ifndef YM_LOGIC_LEXP_NSDEF_H
#define YM_LOGIC_LEXP_NSDEF_H

/// @file ym_lexp/lexp_nsdef.h
/// @brief ym_lexp パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup LexpGroup 論理式を扱うクラス
///
/// 論理式(ファクタードフォーム)を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．


#include "ymtools.h"


/// @brief ym_lexp 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_LEXP \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsLexp)

/// @brief ymlexp 用の名前空間の終了
#define END_NAMESPACE_YM_LEXP \
END_NAMESPACE(nsLexp) \
END_NAMESPACE_YM

/// @namespace nsYm::nsLogic::nsLexp
/// @brief LogExpr 関係のクラスの属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．

BEGIN_NAMESPACE_YM_LEXP

//////////////////////////////////////////////////////////////////////
// LogExpr の先行宣言
//////////////////////////////////////////////////////////////////////

class LogExpr;
class LogExprWriter;

//////////////////////////////////////////////////////////////////////
// LogExpr を要素とするコンテナクラスの定義
//////////////////////////////////////////////////////////////////////

/// @ingroup LexpGroup
/// @brief LogExpr のベクタ
typedef vector<LogExpr> LogExprVector;

/// @ingroup LexpGroup
/// @brief LogExpr のリスト
typedef list<LogExpr> LogExprList;

/// @ingroup LexpGroup
/// @brief 変数番号から LogExpr への写像 (連想配列)
typedef hash_map<ymuint, LogExpr> VarLogExprMap;


END_NAMESPACE_YM_LEXP


BEGIN_NAMESPACE_YM

using nsLexp::LogExpr;
using nsLexp::LogExprWriter;
using nsLexp::LogExprVector;
using nsLexp::LogExprList;
using nsLexp::VarLogExprMap;

END_NAMESPACE_YM

#endif // YM_LOGIC_LEXP_NSDEF_H
