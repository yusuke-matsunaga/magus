
/// @file libym_cell/dotlib/PtTiming.cc
/// @brief PtTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtTiming.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtTiming
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtTiming::PtTiming()
{
}

// @brief デストラクタ
PtTiming::~PtTiming()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
void
PtTiming::add_child(const ShString& attr_name,
		    PtNode* node)
{
}

// @brief テーブルを追加する．
// @param[in] attr_name 属性名
// @param[in] table テーブル
void
PtTiming::add_table(const ShString& attr_name,
		    PtTable* table)
{
}

// @brief 値の数を返す．
// @note このクラスでは常に 0
ymuint
PtTiming::value_num() const
{
  return 0;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
// @note このクラスでの呼び出しはエラーになる．
const PtValue*
PtTiming::value(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_CELL_DOTLIB
