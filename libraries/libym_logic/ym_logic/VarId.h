#ifndef YM_LOGIC_VARID_H
#define YM_LOGIC_VARID_H

/// @file ym_logic/VarId.h
/// @brief tVarId の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @ingroup LogicGroup
/// @brief 変数番号を表す型
/// @note 範囲は 0 〜 kVarIdMax
//////////////////////////////////////////////////////////////////////
typedef ymuint32 tVarId;

//////////////////////////////////////////////////////////////////////
/// @ingroup LogicGroup
/// @relates tVarId
/// @brief 変数番号の最大値
/// @note 極性をパックすることを考えて 1 ビット減らしている．
//////////////////////////////////////////////////////////////////////
const tVarId kVarIdMax = (1UL << (sizeof(tVarId) * 8 - 1)) - 1;


//////////////////////////////////////////////////////////////////////
// tVarId を要素とするコンテナクラスの定義
//////////////////////////////////////////////////////////////////////

/// @ingroup LogicGroup
/// @brief 変数番号のベクタ
typedef vector<tVarId> VarVector;

/// @ingroup LogicGroup
/// @brief 変数番号のリスト
typedef list<tVarId> VarList;

/// @ingroup LogicGroup
/// @brief 変数番号から変数番号への写像 (連想配列)
typedef hash_map<tVarId, tVarId> VarVarMap;

/// @ingroup LogicGroup
/// @brief 文字列から変数番号への写像 (連想配列)
typedef hash_map<string, tVarId> StrVarMap;

/// @ingroup LogicGroup
/// @brief 変数番号から文字列への写像 (連想配列)
typedef hash_map<tVarId, string> VarStrMap;

END_NAMESPACE_YM

#endif // YM_LOGIC_VARID_H
