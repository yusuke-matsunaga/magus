#ifndef YMYMLOGIC_EXPR_NSDEF_H
#define YMYMLOGIC_EXPR_NSDEF_H

/// @file YmLogic/expr_nsdef.h
/// @brief lexp パッケージの基本ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup ExprGroup 論理式を扱うクラス
///
/// 論理式(ファクタードフォーム)を扱うためのクラスライブラリ
/// C++ の機能を活かして論理演算子のオーバーロードを使っている．


#include "YmTools.h"
#include "YmLogic/VarId.h"


/// @brief lexp 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_EXPR \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsExpr)

/// @brief ymlexp 用の名前空間の終了
#define END_NAMESPACE_YM_EXPR \
END_NAMESPACE(nsExpr) \
END_NAMESPACE_YM

/// @namespace nsYm::nsLogic::nsExpr
/// @brief Expr 関係のクラスの属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．

BEGIN_NAMESPACE_YM_EXPR

//////////////////////////////////////////////////////////////////////
// Expr の先行宣言
//////////////////////////////////////////////////////////////////////

class Expr;
class ExprWriter;

END_NAMESPACE_YM_EXPR


BEGIN_NAMESPACE_YM

using nsExpr::Expr;
using nsExpr::ExprWriter;

//////////////////////////////////////////////////////////////////////
// Expr を要素とするコンテナクラスの定義
//////////////////////////////////////////////////////////////////////

/// @ingroup ExprGroup
/// @brief Expr のベクタ
typedef vector<Expr> ExprVector;

/// @ingroup ExprGroup
/// @brief Expr のリスト
typedef list<Expr> ExprList;

/// @ingroup ExprGroup
/// @brief 変数番号から Expr への写像 (連想配列)
typedef unordered_map<VarId, Expr> VarExprMap;

END_NAMESPACE_YM

#endif // YMYMLOGIC_EXPR_NSDEF_H
