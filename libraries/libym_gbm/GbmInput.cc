
/// @file GbmInput.cc
/// @brief GbmInput の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "GbmInput.h"


BEGIN_NAMESPACE_YM_GBM

//////////////////////////////////////////////////////////////////////
// クラス GbmInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid 変数番号
GbmInput::GbmInput(ymuint id,
		   VarId vid) :
  GbmNodeImpl(id, vid)
{
}

// @brief デストラクタ
GbmInput::~GbmInput()
{
}

// @brief 外部入力ノードの時 true を返す．
bool
GbmInput::is_input() const
{
  return true;
}

// @brief ファンイン数を返す．
// @note 外部入力ノードの場合は常に0
// @note AND ノードの場合は常に2
ymuint
GbmInput::fanin_num() const
{
  return 0;
}

// @brief ファンインのハンドルを返す．
// @param[in] pos ファンイン番号 ( 0 <= pos < fanin_num() )
GbmNodeHandle
GbmInput::fanin(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return GbmNodeHandle();
}

END_NAMESPACE_YM_GBM
